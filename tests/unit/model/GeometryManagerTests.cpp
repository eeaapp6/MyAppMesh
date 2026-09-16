#include "TestSupport.h"

#include "model/ModelData/GeometryManager.h"

#include <QCoreApplication>

namespace
{
using namespace AppMesh::Model;

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

GeometryObject validGeometry(const QString& modelKey, GeometryEntityId entityId = 1)
{
    GeometryObject geometry;
    geometry.type = GeometryType::BRep;
    geometry.adapter.providerKey = QStringLiteral("fitk.geometry");
    geometry.adapter.modelKey = modelKey;
    geometry.topology.append(
        {entityId, TopologyEntityType::Solid, QStringLiteral("solid-1"), {}});
    return geometry;
}

AppMesh::Common::OperationResult injectedFailure(ObjectId id)
{
    AppMesh::Common::OperationResult result;
    result.add({QStringLiteral("test"),
                QStringLiteral("TEST-PUBLISH-REJECTED"),
                QStringLiteral("Publication was rejected by the test checkpoint."),
                QStringLiteral("Exercise payload rollback and reservation cancellation."),
                true,
                QStringLiteral("object"),
                QStringLiteral("geometry.commit"),
                QString::number(id)});
    return result;
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    ApplicationRuntime runtime;

    bool failBeforePublish = false;
    bool runtimeWasHidden = true;
    bool joinedViewWasHidden = true;
    GeometryManager* managerView = nullptr;
    GeometryManager manager(
        runtime,
        [&](ObjectId id) {
            runtimeWasHidden = runtimeWasHidden && !runtime.findById(id).has_value();
            joinedViewWasHidden = joinedViewWasHidden &&
                (!managerView || !managerView->findById(id).has_value());
            return failBeforePublish
                ? injectedFailure(id)
                : AppMesh::Common::OperationResult{};
        });
    managerView = &manager;

    GeometryObject invalid;
    invalid.type = GeometryType::Invalid;
    invalid.topology.append(
        {InvalidGeometryEntityId, TopologyEntityType::Invalid, QString(), {}});
    const auto invalidStage = manager.stageGeometry(invalid);
    suite.expect(!invalidStage.succeeded() && !invalidStage.staged.has_value() &&
                     hasCode(invalidStage, QStringLiteral("GEO-TYPE-INVALID")) &&
                     hasCode(invalidStage, QStringLiteral("GEO-ENTITY-ID-INVALID")) &&
                     runtime.objectCount() == 0 && manager.objectCount() == 0,
                 QStringLiteral("invalid geometry is diagnosed and never enters either manager"));

    GeometryObject duplicateTopology = validGeometry(QStringLiteral("duplicate-topology"));
    duplicateTopology.topology.append(
        {1, TopologyEntityType::Face, QStringLiteral("face-1"), {}});
    suite.expect(hasCode(manager.stageGeometry(duplicateTopology),
                         QStringLiteral("GEO-ENTITY-ID-DUPLICATE")) &&
                     hasCode(manager.removeGeometry(InvalidObjectId),
                             QStringLiteral("GEO-OBJECT-ID-INVALID")),
                 QStringLiteral("duplicate entity IDs and invalid object IDs are rejected"));

    const auto staged = manager.stageGeometry(validGeometry(QStringLiteral("model-a")));
    CreateObjectRequest common;
    common.name = QStringLiteral("Body");
    const auto first = staged.staged.has_value()
        ? manager.commitGeometry(common, *staged.staged)
        : CommitGeometryResult{};
    suite.expect(staged.succeeded() && first.succeeded() && first.object.has_value() &&
                     first.object->common.name == QStringLiteral("Body") &&
                     runtimeWasHidden && joinedViewWasHidden,
                 QStringLiteral("staged payload is hidden until its common record is published"));

    CreateGeometryRequest duplicate;
    duplicate.name = QStringLiteral(" body ");
    duplicate.geometry = validGeometry(QStringLiteral("model-b"), 2);
    const auto second = manager.createGeometry(duplicate);
    suite.expect(second.succeeded() && second.object.has_value() &&
                     second.object->common.name == QStringLiteral("body (2)"),
                 QStringLiteral("geometry names use the runtime unique-name index"));

    CreateObjectRequest bypass;
    bypass.name = QStringLiteral("Bypass");
    bypass.type = DataObjectType::Geometry;
    suite.expect(hasCode(runtime.createObject(bypass),
                         QStringLiteral("MODEL-DOMAIN-MANAGER-REQUIRED")),
                 QStringLiteral("the public runtime create API cannot bypass GeometryManager"));

    const ObjectId firstId = first.object->common.id;
    suite.expect(hasCode(runtime.removeObject(firstId),
                         QStringLiteral("MODEL-DOMAIN-MANAGER-REQUIRED")),
                 QStringLiteral("the public runtime remove API cannot bypass GeometryManager"));

    CreateObjectRequest groupRequest;
    groupRequest.name = QStringLiteral("Group");
    groupRequest.type = DataObjectType::Generic;
    const auto group = runtime.createObject(groupRequest);
    suite.expect(group.succeeded() &&
                     hasCode(manager.removeGeometry(group.object->id),
                             QStringLiteral("GEO-OBJECT-TYPE-MISMATCH")) &&
                     runtime.findById(group.object->id).has_value() &&
                     manager.setParent(firstId, group.object->id).succeeded() &&
                     runtime.childIds(group.object->id).contains(firstId),
                 QStringLiteral("typed removal rejects non-Geometry and grouping uses Runtime"));

    suite.expect(manager.renameGeometry(firstId, QStringLiteral("Renamed Body")).succeeded() &&
                     manager.setVisible(firstId, false).succeeded() &&
                     manager.setSelected(firstId, true).succeeded() &&
                     manager.setDisplayMetadataValue(
                         firstId, QStringLiteral("color"), QStringLiteral("blue")).succeeded(),
                 QStringLiteral("geometry business entry points update common and domain state"));
    const auto changed = manager.findById(firstId);
    suite.expect(changed.has_value() &&
                     changed->common.name == QStringLiteral("Renamed Body") &&
                     !changed->common.visible && changed->common.selected &&
                     changed->displayMetadata.value(QStringLiteral("color")).toString() ==
                         QStringLiteral("blue") &&
                     changed->topology.size() == 1,
                 QStringLiteral("geometry queries return a complete detached aggregate snapshot"));

    const int runtimeCountBeforeRollback = runtime.objectCount();
    const int geometryCountBeforeRollback = manager.objectCount();
    failBeforePublish = true;
    CreateGeometryRequest rollback;
    rollback.name = QStringLiteral("Rollback");
    rollback.geometry = validGeometry(QStringLiteral("model-rollback"), 3);
    const auto rejected = manager.createGeometry(rollback);
    failBeforePublish = false;
    suite.expect(!rejected.succeeded() &&
                     hasCode(rejected, QStringLiteral("TEST-PUBLISH-REJECTED")) &&
                     runtime.objectCount() == runtimeCountBeforeRollback &&
                     manager.objectCount() == geometryCountBeforeRollback &&
                     !runtime.findByName(QStringLiteral("Rollback")).has_value() &&
                     runtime.validateIndexes().succeeded() && manager.validateIndexes().succeeded(),
                 QStringLiteral("failed publication removes payload and cancels the hidden reservation"));

    suite.expect(manager.removeGeometry(second.object->common.id).succeeded() &&
                     !runtime.findById(second.object->common.id).has_value() &&
                     !manager.findById(second.object->common.id).has_value(),
                 QStringLiteral("typed removal deletes both the common record and geometry payload"));
    suite.expect(manager.removeGeometry(firstId).succeeded() &&
                     runtime.removeObject(group.object->id).succeeded() &&
                     manager.objectCount() == 0 && runtime.objectCount() == 0 &&
                     runtime.validateIndexes().succeeded() && manager.validateIndexes().succeeded(),
                 QStringLiteral("successful deletion leaves no common, payload, or parent index residue"));

    return suite.result();
}
