#include "app/ApplicationBoundaries.h"
#include "operators/OperatorsModel/TaskService.h"
#include "TestSupport.h"

#include <QCoreApplication>

#include <chrono>
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

namespace
{
using namespace AppMesh;
using namespace AppMesh::Operators;
using namespace AppMesh::OperatorsModel;

class JoiningExecutor final : public ITaskExecutor
{
public:
    ~JoiningExecutor() override { drain(); }
    Common::OperationResult submit(TaskWork work) override
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        ++m_pending;
        m_workers.emplace_back([this, work = std::move(work)]() mutable {
            work();
            std::lock_guard<std::mutex> done(m_mutex);
            --m_pending;
            m_condition.notify_all();
        });
        return {};
    }
    Common::OperationResult waitForDone(int timeoutMs) override
    {
        Common::OperationResult result;
        std::unique_lock<std::mutex> lock(m_mutex);
        if (!m_condition.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                                  [this] { return m_pending == 0; }))
            result.add({QStringLiteral("task"), QStringLiteral("TASK-EXECUTOR-WAIT-TIMEOUT"),
                        QStringLiteral("Timed out"), QStringLiteral("Pending test work."),
                        false, {}, QStringLiteral("test.lifecycle"), {}, {}});
        return result;
    }
    void drain() noexcept override
    {
        std::vector<std::thread> workers;
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_condition.wait(lock, [this] { return m_pending == 0; });
            workers.swap(m_workers);
        }
        for (auto& worker : workers) if (worker.joinable()) worker.join();
    }
private:
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::vector<std::thread> m_workers;
    int m_pending = 0;
};

class GateOperator final : public IOperator
{
public:
    QString key() const override { return QStringLiteral("test.lifecycle"); }
    Common::OperationResult validate(const OperatorInput&) const override { return {}; }
    OperatorResult execute(const OperatorInput&) override
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_started = true;
        m_condition.notify_all();
        m_condition.wait(lock, [this] { return m_released; });
        return OperatorResult::success({});
    }
    void waitStarted()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this] { return m_started; });
    }
    void release()
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_released = true;
        m_condition.notify_all();
    }
private:
    std::mutex m_mutex;
    std::condition_variable m_condition;
    bool m_started = false;
    bool m_released = false;
};
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    auto executor = std::make_shared<JoiningExecutor>();
    TaskService service(executor);
    App::TaskServiceOperatorBoundary boundary([&service] { return &service; });
    suite.expect(boundary.initialize().succeeded(),
                 QStringLiteral("production operator boundary binds the real TaskService"));
    auto operation = std::make_shared<GateOperator>();
    OperatorInput input;
    input.operationKey = operation->key();
    const auto submitted = service.submit(operation, input);
    operation->waitStarted();
    suite.expect(boundary.stopAccepting().succeeded() && !service.isAccepting() &&
                     !service.submit(operation, input).taskId,
                 QStringLiteral("MeshApp operations.stop rejects new production work"));
    auto finishing = std::async(std::launch::async, [&boundary] { return boundary.finishCurrent(); });
    suite.expect(finishing.wait_for(std::chrono::milliseconds(30)) == std::future_status::timeout,
                 QStringLiteral("MeshApp tasks.finish waits for accepted work"));
    operation->release();
    suite.expect(finishing.wait_for(std::chrono::seconds(2)) == std::future_status::ready &&
                     finishing.get().succeeded() && submitted.taskId.has_value(),
                 QStringLiteral("MeshApp tasks.finish returns only after completion"));
    suite.expect(boundary.shutdown().succeeded() && boundary.shutdown().succeeded(),
                 QStringLiteral("production task boundary shutdown is idempotent"));
    return suite.result();
}
