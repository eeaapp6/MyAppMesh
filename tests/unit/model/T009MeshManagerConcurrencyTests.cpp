#include "TestSupport.h"

#include "model/ModelData/GeometryManager.h"
#include "model/ModelData/MeshManager.h"

#include <QCoreApplication>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <set>
#include <stdexcept>
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

GeometryObject validGeometry(const QString& key)
{
    GeometryObject value;
    value.type = GeometryType::BRep;
    value.adapter.providerKey = QStringLiteral("fitk.geometry");
    value.adapter.modelKey = key;
    value.topology.append({1, TopologyEntityType::Solid, QStringLiteral("solid-1"), {}});
    return value;
}

MeshKernel validKernel()
{
    return {MeshDimension::D2,
            {{1, 0.0, 0.0, 0.0}, {2, 1.0, 0.0, 0.0}, {3, 0.0, 1.0, 0.0}},
            {{1, CellType::Triangle3, {1, 2, 3}}},
            {{1, QStringLiteral("surface"), MeshEntityType::Element, {1}}}};
}

CreatedMeshData createdMesh(const QString& key, ObjectId geometryId)
{
    CreatedMeshData value;
    value.mesh = MeshData({key, QStringLiteral("1")}, geometryId, {validKernel()});
    value.adapter.providerKey = QStringLiteral("fitk.mesh");
    value.adapter.meshKey = QStringLiteral("opaque-%1").arg(key);
    return value;
}

class SuccessfulMeshCreator final : public MeshDataCreator
{
public:
    explicit SuccessfulMeshCreator(QString key) : m_key(std::move(key)) {}

    MeshDataCreatorResult create(const MeshDataCreatorRequest& request) const override
    {
        MeshDataCreatorResult result;
        result.created = createdMesh(m_key, request.geometryId);
        return result;
    }

private:
    QString m_key;
};

class ThrowingMeshCreator final : public MeshDataCreator
{
public:
    MeshDataCreatorResult create(const MeshDataCreatorRequest&) const override
    {
        throw std::runtime_error("controlled creator failure");
    }
};

class FailingMeshCreator final : public MeshDataCreator
{
public:
    MeshDataCreatorResult create(const MeshDataCreatorRequest&) const override
    {
        MeshDataCreatorResult result;
        result.add({QStringLiteral("test"),
                    QStringLiteral("TEST-CREATOR-FAILED"),
                    QStringLiteral("Creator rejected the request."),
                    QStringLiteral("Controlled failure result."),
                    true,
                    QStringLiteral("creator"),
                    QStringLiteral("mesh.create.creator"),
                    QString()});
        return result;
    }
};

class NullMeshCreator final : public MeshDataCreator
{
public:
    MeshDataCreatorResult create(const MeshDataCreatorRequest&) const override
    {
        return {};
    }
};

class MismatchedKeyMeshCreator final : public MeshDataCreator
{
public:
    MeshDataCreatorResult create(const MeshDataCreatorRequest& request) const override
    {
        MeshDataCreatorResult result;
        result.created = createdMesh(QStringLiteral("wrong.key"), request.geometryId);
        return result;
    }
};

class InvalidMeshCreator final : public MeshDataCreator
{
public:
    MeshDataCreatorResult create(const MeshDataCreatorRequest& request) const override
    {
        MeshDataCreatorResult result;
        CreatedMeshData value;
        value.mesh = MeshData({QStringLiteral("invalid.creator"), QString()},
                              request.geometryId,
                              {});
        value.adapter.providerKey = QStringLiteral("fitk.mesh");
        value.adapter.meshKey = QStringLiteral("invalid-result");
        result.created = std::move(value);
        return result;
    }
};

class InvalidAdapterMeshCreator final : public MeshDataCreator
{
public:
    MeshDataCreatorResult create(const MeshDataCreatorRequest& request) const override
    {
        MeshDataCreatorResult result;
        auto value = createdMesh(QStringLiteral("invalid.adapter"), request.geometryId);
        value.adapter.meshKey.clear();
        result.created = std::move(value);
        return result;
    }
};

class BlockingMeshCreator final : public MeshDataCreator
{
public:
    MeshDataCreatorResult create(const MeshDataCreatorRequest& request) const override
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_entered = true;
        }
        m_condition.notify_all();
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this] { return m_released; });
        MeshDataCreatorResult result;
        result.created = createdMesh(QStringLiteral("blocking.creator"), request.geometryId);
        return result;
    }

    bool waitUntilEntered() const
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_condition.wait_for(
            lock, std::chrono::seconds(5), [this] { return m_entered; });
    }

    void release() const
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_released = true;
        }
        m_condition.notify_all();
    }

private:
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_condition;
    mutable bool m_entered = false;
    mutable bool m_released = false;
};

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

CreateMeshRequest request(const QString& name,
                          const QString& creatorKey,
                          ObjectId geometryId = InvalidObjectId)
{
    CreateMeshRequest value;
    value.name = name;
    value.creatorKey = creatorKey;
    value.creatorRequest.geometryId = geometryId;
    return value;
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    ApplicationRuntime runtime;
    GeometryManager geometryManager(runtime);
    MeshManager meshManager(runtime, geometryManager);

    auto primaryCreator = std::make_shared<SuccessfulMeshCreator>(QStringLiteral("primary.creator"));
    suite.expect(meshManager.registerCreator(QStringLiteral("primary.creator"), primaryCreator).succeeded() &&
                     hasCode(meshManager.registerCreator(QString(), primaryCreator),
                             QStringLiteral("MESH-CREATOR-KEY-EMPTY")) &&
                     hasCode(meshManager.registerCreator(QStringLiteral("null.creator"), {}),
                             QStringLiteral("MESH-CREATOR-NULL")) &&
                     hasCode(meshManager.registerCreator(QStringLiteral("primary.creator"),
                                                         std::make_shared<ThrowingMeshCreator>()),
                             QStringLiteral("MESH-CREATOR-DUPLICATE")) &&
                     hasCode(meshManager.unregisterCreator(QStringLiteral("missing.creator")),
                             QStringLiteral("MESH-CREATOR-NOT-FOUND")) &&
                     meshManager.creatorKeys() == QStringList{QStringLiteral("primary.creator")},
                 QStringLiteral("creator registry rejects empty, null, duplicate and missing keys without overwrite"));

    struct FailureCase
    {
        QString key;
        std::shared_ptr<const MeshDataCreator> creator;
        QString expectedCode;
    };
    const std::vector<FailureCase> failures = {
        {QStringLiteral("throwing.creator"),
         std::make_shared<ThrowingMeshCreator>(),
         QStringLiteral("MESH-CREATOR-EXCEPTION")},
        {QStringLiteral("failing.creator"),
         std::make_shared<FailingMeshCreator>(),
         QStringLiteral("TEST-CREATOR-FAILED")},
        {QStringLiteral("null.result"),
         std::make_shared<NullMeshCreator>(),
         QStringLiteral("MESH-CREATOR-RESULT-MISSING")},
        {QStringLiteral("mismatch.creator"),
         std::make_shared<MismatchedKeyMeshCreator>(),
         QStringLiteral("MESH-CREATOR-KEY-MISMATCH")},
        {QStringLiteral("invalid.creator"),
         std::make_shared<InvalidMeshCreator>(),
         QStringLiteral("MESH-GENERATOR-VERSION-EMPTY")},
        {QStringLiteral("invalid.adapter"),
         std::make_shared<InvalidAdapterMeshCreator>(),
         QStringLiteral("MESH-ADAPTER-REFERENCE-INVALID")}};
    for (const auto& failure : failures)
    {
        const int beforeRuntime = runtime.objectCount();
        const int beforeMeshes = meshManager.objectCount();
        meshManager.registerCreator(failure.key, failure.creator);
        const auto result = meshManager.createMesh(
            request(QStringLiteral("Failure %1").arg(failure.key), failure.key));
        suite.expect(hasCode(result, failure.expectedCode) &&
                         runtime.objectCount() == beforeRuntime &&
                         meshManager.objectCount() == beforeMeshes &&
                         meshManager.validateIndexes().succeeded() &&
                         meshManager.unregisterCreator(failure.key).succeeded(),
                     QStringLiteral("creator failure '%1' leaves no common record, payload, association or activity")
                         .arg(failure.key));
    }

    CreateObjectRequest genericRequest;
    genericRequest.name = QStringLiteral("Generic source");
    genericRequest.type = DataObjectType::Generic;
    const auto generic = runtime.createObject(genericRequest);
    const int beforeBadGeometry = runtime.objectCount();
    const auto missingGeometry = meshManager.createMesh(
        request(QStringLiteral("Missing geometry"), QStringLiteral("primary.creator"), 999999));
    const auto genericGeometry = meshManager.createMesh(
        request(QStringLiteral("Generic geometry"),
                QStringLiteral("primary.creator"),
                generic.object->id));
    suite.expect(hasCode(missingGeometry, QStringLiteral("MESH-GEOMETRY-NOT-FOUND")) &&
                     hasCode(genericGeometry, QStringLiteral("MESH-GEOMETRY-TYPE-MISMATCH")) &&
                     runtime.objectCount() == beforeBadGeometry &&
                     meshManager.objectCount() == 0 && meshManager.validateIndexes().succeeded(),
                 QStringLiteral("missing and non-Geometry references fail before reservation"));

    auto blockingCreator = std::make_shared<BlockingMeshCreator>();
    meshManager.registerCreator(QStringLiteral("blocking.creator"), blockingCreator);
    CreateMeshResult blockedCreate;
    std::thread blockedWorker([&] {
        blockedCreate = meshManager.createMesh(
            request(QStringLiteral("Blocked mesh"), QStringLiteral("blocking.creator")));
    });
    const bool entered = blockingCreator->waitUntilEntered();
    const auto activeUnregister = meshManager.unregisterCreator(QStringLiteral("blocking.creator"));
    blockingCreator->release();
    blockedWorker.join();
    suite.expect(entered && hasCode(activeUnregister, QStringLiteral("MESH-CREATOR-ACTIVE")) &&
                     blockedCreate.succeeded() && blockedCreate.object.has_value() &&
                     meshManager.removeMesh(blockedCreate.object->common.id).succeeded() &&
                     meshManager.unregisterCreator(QStringLiteral("blocking.creator")).succeeded(),
                 QStringLiteral("condition-controlled active creator cannot be unregistered and finishes without deadlock"));

    CreateGeometryRequest geometryRequest;
    geometryRequest.name = QStringLiteral("Shared mesh geometry");
    geometryRequest.geometry = validGeometry(QStringLiteral("shared-mesh-geometry"));
    const auto geometry = geometryManager.createGeometry(geometryRequest);
    constexpr int creatorThreads = 4;
    constexpr int meshesPerThread = 20;
    StartGate createGate(creatorThreads);
    std::vector<std::thread> workers;
    std::mutex resultMutex;
    std::vector<ObjectId> meshIds;
    std::atomic<bool> operationsValid{true};
    for (int threadIndex = 0; threadIndex < creatorThreads; ++threadIndex)
    {
        workers.emplace_back([&, threadIndex] {
            createGate.arriveAndWait();
            for (int index = 0; index < meshesPerThread; ++index)
            {
                const auto created = meshManager.createMesh(
                    request(QStringLiteral("Concurrent mesh"),
                            QStringLiteral("primary.creator"),
                            geometry.object->common.id));
                if (!created.succeeded() || !created.object.has_value())
                {
                    operationsValid = false;
                    continue;
                }
                std::lock_guard<std::mutex> lock(resultMutex);
                meshIds.push_back(created.object->common.id);
            }
        });
    }
    createGate.open();
    for (auto& worker : workers)
    {
        worker.join();
    }

    const auto associated = meshManager.meshIdsForGeometry(geometry.object->common.id);
    std::set<ObjectId> uniqueAssociated(associated.cbegin(), associated.cend());
    bool reverseMatches = true;
    for (const ObjectId meshId : associated)
    {
        reverseMatches = reverseMatches &&
            meshManager.geometryIdForMesh(meshId) ==
                std::optional<ObjectId>{geometry.object->common.id};
    }
    suite.expect(operationsValid && associated.size() == creatorThreads * meshesPerThread &&
                     uniqueAssociated.size() == static_cast<std::size_t>(associated.size()) &&
                     reverseMatches && meshManager.validateIndexes().succeeded() &&
                     geometryManager.validateIndexes().succeeded(),
                 QStringLiteral("same creator concurrently publishes unique meshes with symmetric associations"));

    StartGate mixedGate(creatorThreads + 2);
    workers.clear();
    for (int reader = 0; reader < 2; ++reader)
    {
        workers.emplace_back([&] {
            mixedGate.arriveAndWait();
            for (int iteration = 0; iteration < 300; ++iteration)
            {
                const auto values = meshManager.snapshots();
                for (const auto& value : values)
                {
                    if (value.common.type != DataObjectType::Mesh ||
                        value.common.id == InvalidObjectId ||
                        !value.mesh.validate(runtime).succeeded())
                    {
                        operationsValid = false;
                    }
                }
                meshManager.meshIdsForGeometry(geometry.object->common.id);
            }
        });
    }
    for (int worker = 0; worker < creatorThreads; ++worker)
    {
        workers.emplace_back([&, worker] {
            mixedGate.arriveAndWait();
            for (int index = worker; index < static_cast<int>(meshIds.size()); index += creatorThreads)
            {
                if (!meshManager.removeMesh(meshIds[static_cast<std::size_t>(index)]).succeeded())
                {
                    operationsValid = false;
                }
            }
        });
    }
    mixedGate.open();
    for (auto& worker : workers)
    {
        worker.join();
    }
    suite.expect(operationsValid && meshManager.objectCount() == 0 &&
                     meshManager.meshIdsForGeometry(geometry.object->common.id).isEmpty() &&
                     meshManager.validateIndexes().succeeded() &&
                     geometryManager.validateIndexes().succeeded(),
                 QStringLiteral("mesh delete/snapshot/index-read stress leaves no one-sided association"));

    CreateGeometryRequest secondGeometryRequest;
    secondGeometryRequest.name = QStringLiteral("Second mesh geometry");
    secondGeometryRequest.geometry = validGeometry(QStringLiteral("second-mesh-geometry"));
    const auto secondGeometry = geometryManager.createGeometry(secondGeometryRequest);
    const auto firstAssociated = meshManager.createMesh(
        request(QStringLiteral("First associated"),
                QStringLiteral("primary.creator"),
                geometry.object->common.id));
    const auto secondAssociated = meshManager.createMesh(
        request(QStringLiteral("Second associated"),
                QStringLiteral("primary.creator"),
                secondGeometry.object->common.id));
    const auto orphan = meshManager.createMesh(
        request(QStringLiteral("Orphan mesh"), QStringLiteral("primary.creator")));
    suite.expect(firstAssociated.succeeded() && secondAssociated.succeeded() && orphan.succeeded() &&
                     meshManager.meshIdsForGeometry(geometry.object->common.id) ==
                         QVector<ObjectId>{firstAssociated.object->common.id} &&
                     meshManager.meshIdsForGeometry(secondGeometry.object->common.id) ==
                         QVector<ObjectId>{secondAssociated.object->common.id} &&
                     meshManager.geometryIdForMesh(orphan.object->common.id) ==
                         std::optional<ObjectId>{InvalidObjectId} &&
                     meshManager.meshIdsForGeometry(InvalidObjectId) ==
                         QVector<ObjectId>{orphan.object->common.id},
                 QStringLiteral("multiple geometries and an orphan mesh retain independent bidirectional mappings"));
    meshManager.removeMesh(firstAssociated.object->common.id);
    meshManager.removeMesh(secondAssociated.object->common.id);
    meshManager.removeMesh(orphan.object->common.id);
    suite.expect(meshManager.meshIdsForGeometry(geometry.object->common.id).isEmpty() &&
                     meshManager.meshIdsForGeometry(secondGeometry.object->common.id).isEmpty() &&
                     geometryManager.removeGeometry(secondGeometry.object->common.id).succeeded(),
                 QStringLiteral("deleting the last mesh removes empty association sets"));

    const auto meshWithChild = meshManager.createMesh(
        request(QStringLiteral("Mesh with child"),
                QStringLiteral("primary.creator"),
                geometry.object->common.id));
    CreateObjectRequest childRequest;
    childRequest.name = QStringLiteral("Mesh child");
    childRequest.type = DataObjectType::Generic;
    childRequest.parentId = meshWithChild.object->common.id;
    const auto child = runtime.createObject(childRequest);
    const auto failedDelete = meshManager.removeMesh(meshWithChild.object->common.id);
    suite.expect(hasCode(failedDelete, QStringLiteral("MODEL-PARENT-HAS-CHILDREN")) &&
                     meshManager.findById(meshWithChild.object->common.id).has_value() &&
                     meshManager.meshIdsForGeometry(geometry.object->common.id).contains(
                         meshWithChild.object->common.id) &&
                     meshManager.geometryIdForMesh(meshWithChild.object->common.id) ==
                         std::optional<ObjectId>{geometry.object->common.id} &&
                     runtime.removeObject(child.object->id).succeeded() &&
                     meshManager.removeMesh(meshWithChild.object->common.id).succeeded() &&
                     meshManager.validateIndexes().succeeded(),
                 QStringLiteral("Runtime delete failure restores both association directions and preserves payload"));

    suite.expect(hasCode(meshManager.removeMesh(generic.object->id),
                         QStringLiteral("MESH-OBJECT-TYPE-MISMATCH")) &&
                     hasCode(meshManager.removeMesh(999999),
                             QStringLiteral("MESH-OBJECT-NOT-FOUND")),
                 QStringLiteral("Mesh typed-remove rejects Generic and missing objects deterministically"));

    suite.expect(geometryManager.removeGeometry(geometry.object->common.id).succeeded() &&
                     meshManager.unregisterCreator(QStringLiteral("primary.creator")).succeeded() &&
                     runtime.removeObject(generic.object->id).succeeded() &&
                     runtime.objectCount() == 0 && runtime.validateIndexes().succeeded() &&
                     geometryManager.validateIndexes().succeeded() &&
                     meshManager.validateIndexes().succeeded(),
                 QStringLiteral("stress cleanup proves creator activity and every ModelData index returned to zero"));

    {
        ApplicationRuntime raceRuntime;
        GeometryManager raceGeometryManager(raceRuntime);
        std::mutex checkpointMutex;
        std::condition_variable checkpointCondition;
        bool blockRemove = false;
        bool checkpointEntered = false;
        bool releaseRemove = false;
        bool failRemove = false;
        MeshManager raceMeshManager(
            raceRuntime,
            raceGeometryManager,
            {},
            [&](ObjectId meshId, ObjectId geometryId) {
                AppMesh::Common::OperationResult result;
                if (blockRemove)
                {
                    std::unique_lock<std::mutex> lock(checkpointMutex);
                    checkpointEntered = true;
                    checkpointCondition.notify_all();
                    checkpointCondition.wait(lock, [&] { return releaseRemove; });
                }
                if (failRemove)
                {
                    result.add({QStringLiteral("test"),
                                QStringLiteral("TEST-MESH-REMOVE-REJECTED"),
                                QStringLiteral("Mesh removal was rejected."),
                                QStringLiteral("Verify association rollback after the delete checkpoint."),
                                true,
                                QStringLiteral("mesh/%1/geometry/%2")
                                    .arg(meshId)
                                    .arg(geometryId),
                                QStringLiteral("mesh.remove.checkpoint"),
                                QString::number(meshId)});
                }
                return result;
            });
        raceMeshManager.registerCreator(
            QStringLiteral("primary.creator"),
            std::make_shared<SuccessfulMeshCreator>(QStringLiteral("primary.creator")));
        CreateGeometryRequest raceGeometryRequest;
        raceGeometryRequest.name = QStringLiteral("Removal race geometry");
        raceGeometryRequest.geometry = validGeometry(QStringLiteral("removal-race"));
        const auto raceGeometry = raceGeometryManager.createGeometry(raceGeometryRequest);

        const auto rollbackMesh = raceMeshManager.createMesh(
            request(QStringLiteral("Rollback delete mesh"),
                    QStringLiteral("primary.creator"),
                    raceGeometry.object->common.id));
        failRemove = true;
        const auto rejectedDelete = raceMeshManager.removeMesh(rollbackMesh.object->common.id);
        failRemove = false;
        suite.expect(hasCode(rejectedDelete, QStringLiteral("TEST-MESH-REMOVE-REJECTED")) &&
                         raceMeshManager.findById(rollbackMesh.object->common.id).has_value() &&
                         raceMeshManager.meshIdsForGeometry(raceGeometry.object->common.id).contains(
                             rollbackMesh.object->common.id) &&
                         raceMeshManager.geometryIdForMesh(rollbackMesh.object->common.id) ==
                             std::optional<ObjectId>{raceGeometry.object->common.id} &&
                         raceMeshManager.validateIndexes().succeeded(),
                     QStringLiteral("injected delete failure restores both indexes and the original payload"));
        raceMeshManager.removeMesh(rollbackMesh.object->common.id);

        const auto racingMesh = raceMeshManager.createMesh(
            request(QStringLiteral("In-flight delete mesh"),
                    QStringLiteral("primary.creator"),
                    raceGeometry.object->common.id));
        blockRemove = true;
        AppMesh::Common::OperationResult meshDeleteResult;
        std::thread remover([&] {
            meshDeleteResult = raceMeshManager.removeMesh(racingMesh.object->common.id);
        });
        {
            std::unique_lock<std::mutex> lock(checkpointMutex);
            checkpointCondition.wait(lock, [&] { return checkpointEntered; });
        }
        const auto geometryDeleteDuringMeshDelete =
            raceGeometryManager.removeGeometry(raceGeometry.object->common.id);
        {
            std::lock_guard<std::mutex> lock(checkpointMutex);
            releaseRemove = true;
        }
        checkpointCondition.notify_all();
        remover.join();
        suite.expect(hasCode(geometryDeleteDuringMeshDelete,
                             QStringLiteral("MESH-GEOMETRY-IN-USE")) &&
                         meshDeleteResult.succeeded() &&
                         raceGeometryManager.removeGeometry(raceGeometry.object->common.id).succeeded() &&
                         raceRuntime.objectCount() == 0 &&
                         raceRuntime.validateIndexes().succeeded() &&
                         raceGeometryManager.validateIndexes().succeeded() &&
                         raceMeshManager.validateIndexes().succeeded(),
                     QStringLiteral("in-flight mesh deletion count deterministically blocks geometry deletion"));
    }

    {
        ApplicationRuntime rollbackRuntime;
        GeometryManager rollbackGeometryManager(rollbackRuntime);
        bool rejectPublish = true;
        MeshManager rollbackManager(
            rollbackRuntime,
            rollbackGeometryManager,
            [&](ObjectId id) {
                AppMesh::Common::OperationResult result;
                if (rejectPublish)
                {
                    result.add({QStringLiteral("test"),
                                QStringLiteral("TEST-MESH-PUBLISH-REJECTED"),
                                QStringLiteral("Mesh publication was rejected."),
                                QStringLiteral("Verify payload, association and reservation rollback."),
                                true,
                                QStringLiteral("object"),
                                QStringLiteral("mesh.create.publish"),
                                QString::number(id)});
                }
                return result;
            });
        rollbackManager.registerCreator(
            QStringLiteral("primary.creator"),
            std::make_shared<SuccessfulMeshCreator>(QStringLiteral("primary.creator")));
        const auto rejectedPublish = rollbackManager.createMesh(
            request(QStringLiteral("Rejected publish"), QStringLiteral("primary.creator")));
        rejectPublish = false;
        const auto afterRollback = rollbackManager.createMesh(
            request(QStringLiteral("Rejected publish"), QStringLiteral("primary.creator")));
        suite.expect(hasCode(rejectedPublish, QStringLiteral("TEST-MESH-PUBLISH-REJECTED")) &&
                         rollbackRuntime.objectCount() == 1 && rollbackManager.objectCount() == 1 &&
                         afterRollback.object->common.name == QStringLiteral("Rejected publish") &&
                         rollbackRuntime.validateIndexes().succeeded() &&
                         rollbackManager.validateIndexes().succeeded(),
                     QStringLiteral("publish rejection cancels reservation and releases its name without reusing ID"));
    }

    return suite.result();
}
