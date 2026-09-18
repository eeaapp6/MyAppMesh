#include "operators/OperatorsModel/FITKTaskExecutor.h"
#include "operators/OperatorsModel/TaskService.h"
#include "TestSupport.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"

#include <QCoreApplication>
#include <QThread>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <future>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

namespace
{
using namespace AppMesh;
using namespace AppMesh::Operators;
using namespace AppMesh::OperatorsModel;

class ControlledExecutor final : public ITaskExecutor
{
public:
    Common::OperationResult submit(TaskWork work) override
    {
        Common::OperationResult result;
        if (reject)
        {
            result.add({QStringLiteral("task"), QStringLiteral("TEST-REJECTED"),
                        QStringLiteral("Controlled rejection"), QStringLiteral("Rejected by test."),
                        false, QStringLiteral("executor"), QStringLiteral("test.executor"), {}, {}});
            return result;
        }
        std::lock_guard<std::mutex> guard(mutex);
        workItems.push_back(std::move(work));
        return result;
    }

    Common::OperationResult waitForDone(int) override
    {
        runAll();
        return {};
    }

    void drain() noexcept override { runAll(); }

    void runAll()
    {
        std::vector<TaskWork> pending;
        {
            std::lock_guard<std::mutex> guard(mutex);
            pending.swap(workItems);
        }
        for (auto& work : pending)
        {
            work();
        }
    }

    bool reject = false;
    std::mutex mutex;
    std::vector<TaskWork> workItems;
};

class ValueOperator final : public IOperator
{
public:
    QString key() const override { return QStringLiteral("test.value"); }
    Common::OperationResult validate(const OperatorInput& input) const override
    {
        Common::OperationResult result;
        if (input.operationKey != key())
        {
            result.add({QStringLiteral("validation"), QStringLiteral("TEST-KEY"),
                        QStringLiteral("Invalid key"), QStringLiteral("Use test.value."),
                        false, QStringLiteral("operationKey"), QStringLiteral("test.validate"), {}, {}});
        }
        return result;
    }
    OperatorResult execute(const OperatorInput& input) override
    {
        if (input.parameters.value(QStringLiteral("fail")).toBool())
        {
            ErrorInfo error{QStringLiteral("test"), QStringLiteral("TEST-FAIL"),
                            QStringLiteral("Controlled failure"), QStringLiteral("Requested by test."),
                            false, InvalidTaskId, Model::InvalidObjectId, QStringLiteral("execute")};
            return std::move(*OperatorResult::failure(error));
        }
        executedThread = QThread::currentThreadId();
        ResultReference result;
        result.value = input.parameters.value(QStringLiteral("value"));
        return OperatorResult::success(result);
    }
    std::atomic<Qt::HANDLE> executedThread{nullptr};
};

struct BlockingState
{
    void waitUntilStarted()
    {
        std::unique_lock<std::mutex> lock(mutex);
        condition.wait(lock, [this] { return started; });
    }
    void release()
    {
        std::lock_guard<std::mutex> guard(mutex);
        released = true;
        condition.notify_all();
    }
    std::mutex mutex;
    std::condition_variable condition;
    bool started = false;
    bool released = false;
};

class BlockingOperator final : public IOperator
{
public:
    explicit BlockingOperator(BlockingState& state, std::atomic<int>& touches)
        : m_state(state), m_touches(touches) {}
    QString key() const override { return QStringLiteral("test.blocking"); }
    Common::OperationResult validate(const OperatorInput&) const override { return {}; }
    OperatorResult execute(const OperatorInput&) override
    {
        std::unique_lock<std::mutex> lock(m_state.mutex);
        m_state.started = true;
        m_state.condition.notify_all();
        m_state.condition.wait(lock, [this] { return m_state.released; });
        ++m_touches;
        return OperatorResult::success({});
    }
private:
    BlockingState& m_state;
    std::atomic<int>& m_touches;
};

class AsyncGateExecutor final : public ITaskExecutor
{
public:
    ~AsyncGateExecutor() override { drain(); }
    Common::OperationResult submit(TaskWork work) override
    {
        Common::OperationResult result;
        std::lock_guard<std::mutex> guard(mutex);
        if (!accepting)
        {
            result.add({QStringLiteral("task"), QStringLiteral("TEST-STOPPED"),
                        QStringLiteral("Stopped"), QStringLiteral("No new work."),
                        false, {}, QStringLiteral("test.executor"), {}, {}});
            return result;
        }
        ++pending;
        workers.emplace_back([this, work = std::move(work)]() mutable {
            work();
            std::lock_guard<std::mutex> guard(mutex);
            --pending;
            condition.notify_all();
        });
        return result;
    }
    Common::OperationResult waitForDone(int timeoutMs) override
    {
        Common::OperationResult result;
        std::unique_lock<std::mutex> lock(mutex);
        if (!condition.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                                [this] { return pending == 0; }))
            result.add({QStringLiteral("task"), QStringLiteral("TASK-EXECUTOR-WAIT-TIMEOUT"),
                        QStringLiteral("Timed out"), QStringLiteral("Work remains pending."),
                        false, {}, QStringLiteral("test.executor"), {}, {}});
        return result;
    }
    void drain() noexcept override
    {
        std::vector<std::thread> joined;
        {
            std::unique_lock<std::mutex> lock(mutex);
            accepting = false;
            condition.wait(lock, [this] { return pending == 0; });
            joined.swap(workers);
        }
        for (auto& worker : joined) if (worker.joinable()) worker.join();
    }
private:
    std::mutex mutex;
    std::condition_variable condition;
    std::vector<std::thread> workers;
    int pending = 0;
    bool accepting = true;
};

class UnrelatedFITKTask final : public Core::FITKThreadTask
{
public:
    UnrelatedFITKTask(BlockingState& state, std::atomic<bool>& finished)
        : m_state(state), m_finished(finished) {}
    void run() override
    {
        std::unique_lock<std::mutex> lock(m_state.mutex);
        m_state.started = true;
        m_state.condition.notify_all();
        m_state.condition.wait(lock, [this] { return m_state.released; });
        m_finished = true;
        m_state.condition.notify_all();
    }
private:
    BlockingState& m_state;
    std::atomic<bool>& m_finished;
};

OperatorInput input(bool fail = false)
{
    OperatorInput value;
    value.operationKey = QStringLiteral("test.value");
    value.parameters.insert(QStringLiteral("value"), 17);
    value.parameters.insert(QStringLiteral("fail"), fail);
    return value;
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;

    auto executor = std::make_shared<ControlledExecutor>();
    TaskService service(executor);
    auto operation = std::make_shared<ValueOperator>();
    QVector<TaskEvent> events;
    auto throwing = service.subscribe([](const TaskEvent&) { throw std::runtime_error("observer"); });
    auto subscription = service.subscribe([&events](const TaskEvent& event) { events.append(event); });

    auto submitted = service.submit(operation, input());
    suite.expect(submitted.succeeded() && submitted.taskId.has_value() &&
                     service.findById(*submitted.taskId)->status == TaskStatus::Executing &&
                     events.size() == 1 && events.front().type == TaskEventType::Started,
                 QStringLiteral("submission exposes Executing and isolates throwing observers"));
    Common::Diagnostic progressDiagnostic{
        QStringLiteral("test"), QStringLiteral("TEST-DIAGNOSTIC"),
        QStringLiteral("Controlled diagnostic"), QStringLiteral("Execution continues."),
        true, QStringLiteral("work"), QStringLiteral("test.execute"), {}, {}};
    suite.expect(service.updateProgress(*submitted.taskId, 25, QStringLiteral("reading")).succeeded() &&
                     service.reportDiagnostic(*submitted.taskId, progressDiagnostic).succeeded() &&
                     service.findById(*submitted.taskId)->progress == 25,
                 QStringLiteral("progress and diagnostics use the frozen sequenced TaskEvent types"));
    executor->runAll();
    const auto successful = service.findById(*submitted.taskId);
    suite.expect(successful && successful->status == TaskStatus::Succeeded &&
                     successful->result && events.size() == 4 &&
                     events.back().type == TaskEventType::Succeeded &&
                     events.front().sequence < events.back().sequence,
                 QStringLiteral("successful work publishes one ordered terminal event"));

    auto failedSubmit = service.submit(operation, input(true));
    executor->runAll();
    const auto failed = service.findById(*failedSubmit.taskId);
    int failedTerminals = 0;
    for (const auto& event : events)
    {
        if (event.taskId == *failedSubmit.taskId && event.type == TaskEventType::Failed)
        {
            ++failedTerminals;
        }
    }
    suite.expect(failed && failed->status == TaskStatus::Failed && failed->error &&
                     failedTerminals == 1,
                 QStringLiteral("operator failure produces exactly one Failed terminal"));
    suite.expect(!service.updateProgress(*submitted.taskId, 50).succeeded() &&
                     !service.reportDiagnostic(*submitted.taskId, progressDiagnostic).succeeded(),
                 QStringLiteral("terminal tasks reject later progress and business diagnostics"));

    OperatorInput invalid;
    invalid.operationKey = QStringLiteral("wrong");
    const auto preflight = service.submit(operation, invalid);
    suite.expect(!preflight.succeeded() && !preflight.taskId && service.snapshots().size() == 2,
                 QStringLiteral("pre-submission validation failure creates no Task"));

    auto rejectingExecutor = std::make_shared<ControlledExecutor>();
    rejectingExecutor->reject = true;
    TaskService rejectingService(rejectingExecutor);
    const auto rejected = rejectingService.submit(operation, input());
    suite.expect(rejected.taskId.has_value() && !rejected.succeeded() &&
                     rejectingService.findById(*rejected.taskId)->status == TaskStatus::Failed,
                 QStringLiteral("executor rejection converts an allocated Task to Failed"));

    const int eventCountBeforeReset = events.size();
    subscription.reset();
    const auto afterReset = service.submit(operation, input());
    executor->runAll();
    suite.expect(events.size() == eventCountBeforeReset &&
                     service.findById(*afterReset.taskId)->status == TaskStatus::Succeeded,
                 QStringLiteral("unsubscribed observers receive no later callbacks"));

    suite.expect(service.stop(1000).succeeded() && !service.isAccepting() &&
                     !service.submit(operation, input()).taskId.has_value() &&
                     !service.findById(999999).has_value(),
                 QStringLiteral("stop drains work, rejects new submissions, and unknown IDs are empty"));

    auto destructorExecutor = std::make_shared<ControlledExecutor>();
    auto destructorOperation = std::make_shared<ValueOperator>();
    {
        TaskService destructingService(destructorExecutor);
        const auto pending = destructingService.submit(destructorOperation, input());
        suite.expect(pending.taskId.has_value() &&
                         destructingService.findById(*pending.taskId)->status ==
                             TaskStatus::Executing,
                     QStringLiteral("destructor fixture has one accepted in-flight task"));
    }
    suite.expect(destructorOperation->executedThread.load() != nullptr &&
                     destructorExecutor->workItems.empty(),
                 QStringLiteral("TaskService destruction drains accepted work without detaching it"));

    auto fitkExecutor = std::make_shared<FITKTaskExecutor>();
    TaskService fitkService(fitkExecutor);
    auto fitkOperator = std::make_shared<ValueOperator>();
    const auto guiThread = QThread::currentThreadId();
    QVector<TaskId> fitkIds;
    for (int i = 0; i < 8; ++i)
    {
        const auto item = fitkService.submit(fitkOperator, input());
        if (item.taskId)
        {
            fitkIds.append(*item.taskId);
        }
    }
    const auto drained = fitkService.stop(5000);
    bool allSucceeded = fitkIds.size() == 8;
    for (const auto id : fitkIds)
    {
        const auto snapshot = fitkService.findById(id);
        allSucceeded = allSucceeded && snapshot && snapshot->status == TaskStatus::Succeeded;
    }
    suite.expect(drained.succeeded() && allSucceeded &&
                     fitkOperator->executedThread.load() != guiThread,
                 QStringLiteral("FITKTaskExecutor runs concurrent work off the submitting thread and drains"));

    BlockingState boundedState;
    std::atomic<int> boundedTouches{0};
    auto asyncExecutor = std::make_shared<AsyncGateExecutor>();
    TaskService boundedService(asyncExecutor);
    auto blockingOperation = std::make_shared<BlockingOperator>(boundedState, boundedTouches);
    OperatorInput blockingInput;
    blockingInput.operationKey = blockingOperation->key();
    QVector<TaskEvent> blockingEvents;
    auto blockingSubscription = boundedService.subscribe(
        [&blockingEvents](const TaskEvent& event) { blockingEvents.append(event); });
    const auto blockingSubmission = boundedService.submit(blockingOperation, blockingInput);
    boundedState.waitUntilStarted();
    const auto timedOut = boundedService.stop(1);
    suite.expect(!timedOut.succeeded() && !boundedService.isAccepting() &&
                     boundedService.findById(*blockingSubmission.taskId)->status == TaskStatus::Executing &&
                     !boundedService.submit(blockingOperation, blockingInput).taskId,
                 QStringLiteral("bounded stop diagnoses timeout without inventing a terminal state"));
    auto finalDrain = std::async(std::launch::async, [&boundedService] { boundedService.drain(); });
    suite.expect(finalDrain.wait_for(std::chrono::milliseconds(30)) == std::future_status::timeout,
                 QStringLiteral("final drain cannot return while accepted work is blocked"));
    boundedState.release();
    suite.expect(finalDrain.wait_for(std::chrono::seconds(2)) == std::future_status::ready,
                 QStringLiteral("final drain returns after accepted work finishes"));
    boundedService.drain();
    int startedCount = 0;
    int terminalCount = 0;
    for (const auto& event : blockingEvents)
    {
        if (event.type == TaskEventType::Started) ++startedCount;
        if (event.type == TaskEventType::Succeeded || event.type == TaskEventType::Failed) ++terminalCount;
    }
    suite.expect(boundedTouches == 1 && startedCount == 1 && terminalCount == 1,
                 QStringLiteral("repeated stop and drain preserve exactly one terminal event"));

    BlockingState destructorState;
    std::atomic<int> dependencyTouches{0};
    auto destructorAsyncExecutor = std::make_shared<AsyncGateExecutor>();
    auto destroyingService = std::make_unique<TaskService>(destructorAsyncExecutor);
    auto dependencyOperator = std::make_shared<BlockingOperator>(destructorState, dependencyTouches);
    const auto dependencyTask = destroyingService->submit(dependencyOperator, blockingInput);
    destructorState.waitUntilStarted();
    auto destruction = std::async(std::launch::async,
        [service = std::move(destroyingService)]() mutable { service.reset(); });
    suite.expect(destruction.wait_for(std::chrono::milliseconds(30)) == std::future_status::timeout,
                 QStringLiteral("TaskService destruction waits instead of crossing a live dependency"));
    destructorState.release();
    suite.expect(destruction.wait_for(std::chrono::seconds(2)) == std::future_status::ready &&
                     dependencyTouches == 1 && dependencyTask.taskId.has_value(),
                 QStringLiteral("dependency is touched before final TaskService destruction returns"));

    FITKTaskExecutor directExecutor;
    BlockingState fitkGate;
    std::atomic<int> fitkStarted{0};
    std::atomic<int> fitkFinished{0};
    for (int i = 0; i < 3; ++i)
    {
        suite.expect(directExecutor.submit([&] {
            {
                std::lock_guard<std::mutex> guard(fitkGate.mutex);
                ++fitkStarted;
                fitkGate.started = fitkStarted == 3;
                fitkGate.condition.notify_all();
            }
            std::unique_lock<std::mutex> lock(fitkGate.mutex);
            fitkGate.condition.wait(lock, [&fitkGate] { return fitkGate.released; });
            ++fitkFinished;
        }).succeeded(), QStringLiteral("direct FITK task accepted"));
    }
    fitkGate.waitUntilStarted();
    suite.expect(!directExecutor.waitForDone(1).succeeded(),
                 QStringLiteral("FITK bounded wait reports pending work"));
    auto fitkDrain = std::async(std::launch::async, [&directExecutor] { directExecutor.drain(); });
    suite.expect(fitkDrain.wait_for(std::chrono::milliseconds(30)) == std::future_status::timeout,
                 QStringLiteral("FITK final drain waits for all owned work"));
    fitkGate.release();
    suite.expect(fitkDrain.wait_for(std::chrono::seconds(2)) == std::future_status::ready &&
                     fitkFinished == 3 && directExecutor.waitForDone(0).succeeded() &&
                     !directExecutor.submit([] {}).succeeded(),
                 QStringLiteral("FITK drain leaves no pending count and rejects later work"));

    BlockingState unrelatedState;
    std::atomic<bool> unrelatedFinished{false};
    Core::FITKThreadPool::getInstance()->execTask(
        new UnrelatedFITKTask(unrelatedState, unrelatedFinished));
    unrelatedState.waitUntilStarted();
    FITKTaskExecutor isolatedExecutor;
    std::atomic<bool> ownedFinished{false};
    suite.expect(isolatedExecutor.submit([&ownedFinished] { ownedFinished = true; }).succeeded(),
                 QStringLiteral("isolated FITK executor accepts owned work"));
    auto isolatedDrain = std::async(std::launch::async,
        [&isolatedExecutor] { isolatedExecutor.drain(); });
    suite.expect(isolatedDrain.wait_for(std::chrono::seconds(2)) == std::future_status::ready &&
                     ownedFinished && !unrelatedFinished,
                 QStringLiteral("FITK drain does not wait for unrelated global-pool work"));
    unrelatedState.release();
    {
        std::unique_lock<std::mutex> lock(unrelatedState.mutex);
        unrelatedState.condition.wait_for(lock, std::chrono::seconds(2),
                                          [&unrelatedFinished] { return unrelatedFinished.load(); });
    }
    suite.expect(unrelatedFinished,
                 QStringLiteral("unrelated FITK fixture is released and joined by the global pool"));

    return suite.result();
}
