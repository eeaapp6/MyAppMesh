#include "TestSupport.h"

#include "model/ModelData/GeometryManager.h"

#include <QCoreApplication>

#include <atomic>
#include <condition_variable>
#include <limits>
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

CreateObjectRequest generic(const QString& name, ObjectId parentId = InvalidObjectId)
{
    CreateObjectRequest request;
    request.name = name;
    request.type = DataObjectType::Generic;
    request.parentId = parentId;
    return request;
}

GeometryObject validGeometry(const QString& modelKey)
{
    GeometryObject geometry;
    geometry.type = GeometryType::BRep;
    geometry.adapter.providerKey = QStringLiteral("fitk.geometry");
    geometry.adapter.modelKey = modelKey;
    geometry.topology.append(
        {1, TopologyEntityType::Solid, QStringLiteral("solid-1"), {}});
    return geometry;
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
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    ApplicationRuntime runtime;

    std::mutex checkpointMutex;
    std::condition_variable checkpointCondition;
    bool checkpointEntered = false;
    bool releaseCheckpoint = false;
    ObjectId cancelledId = InvalidObjectId;
    GeometryManager manager(
        runtime,
        [&](ObjectId id) {
            std::unique_lock<std::mutex> lock(checkpointMutex);
            cancelledId = id;
            checkpointEntered = true;
            checkpointCondition.notify_all();
            checkpointCondition.wait(lock, [&] { return releaseCheckpoint; });
            AppMesh::Common::OperationResult result;
            result.add({QStringLiteral("test"),
                        QStringLiteral("TEST-CANCEL-RESERVATION"),
                        QStringLiteral("Cancel the hidden reservation."),
                        QStringLiteral("Verify hidden names, ID consumption and cancel cleanup."),
                        true,
                        QStringLiteral("object"),
                        QStringLiteral("geometry.commit"),
                        QString::number(id)});
            return result;
        });

    CreateGeometryRequest reservationRequest;
    reservationRequest.name = QStringLiteral("Reserved Name");
    reservationRequest.geometry = validGeometry(QStringLiteral("reservation-model"));
    CommitGeometryResult rejected;
    std::thread creator([&] { rejected = manager.createGeometry(reservationRequest); });

    {
        std::unique_lock<std::mutex> lock(checkpointMutex);
        checkpointCondition.wait(lock, [&] { return checkpointEntered; });
    }
    const auto publicDuringReservation = runtime.findById(cancelledId);
    const auto nameDuringReservation = runtime.findByName(QStringLiteral("Reserved Name"));
    const auto competing = runtime.createObject(generic(QStringLiteral(" reserved name ")));
    {
        std::lock_guard<std::mutex> lock(checkpointMutex);
        releaseCheckpoint = true;
    }
    checkpointCondition.notify_all();
    creator.join();

    suite.expect(!publicDuringReservation.has_value() && !nameDuringReservation.has_value() &&
                     competing.succeeded() &&
                     competing.object->name == QStringLiteral("reserved name (2)") &&
                     hasCode(rejected, QStringLiteral("TEST-CANCEL-RESERVATION")) &&
                     !runtime.findById(cancelledId).has_value() &&
                     !manager.findById(cancelledId).has_value() &&
                     runtime.validateIndexes().succeeded() && manager.validateIndexes().succeeded(),
                 QStringLiteral("reservation is hidden, claims its name, and cancel removes every hidden index"));

    const auto reusedName = runtime.createObject(generic(QStringLiteral("Reserved Name")));
    const auto afterCancel = runtime.createObject(generic(QStringLiteral("After cancel")));
    suite.expect(reusedName.succeeded() && reusedName.object->name == QStringLiteral("Reserved Name") &&
                     afterCancel.succeeded() && afterCancel.object->id > cancelledId,
                 QStringLiteral("cancel releases the name but never reuses its ObjectId"));

    GeometryManager reserveFailureManager(runtime);
    CreateGeometryRequest emptyName;
    emptyName.name = QStringLiteral(" ");
    emptyName.geometry = validGeometry(QStringLiteral("empty-name"));
    CreateGeometryRequest missingParent;
    missingParent.name = QStringLiteral("Missing parent geometry");
    missingParent.parentId = 999999;
    missingParent.geometry = validGeometry(QStringLiteral("missing-parent"));
    suite.expect(hasCode(reserveFailureManager.createGeometry(emptyName),
                         QStringLiteral("MODEL-NAME-EMPTY")) &&
                     hasCode(reserveFailureManager.createGeometry(missingParent),
                             QStringLiteral("MODEL-PARENT-NOT-FOUND")) &&
                     reserveFailureManager.objectCount() == 0 && runtime.validateIndexes().succeeded(),
                 QStringLiteral("runtime reservation failures do not install geometry payloads"));

    ApplicationRuntime publishFailureRuntime(
        1,
        [](ObjectId id, DataObjectType type) {
            AppMesh::Common::OperationResult result;
            result.add({QStringLiteral("test"),
                        QStringLiteral("TEST-RUNTIME-PUBLISH-FAILED"),
                        QStringLiteral("Runtime publication was rejected."),
                        QStringLiteral("Exercise the manager rollback after publishDomainObject fails."),
                        true,
                        QStringLiteral("object/type/%1").arg(static_cast<int>(type)),
                        QStringLiteral("model.runtime.publish"),
                        QString::number(id)});
            return result;
        });
    GeometryManager publishFailureManager(publishFailureRuntime);
    CreateGeometryRequest publishFailureRequest;
    publishFailureRequest.name = QStringLiteral("Publish failure");
    publishFailureRequest.geometry = validGeometry(QStringLiteral("publish-failure"));
    const auto publishFailure = publishFailureManager.createGeometry(publishFailureRequest);
    suite.expect(hasCode(publishFailure, QStringLiteral("TEST-RUNTIME-PUBLISH-FAILED")) &&
                     publishFailureRuntime.objectCount() == 0 &&
                     publishFailureManager.objectCount() == 0 &&
                     !publishFailureRuntime.findByName(QStringLiteral("Publish failure")).has_value() &&
                     publishFailureRuntime.validateIndexes().succeeded() &&
                     publishFailureManager.validateIndexes().succeeded(),
                 QStringLiteral("Runtime publish failure removes payload and cancels its reservation"));

    constexpr int pairCount = 24;
    QVector<ObjectId> parents;
    QVector<ObjectId> children;
    for (int index = 0; index < pairCount; ++index)
    {
        parents.append(runtime.createObject(
            generic(QStringLiteral("Parent %1").arg(index))).object->id);
        children.append(runtime.createObject(
            generic(QStringLiteral("Child %1").arg(index))).object->id);
    }
    constexpr int threadCount = 4;
    StartGate gate(threadCount);
    std::vector<std::thread> workers;
    std::atomic<bool> operationsSucceeded{true};
    for (int threadIndex = 0; threadIndex < threadCount; ++threadIndex)
    {
        workers.emplace_back([&, threadIndex] {
            gate.arriveAndWait();
            for (int index = threadIndex; index < pairCount; index += threadCount)
            {
                if (!runtime.setParent(children[index], parents[index]).succeeded())
                {
                    operationsSucceeded = false;
                }
                const auto snapshot = runtime.findById(children[index]);
                if (!snapshot.has_value() || snapshot->parentId != parents[index])
                {
                    operationsSucceeded = false;
                }
            }
        });
    }
    gate.open();
    for (auto& worker : workers)
    {
        worker.join();
    }

    bool reverseIndexesMatch = true;
    for (int index = 0; index < pairCount; ++index)
    {
        reverseIndexesMatch = reverseIndexesMatch &&
            runtime.childIds(parents[index]) == QVector<ObjectId>{children[index]};
    }
    const ObjectId oldParent = parents.first();
    const auto invalidParent = runtime.setParent(children.first(), 999999);
    suite.expect(operationsSucceeded && reverseIndexesMatch &&
                     hasCode(invalidParent, QStringLiteral("MODEL-PARENT-NOT-FOUND")) &&
                     runtime.findById(children.first())->parentId == oldParent &&
                     hasCode(runtime.removeObject(oldParent),
                             QStringLiteral("MODEL-PARENT-HAS-CHILDREN")) &&
                     runtime.validateIndexes().succeeded(),
                 QStringLiteral("concurrent independent reparenting preserves both parent indexes and failure atomicity"));

    ApplicationRuntime boundary(std::numeric_limits<ObjectId>::max());
    const auto last = boundary.createObject(generic(QStringLiteral("Maximum")));
    const auto exhausted = boundary.createObject(generic(QStringLiteral("Exhausted")));
    suite.expect(last.succeeded() && last.object->id == std::numeric_limits<ObjectId>::max() &&
                     hasCode(exhausted, QStringLiteral("MODEL-ID-EXHAUSTED")) &&
                     !boundary.findById(InvalidObjectId).has_value() &&
                     boundary.validateIndexes().succeeded(),
                 QStringLiteral("ObjectId zero is invalid and maximum allocation never wraps"));

    return suite.result();
}
