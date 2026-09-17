#include "operators/OperatorsModel/FITKTaskExecutor.h"
#include "operators/OperatorsModel/TaskService.h"
#include "TestSupport.h"

#include <QCoreApplication>
#include <QThread>

#include <atomic>
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

    return suite.result();
}
