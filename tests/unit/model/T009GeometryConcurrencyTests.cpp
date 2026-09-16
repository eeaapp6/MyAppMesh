#include "TestSupport.h"

#include "model/ModelData/GeometryManager.h"
#include "model/ModelData/MeshManager.h"

#include <QCoreApplication>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

namespace
{
using namespace AppMesh::Model;

bool hasCode(const AppMesh::Common::OperationResult& result, const QString& code)
{
    for (const auto& item : result.diagnostics)
    {
        if (item.code == code)
        {
            return true;
        }
    }
    return false;
}

class StartGate
{
public:
    explicit StartGate(int participants) : m_participants(participants) {}

    void arriveAndWait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        ++m_ready;
        m_condition.notify_all();
        m_condition.wait(lock, [this] { return m_open; });
    }

    void open()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this] { return m_ready == m_participants; });
        m_open = true;
        lock.unlock();
        m_condition.notify_all();
    }

private:
    int m_participants;
    int m_ready = 0;
    bool m_open = false;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};

GeometryObject validGeometry(const QString& key, GeometryEntityId entityId)
{
    GeometryObject value;
    value.type = GeometryType::BRep;
    value.adapter.providerKey = QStringLiteral("fitk.geometry");
    value.adapter.modelKey = key;
    value.topology.append(
        {entityId, TopologyEntityType::Solid, QStringLiteral("solid-%1").arg(entityId), {}});
    return value;
}

MeshKernel validKernel()
{
    return {MeshDimension::D2,
            {{1, 0.0, 0.0, 0.0}, {2, 1.0, 0.0, 0.0}, {3, 0.0, 1.0, 0.0}},
            {{1, CellType::Triangle3, {1, 2, 3}}}};
}

class GeometryReferenceCreator final : public MeshDataCreator
{
public:
    MeshDataCreatorResult create(const MeshDataCreatorRequest& request) const override
    {
        MeshDataCreatorResult result;
        CreatedMeshData value;
        value.mesh = MeshData({QStringLiteral("geometry.reference"), QStringLiteral("1")},
                              request.geometryId,
                              {validKernel()});
        value.adapter.providerKey = QStringLiteral("fitk.mesh");
        value.adapter.meshKey = QStringLiteral("geometry-reference-mesh");
        result.created = std::move(value);
        return result;
    }
};
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    ApplicationRuntime runtime;
    GeometryManager geometryManager(runtime);

    constexpr int writerCount = 4;
    constexpr int readerCount = 2;
    constexpr int objectsPerWriter = 30;
    StartGate createGate(writerCount + readerCount);
    std::vector<std::thread> workers;
    std::mutex idsMutex;
    std::vector<ObjectId> ids;
    std::atomic<bool> operationsValid{true};

    for (int writer = 0; writer < writerCount; ++writer)
    {
        workers.emplace_back([&, writer] {
            createGate.arriveAndWait();
            for (int index = 0; index < objectsPerWriter; ++index)
            {
                CreateGeometryRequest request;
                request.name = QStringLiteral("Concurrent Geometry");
                request.geometry = validGeometry(
                    QStringLiteral("model-%1-%2").arg(writer).arg(index),
                    static_cast<GeometryEntityId>(index + 1));
                const auto created = geometryManager.createGeometry(request);
                if (!created.succeeded() || !created.object.has_value())
                {
                    operationsValid = false;
                    continue;
                }
                std::lock_guard<std::mutex> lock(idsMutex);
                ids.push_back(created.object->common.id);
            }
        });
    }
    for (int reader = 0; reader < readerCount; ++reader)
    {
        workers.emplace_back([&] {
            createGate.arriveAndWait();
            for (int iteration = 0; iteration < 300; ++iteration)
            {
                const auto snapshots = geometryManager.snapshots();
                for (const auto& snapshot : snapshots)
                {
                    if (snapshot.common.id == InvalidObjectId ||
                        snapshot.common.type != DataObjectType::Geometry ||
                        snapshot.topology.isEmpty() ||
                        !runtime.findById(snapshot.common.id).has_value())
                    {
                        operationsValid = false;
                    }
                }
            }
        });
    }
    createGate.open();
    for (auto& worker : workers)
    {
        worker.join();
    }

    const auto snapshots = geometryManager.snapshots();
    std::set<ObjectId> uniqueIds;
    std::set<QString> uniqueNames;
    for (const auto& snapshot : snapshots)
    {
        uniqueIds.insert(snapshot.common.id);
        uniqueNames.insert(snapshot.common.name.toCaseFolded());
    }
    suite.expect(operationsValid &&
                     snapshots.size() == writerCount * objectsPerWriter &&
                     uniqueIds.size() == static_cast<std::size_t>(snapshots.size()) &&
                     uniqueNames.size() == static_cast<std::size_t>(snapshots.size()) &&
                     runtime.objectCount() == geometryManager.objectCount() &&
                     runtime.validateIndexes().succeeded() &&
                     geometryManager.validateIndexes().succeeded(),
                 QStringLiteral("barrier-controlled geometry create/query exposes only complete published snapshots"));

    constexpr int deleteReaderCount = 2;
    constexpr int deleteWorkerCount = 4;
    StartGate deleteGate(deleteReaderCount + deleteWorkerCount);
    workers.clear();
    operationsValid = true;
    for (int reader = 0; reader < deleteReaderCount; ++reader)
    {
        workers.emplace_back([&] {
            deleteGate.arriveAndWait();
            for (int iteration = 0; iteration < 250; ++iteration)
            {
                const auto values = geometryManager.snapshots();
                for (const auto& value : values)
                {
                    if (value.common.id == InvalidObjectId || value.topology.size() != 1)
                    {
                        operationsValid = false;
                    }
                }
            }
        });
    }
    for (int worker = 0; worker < deleteWorkerCount; ++worker)
    {
        workers.emplace_back([&, worker] {
            deleteGate.arriveAndWait();
            for (int index = worker; index < static_cast<int>(ids.size()); index += deleteWorkerCount)
            {
                if (!geometryManager.removeGeometry(ids[static_cast<std::size_t>(index)]).succeeded())
                {
                    operationsValid = false;
                }
            }
        });
    }
    deleteGate.open();
    for (auto& worker : workers)
    {
        worker.join();
    }
    suite.expect(operationsValid && geometryManager.objectCount() == 0 &&
                     runtime.objectCount() == 0 && runtime.validateIndexes().succeeded() &&
                     geometryManager.validateIndexes().succeeded(),
                 QStringLiteral("geometry delete/query race returns snapshots or absence and clears every index"));

    CreateGeometryRequest parentRequest;
    parentRequest.name = QStringLiteral("Geometry with child");
    parentRequest.geometry = validGeometry(QStringLiteral("parent-model"), 1);
    const auto parentGeometry = geometryManager.createGeometry(parentRequest);
    CreateObjectRequest childRequest;
    childRequest.name = QStringLiteral("Geometry child");
    childRequest.type = DataObjectType::Generic;
    childRequest.parentId = parentGeometry.object->common.id;
    const auto child = runtime.createObject(childRequest);
    const auto blockedByChild = geometryManager.removeGeometry(parentGeometry.object->common.id);
    suite.expect(hasCode(blockedByChild, QStringLiteral("MODEL-PARENT-HAS-CHILDREN")) &&
                     geometryManager.findById(parentGeometry.object->common.id).has_value() &&
                     runtime.removeObject(child.object->id).succeeded() &&
                     geometryManager.removeGeometry(parentGeometry.object->common.id).succeeded() &&
                     runtime.validateIndexes().succeeded() && geometryManager.validateIndexes().succeeded(),
                 QStringLiteral("failed geometry typed-remove preserves payload and releases its removal constraint"));

    CreateObjectRequest genericRequest;
    genericRequest.name = QStringLiteral("Not geometry");
    genericRequest.type = DataObjectType::Generic;
    const auto generic = runtime.createObject(genericRequest);
    suite.expect(hasCode(geometryManager.removeGeometry(generic.object->id),
                         QStringLiteral("GEO-OBJECT-TYPE-MISMATCH")) &&
                     runtime.findById(generic.object->id).has_value(),
                 QStringLiteral("typed Geometry removal cannot delete a Generic object"));

    CreateGeometryRequest referencedRequest;
    referencedRequest.name = QStringLiteral("Referenced geometry");
    referencedRequest.geometry = validGeometry(QStringLiteral("referenced-model"), 1);
    const auto referencedGeometry = geometryManager.createGeometry(referencedRequest);
    MeshManager meshManager(runtime, geometryManager);
    meshManager.registerCreator(QStringLiteral("geometry.reference"),
                                std::make_shared<GeometryReferenceCreator>());
    CreateMeshRequest meshRequest;
    meshRequest.name = QStringLiteral("Reference mesh");
    meshRequest.creatorKey = QStringLiteral("geometry.reference");
    meshRequest.creatorRequest.geometryId = referencedGeometry.object->common.id;
    const auto mesh = meshManager.createMesh(meshRequest);
    suite.expect(mesh.succeeded() &&
                     hasCode(geometryManager.removeGeometry(referencedGeometry.object->common.id),
                             QStringLiteral("MESH-GEOMETRY-IN-USE")) &&
                     meshManager.removeMesh(mesh.object->common.id).succeeded() &&
                     geometryManager.removeGeometry(referencedGeometry.object->common.id).succeeded() &&
                     meshManager.validateIndexes().succeeded() &&
                     geometryManager.validateIndexes().succeeded(),
                 QStringLiteral("geometry deletion is rejected until every Mesh reference is removed"));

    runtime.removeObject(generic.object->id);
    return suite.result();
}
