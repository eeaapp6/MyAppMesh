#include "TestSupport.h"

#include "model/ModelData/ApplicationRuntime.h"

#include <QCoreApplication>

#include <type_traits>

namespace
{
using namespace AppMesh::Model;

CreateObjectRequest request(const QString& name)
{
    CreateObjectRequest value;
    value.name = name;
    value.type = DataObjectType::Generic;
    value.metadata.insert(QStringLiteral("owner"), QStringLiteral("initial"));
    return value;
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;

    static_assert(!std::is_copy_constructible<DataObject>::value,
                  "DataObject must not be copied with a stable ID");
    static_assert(!std::is_move_constructible<DataObject>::value,
                  "DataObject ownership remains inside ApplicationRuntime");

    ApplicationRuntime runtime;
    auto created = runtime.createObject(request(QStringLiteral("  Object  ")));
    suite.expect(created.succeeded() && created.object.has_value(),
                 QStringLiteral("a valid generic object is created"));
    const auto initial = created.object.value();
    suite.expect(initial.id != InvalidObjectId && initial.name == QStringLiteral("Object") &&
                     initial.type == DataObjectType::Generic && initial.visible &&
                     !initial.selected && initial.parentId == InvalidObjectId &&
                     initial.createdAt.isValid(),
                 QStringLiteral("the immutable and display fields are initialized"));

    const auto createdAt = initial.createdAt;
    runtime.setVisible(initial.id, false);
    runtime.setSelected(initial.id, true);
    runtime.setMetadataValue(initial.id, QStringLiteral("owner"), QStringLiteral("updated"));
    auto changed = runtime.findById(initial.id);
    suite.expect(changed.has_value() && !changed->visible && changed->selected &&
                     changed->createdAt == createdAt &&
                     changed->metadata.value(QStringLiteral("owner")).toString() ==
                         QStringLiteral("updated"),
                 QStringLiteral("controlled updates preserve ID and creation time"));

    QVariantMap detachedMetadata = changed->metadata;
    detachedMetadata.insert(QStringLiteral("owner"), QStringLiteral("detached"));
    suite.expect(runtime.findById(initial.id)->metadata.value(QStringLiteral("owner")).toString() ==
                     QStringLiteral("updated"),
                 QStringLiteral("metadata queries return detached values"));

    return suite.result();
}
