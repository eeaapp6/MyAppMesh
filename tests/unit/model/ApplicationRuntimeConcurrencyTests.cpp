#include "TestSupport.h"

#include "model/ModelData/ApplicationRuntime.h"

#include <QCoreApplication>

#include <condition_variable>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

namespace
{
using namespace AppMesh::Model;

class StartGate
{
public:
    explicit StartGate(int participants) : m_participants(participants) {}

    void arriveAndWait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        ++m_ready;
        if (m_ready == m_participants)
        {
            m_condition.notify_all();
        }
        m_condition.wait(lock, [this] { return m_start; });
    }

    void open()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this] { return m_ready == m_participants; });
        m_start = true;
        lock.unlock();
        m_condition.notify_all();
    }

private:
    const int m_participants;
    int m_ready = 0;
    bool m_start = false;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};

CreateObjectRequest request(const QString& name)
{
    CreateObjectRequest value;
    value.name = name;
    value.type = DataObjectType::Generic;
    return value;
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    ApplicationRuntime runtime;

    constexpr int threadCount = 8;
    constexpr int objectsPerThread = 100;
    StartGate createGate(threadCount);
    std::vector<std::thread> workers;
    for (int threadIndex = 0; threadIndex < threadCount; ++threadIndex)
    {
        workers.emplace_back([&runtime, &createGate, objectsPerThread] {
            createGate.arriveAndWait();
            for (int index = 0; index < objectsPerThread; ++index)
            {
                runtime.createObject(request(QStringLiteral("Shared")));
            }
        });
    }
    createGate.open();
    for (auto& worker : workers)
    {
        worker.join();
    }

    const auto created = runtime.snapshots();
    std::set<ObjectId> ids;
    std::set<QString> names;
    for (const auto& object : created)
    {
        ids.insert(object.id);
        names.insert(object.name.toCaseFolded());
    }
    suite.expect(created.size() == threadCount * objectsPerThread &&
                     ids.size() == static_cast<std::size_t>(created.size()) &&
                     names.size() == static_cast<std::size_t>(created.size()),
                 QStringLiteral("concurrent creates preserve unique IDs and names"));

    StartGate mixedGate(threadCount);
    workers.clear();
    for (int threadIndex = 0; threadIndex < threadCount; ++threadIndex)
    {
        workers.emplace_back([&runtime, &created, &mixedGate, threadIndex, threadCount] {
            mixedGate.arriveAndWait();
            for (int index = threadIndex; index < created.size(); index += threadCount)
            {
                const ObjectId id = created[index].id;
                runtime.findById(id);
                runtime.renameObject(id, QStringLiteral("Renamed %1").arg(id));
                runtime.findByName(QStringLiteral("Renamed %1").arg(id));
            }
        });
    }
    mixedGate.open();
    for (auto& worker : workers)
    {
        worker.join();
    }
    suite.expect(runtime.validateIndexes().succeeded(),
                 QStringLiteral("concurrent query and rename preserve index consistency"));

    const auto renamed = runtime.snapshots();
    StartGate deleteGate(threadCount);
    workers.clear();
    for (int threadIndex = 0; threadIndex < threadCount; ++threadIndex)
    {
        workers.emplace_back([&runtime, &renamed, &deleteGate, threadIndex, threadCount] {
            deleteGate.arriveAndWait();
            for (int index = threadIndex; index < renamed.size(); index += threadCount)
            {
                runtime.removeObject(renamed[index].id);
            }
        });
    }
    deleteGate.open();
    for (auto& worker : workers)
    {
        worker.join();
    }
    suite.expect(runtime.objectCount() == 0 && runtime.validateIndexes().succeeded(),
                 QStringLiteral("concurrent leaf deletion clears every index"));

    return suite.result();
}
