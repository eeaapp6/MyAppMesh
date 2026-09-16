#include "TestSupport.h"

#include "model/ModelData/GeometryManager.h"
#include "model/ModelData/MeshManager.h"

#include <QCoreApplication>

#include <functional>

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

GeometryObject validGeometry()
{
    GeometryObject geometry;
    geometry.type = GeometryType::BRep;
    geometry.adapter.providerKey = QStringLiteral("fitk.geometry");
    geometry.adapter.modelKey = QStringLiteral("mesh-manager-source");
    geometry.topology.append(
        {1, TopologyEntityType::Solid, QStringLiteral("solid-1"), {}});
    return geometry;
}

MeshKernel validKernel()
{
    return {MeshDimension::D2,
            {{1, 0.0, 0.0, 0.0},
             {2, 1.0, 0.0, 0.0},
             {3, 0.0, 1.0, 0.0}},
            {{1, CellType::Triangle3, {1, 2, 3}}},
            {{1, QStringLiteral("surface"), MeshEntityType::Element, {1}}}};
}

class FakeMeshCreator final : public MeshDataCreator
{
public:
    explicit FakeMeshCreator(
        QString key = QStringLiteral("fake.gmsh"),
        std::function<void()> duringCreate = {})
        : m_key(std::move(key)), m_duringCreate(std::move(duringCreate))
    {
    }

    MeshDataCreatorResult create(const MeshDataCreatorRequest& request) const override
    {
        if (m_duringCreate)
        {
            m_duringCreate();
        }
        MeshDataCreatorResult result;
        CreatedMeshData created;
        created.mesh = MeshData({m_key, QStringLiteral("1.0")},
                                request.geometryId,
                                {validKernel()},
                                request.commonParameters,
                                request.generatorParameters,
                                request.displayMetadata);
        created.adapter.providerKey = QStringLiteral("fitk.mesh");
        created.adapter.meshKey = QStringLiteral("opaque-mesh-key");
        result.created = std::move(created);
        return result;
    }

private:
    QString m_key;
    std::function<void()> m_duringCreate;
};

AppMesh::Common::OperationResult injectedFailure(ObjectId id)
{
    AppMesh::Common::OperationResult result;
    result.add({QStringLiteral("test"),
                QStringLiteral("TEST-MESH-PUBLISH-REJECTED"),
                QStringLiteral("Mesh publication was rejected."),
                QStringLiteral("Exercise payload, association and reservation rollback."),
                true,
                QStringLiteral("object"),
                QStringLiteral("mesh.create.publish"),
                QString::number(id)});
    return result;
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    ApplicationRuntime runtime;
    GeometryManager geometryManager(runtime);

    bool failBeforePublish = false;
    MeshManager manager(
        runtime,
        geometryManager,
        [&](ObjectId id) {
            return failBeforePublish
                ? injectedFailure(id)
                : AppMesh::Common::OperationResult{};
        });

    auto creator = std::make_shared<FakeMeshCreator>();
    suite.expect(manager.registerCreator(QStringLiteral("fake.gmsh"), creator).succeeded() &&
                     hasCode(manager.registerCreator(QStringLiteral("fake.gmsh"), creator),
                             QStringLiteral("MESH-CREATOR-DUPLICATE")) &&
                     manager.creatorKeys() == QStringList{QStringLiteral("fake.gmsh")},
                 QStringLiteral("creator keys are unique and duplicate registration does not overwrite"));

    CreateGeometryRequest geometryRequest;
    geometryRequest.name = QStringLiteral("Source geometry");
    geometryRequest.geometry = validGeometry();
    const auto geometry = geometryManager.createGeometry(geometryRequest);
    suite.expect(geometry.succeeded() && geometry.object.has_value(),
                 QStringLiteral("mesh manager fixture has a published geometry"));
    const ObjectId geometryId = geometry.object->common.id;

    CreateMeshRequest request;
    request.name = QStringLiteral("Surface mesh");
    request.creatorKey = QStringLiteral("fake.gmsh");
    request.creatorRequest.geometryId = geometryId;
    request.creatorRequest.generatorParameters.insert(
        QStringLiteral("algorithm"), QStringLiteral("test"));
    const auto created = manager.createMesh(request);
    const ObjectId meshId = created.object.has_value()
        ? created.object->common.id
        : InvalidObjectId;
    suite.expect(created.succeeded() && created.object.has_value() &&
                     created.object->mesh.geometryId() == geometryId &&
                     created.object->adapterProviderKey == QStringLiteral("fitk.mesh") &&
                     manager.meshIdsForGeometry(geometryId) == QVector<ObjectId>{meshId} &&
                     manager.geometryIdForMesh(meshId) == std::optional<ObjectId>{geometryId} &&
                     manager.validateIndexes().succeeded(),
                 QStringLiteral("validated creator output is atomically published with both association indexes"));

    suite.expect(hasCode(manager.unregisterCreator(QStringLiteral("fake.gmsh")),
                         QStringLiteral("MESH-CREATOR-IN-USE")) &&
                     hasCode(geometryManager.removeGeometry(geometryId),
                             QStringLiteral("MESH-GEOMETRY-IN-USE")) &&
                     geometryManager.findById(geometryId).has_value(),
                 QStringLiteral("active mesh data retains its creator and prevents geometry deletion"));

    AppMesh::Common::OperationResult activeUnregister;
    auto reentrantCreator = std::make_shared<FakeMeshCreator>(
        QStringLiteral("active.creator"),
        [&]() {
            activeUnregister = manager.unregisterCreator(QStringLiteral("active.creator"));
        });
    suite.expect(manager.registerCreator(QStringLiteral("active.creator"),
                                         reentrantCreator).succeeded(),
                 QStringLiteral("a creator can be registered for active-operation validation"));
    CreateMeshRequest activeRequest;
    activeRequest.name = QStringLiteral("Active creator mesh");
    activeRequest.creatorKey = QStringLiteral("active.creator");
    const auto activeMesh = manager.createMesh(activeRequest);
    suite.expect(activeMesh.succeeded() && activeMesh.object.has_value() &&
                     hasCode(activeUnregister, QStringLiteral("MESH-CREATOR-ACTIVE")) &&
                     manager.removeMesh(activeMesh.object->common.id).succeeded() &&
                     manager.unregisterCreator(QStringLiteral("active.creator")).succeeded(),
                 QStringLiteral("creator callbacks run without the manager lock and active unregister is rejected"));

    const int runtimeCount = runtime.objectCount();
    const int meshCount = manager.objectCount();
    CreateMeshRequest missingGeometry = request;
    missingGeometry.name = QStringLiteral("Missing geometry mesh");
    missingGeometry.creatorRequest.geometryId = geometryId + 1000;
    const auto missing = manager.createMesh(missingGeometry);
    suite.expect(hasCode(missing, QStringLiteral("MESH-GEOMETRY-NOT-FOUND")) &&
                     runtime.objectCount() == runtimeCount && manager.objectCount() == meshCount &&
                     manager.validateIndexes().succeeded(),
                 QStringLiteral("invalid geometry IDs fail validation without common or association residue"));

    failBeforePublish = true;
    CreateMeshRequest rollbackRequest = request;
    rollbackRequest.name = QStringLiteral("Rollback mesh");
    const auto rolledBack = manager.createMesh(rollbackRequest);
    failBeforePublish = false;
    suite.expect(hasCode(rolledBack, QStringLiteral("TEST-MESH-PUBLISH-REJECTED")) &&
                     runtime.objectCount() == runtimeCount && manager.objectCount() == meshCount &&
                     manager.meshIdsForGeometry(geometryId) == QVector<ObjectId>{meshId} &&
                     manager.validateIndexes().succeeded(),
                 QStringLiteral("publish failure rolls back payload, both indexes and hidden reservation"));

    CreateObjectRequest bypass;
    bypass.name = QStringLiteral("Bypass mesh");
    bypass.type = DataObjectType::Mesh;
    suite.expect(hasCode(runtime.createObject(bypass),
                         QStringLiteral("MODEL-DOMAIN-MANAGER-REQUIRED")) &&
                     hasCode(runtime.removeObject(meshId),
                             QStringLiteral("MODEL-DOMAIN-MANAGER-REQUIRED")),
                 QStringLiteral("public Runtime APIs cannot bypass MeshManager lifecycle"));

    suite.expect(manager.removeMesh(meshId).succeeded() &&
                     !manager.findById(meshId).has_value() &&
                     manager.meshIdsForGeometry(geometryId).isEmpty() &&
                     !manager.geometryIdForMesh(meshId).has_value() &&
                     manager.validateIndexes().succeeded() &&
                     geometryManager.removeGeometry(geometryId).succeeded() &&
                     manager.unregisterCreator(QStringLiteral("fake.gmsh")).succeeded() &&
                     runtime.objectCount() == 0,
                 QStringLiteral("mesh deletion clears both indexes and then permits geometry and creator removal"));

    return suite.result();
}
