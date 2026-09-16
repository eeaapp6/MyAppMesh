#include "TestSupport.h"

#include "model/ModelData/ApplicationRuntime.h"

#include <QCoreApplication>

#include <limits>

namespace
{
using namespace AppMesh::Model;

CreateObjectRequest request(const QString& name,
                            ObjectId parentId = InvalidObjectId,
                            DataObjectType type = DataObjectType::Generic)
{
    CreateObjectRequest value;
    value.name = name;
    value.parentId = parentId;
    value.type = type;
    return value;
}

bool hasCode(const AppMesh::Common::OperationResult& result, const QString& code)
{
    for (const auto& diagnostic : result.diagnostics)
    {
        if (diagnostic.code == code)
        {
            return true;
        }
    }
    return false;
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;

    ApplicationRuntime runtime;
    const auto first = runtime.createObject(request(QStringLiteral("Object")));
    const auto second = runtime.createObject(request(QStringLiteral(" object ")));
    const auto third = runtime.createObject(request(QStringLiteral("OBJECT")));
    suite.expect(first.object->name == QStringLiteral("Object") &&
                     second.object->name == QStringLiteral("object (2)") &&
                     third.object->name == QStringLiteral("OBJECT (3)"),
                 QStringLiteral("case-insensitive duplicate names receive deterministic suffixes"));
    suite.expect(first.object->id < second.object->id && second.object->id < third.object->id,
                 QStringLiteral("IDs are unique and monotonic"));
    suite.expect(runtime.findByName(QStringLiteral("oBjEcT"))->id == first.object->id,
                 QStringLiteral("name lookup is trimmed and case-insensitive"));

    const auto renamed = runtime.renameObject(second.object->id, QStringLiteral("Object"));
    suite.expect(renamed.succeeded() &&
                     runtime.findById(second.object->id)->name == QStringLiteral("Object (4)"),
                 QStringLiteral("rename updates the index without overwriting a name"));
    const auto oldName = runtime.findById(first.object->id)->name;
    const auto invalidRename = runtime.renameObject(first.object->id, QStringLiteral("  "));
    suite.expect(hasCode(invalidRename, QStringLiteral("MODEL-NAME-EMPTY")) &&
                     runtime.findById(first.object->id)->name == oldName,
                 QStringLiteral("failed rename leaves the old index unchanged"));

    const auto parent = runtime.createObject(request(QStringLiteral("Parent")));
    const auto child = runtime.createObject(request(QStringLiteral("Child"), parent.object->id));
    suite.expect(runtime.childIds(parent.object->id) ==
                     (QVector<ObjectId>() << child.object->id),
                 QStringLiteral("parent and child indexes are synchronized"));
    const auto parentRemoval = runtime.removeObject(parent.object->id);
    suite.expect(hasCode(parentRemoval, QStringLiteral("MODEL-PARENT-HAS-CHILDREN")) &&
                     runtime.findById(parent.object->id).has_value(),
                 QStringLiteral("a parent with children cannot be removed"));
    suite.expect(hasCode(runtime.setParent(parent.object->id, parent.object->id),
                         QStringLiteral("MODEL-PARENT-SELF")),
                 QStringLiteral("self-parenting is rejected"));
    suite.expect(hasCode(runtime.setParent(parent.object->id, child.object->id),
                         QStringLiteral("MODEL-PARENT-CYCLE")),
                 QStringLiteral("parent cycles are rejected"));
    suite.expect(runtime.removeObject(child.object->id).succeeded() &&
                     runtime.childIds(parent.object->id).isEmpty() &&
                     runtime.removeObject(parent.object->id).succeeded(),
                 QStringLiteral("leaf deletion clears reverse indexes"));

    const int beforeInvalid = runtime.objectCount();
    suite.expect(hasCode(runtime.createObject(request(QStringLiteral(""))),
                         QStringLiteral("MODEL-NAME-EMPTY")) &&
                     hasCode(runtime.createObject(request(QStringLiteral("Bad"),
                                                         InvalidObjectId,
                                                         DataObjectType::Invalid)),
                             QStringLiteral("MODEL-TYPE-INVALID")) &&
                     hasCode(runtime.createObject(request(QStringLiteral("Orphan"), 999999)),
                             QStringLiteral("MODEL-PARENT-NOT-FOUND")) &&
                     runtime.objectCount() == beforeInvalid,
                 QStringLiteral("invalid creates are atomic"));

    const ObjectId removedId = third.object->id;
    suite.expect(runtime.removeObject(removedId).succeeded(),
                 QStringLiteral("existing objects can be removed"));
    const auto afterRemoval = runtime.createObject(request(QStringLiteral("After removal")));
    suite.expect(afterRemoval.object->id > removedId,
                 QStringLiteral("deleted IDs are not reused"));

    ApplicationRuntime exhausted(std::numeric_limits<ObjectId>::max());
    const auto last = exhausted.createObject(request(QStringLiteral("Last")));
    const auto overflow = exhausted.createObject(request(QStringLiteral("Overflow")));
    suite.expect(last.succeeded() && last.object->id == std::numeric_limits<ObjectId>::max() &&
                     hasCode(overflow, QStringLiteral("MODEL-ID-EXHAUSTED")) &&
                     exhausted.objectCount() == 1,
                 QStringLiteral("ID exhaustion is diagnosed without wraparound"));

    suite.expect(runtime.validateIndexes().succeeded(),
                 QStringLiteral("all runtime indexes remain consistent"));
    suite.expect(hasCode(runtime.removeObject(999999), QStringLiteral("MODEL-OBJECT-NOT-FOUND")),
                 QStringLiteral("missing-object deletion is deterministic"));

    return suite.result();
}
