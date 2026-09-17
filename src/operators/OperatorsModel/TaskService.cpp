#include "TaskService.h"

#include <algorithm>
#include <condition_variable>
#include <map>
#include <mutex>
#include <utility>
#include <vector>

namespace AppMesh::OperatorsModel
{
struct TaskServiceState
{
    TaskServiceState(std::shared_ptr<ITaskExecutor> taskExecutor,
                     Operators::TaskId firstTaskId)
        : executor(std::move(taskExecutor)), allocator(firstTaskId)
    {
    }

    std::shared_ptr<ITaskExecutor> executor;
    Operators::TaskIdAllocator allocator;
    std::mutex submissionMutex;
    mutable std::mutex taskMutex;
    std::map<Operators::TaskId, std::shared_ptr<Operators::Task>> tasks;
    bool accepting = true;

    mutable std::mutex observerMutex;
    std::recursive_mutex deliveryMutex;
    std::map<quint64, Operators::TaskEventObserver> observers;
    quint64 nextObserverId = 1;
    bool sourceAlive = true;
};

namespace
{
struct ExecutionGate
{
    void wait()
    {
        std::unique_lock<std::mutex> lock(mutex);
        condition.wait(lock, [this] { return open; });
    }

    void release()
    {
        std::lock_guard<std::mutex> guard(mutex);
        open = true;
        condition.notify_all();
    }

    std::mutex mutex;
    std::condition_variable condition;
    bool open = false;
};

Common::Diagnostic serviceDiagnostic(const QString& code,
                                     const QString& message,
                                     const QString& detail,
                                     Operators::TaskId taskId = Operators::InvalidTaskId)
{
    return {QStringLiteral("task"),
            code,
            message,
            detail,
            false,
            QStringLiteral("task"),
            QStringLiteral("operator.task-service"),
            QString(),
            taskId == Operators::InvalidTaskId ? QString() : QString::number(taskId)};
}

void append(Common::OperationResult& target, const Common::OperationResult& source)
{
    for (const auto& diagnostic : source.diagnostics)
    {
        target.add(diagnostic);
    }
}

void dispatch(const std::shared_ptr<TaskServiceState>& state,
              const Operators::TaskEvent& event)
{
    std::lock_guard<std::recursive_mutex> delivery(state->deliveryMutex);
    std::vector<Operators::TaskEventObserver> observers;
    {
        std::lock_guard<std::mutex> guard(state->observerMutex);
        if (!state->sourceAlive)
        {
            return;
        }
        observers.reserve(state->observers.size());
        for (const auto& item : state->observers)
        {
            observers.push_back(item.second);
        }
    }
    for (const auto& observer : observers)
    {
        try
        {
            observer(event);
        }
        catch (...)
        {
            // Observer failures are isolated from the authoritative task state.
        }
    }
}

Operators::ErrorInfo executorFailure(Operators::TaskId id,
                                     const Common::OperationResult& submitResult)
{
    if (!submitResult.diagnostics.isEmpty())
    {
        return Operators::errorInfoFromDiagnostic(submitResult.diagnostics.front(), id);
    }
    return {QStringLiteral("task"),
            QStringLiteral("TASK-EXECUTOR-SUBMIT-FAILED"),
            QStringLiteral("The task executor rejected the task."),
            QStringLiteral("No executor diagnostic was provided."),
            false,
            id,
            Model::InvalidObjectId,
            QStringLiteral("executor")};
}

void finishTask(const std::shared_ptr<TaskServiceState>& state,
                const std::shared_ptr<Operators::Task>& task,
                const std::shared_ptr<Operators::IOperator>& operation,
                Operators::OperatorInput input)
{
    const Operators::TaskId taskId = task->snapshot().id;
    Operators::OperatorResult outcome = Operators::executeSafely(*operation, input, taskId);
    for (auto diagnostic : outcome.diagnostics())
    {
        if (diagnostic.taskId.isEmpty())
        {
            diagnostic.taskId = QString::number(taskId);
        }
        const auto update = task->reportDiagnostic(diagnostic);
        if (update.event)
        {
            dispatch(state, *update.event);
        }
    }

    Operators::TaskUpdateResult terminal;
    if (outcome.succeeded())
    {
        terminal = task->succeed(outcome.result());
    }
    else if (outcome.error())
    {
        terminal = task->fail(*outcome.error());
    }
    else
    {
        const Operators::ErrorInfo error{
            QStringLiteral("task"),
            QStringLiteral("TASK-OPERATOR-RESULT-INVALID"),
            QStringLiteral("The operator returned an invalid failure result."),
            QStringLiteral("A failed result requires ErrorInfo."),
            false,
            taskId,
            Model::InvalidObjectId,
            QStringLiteral("result")};
        terminal = task->fail(error);
    }
    if (terminal.event)
    {
        dispatch(state, *terminal.event);
    }
}

void closeObservers(const std::shared_ptr<TaskServiceState>& state) noexcept
{
    std::lock_guard<std::recursive_mutex> delivery(state->deliveryMutex);
    std::lock_guard<std::mutex> guard(state->observerMutex);
    state->sourceAlive = false;
    state->observers.clear();
}
}

TaskEventSubscription::TaskEventSubscription(std::weak_ptr<TaskServiceState> state,
                                             quint64 id) noexcept
    : m_state(std::move(state)), m_id(id)
{
}

TaskEventSubscription::TaskEventSubscription(TaskEventSubscription&& other) noexcept
    : m_state(std::move(other.m_state)), m_id(other.m_id)
{
    other.m_id = 0;
}

TaskEventSubscription& TaskEventSubscription::operator=(
    TaskEventSubscription&& other) noexcept
{
    if (this != &other)
    {
        reset();
        m_state = std::move(other.m_state);
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

TaskEventSubscription::~TaskEventSubscription()
{
    reset();
}

void TaskEventSubscription::reset() noexcept
{
    if (m_id != 0)
    {
        if (const auto state = m_state.lock())
        {
            std::lock_guard<std::recursive_mutex> delivery(state->deliveryMutex);
            std::lock_guard<std::mutex> guard(state->observerMutex);
            state->observers.erase(m_id);
        }
    }
    m_state.reset();
    m_id = 0;
}

bool TaskEventSubscription::isActive() const noexcept
{
    const auto state = m_state.lock();
    if (!state || m_id == 0)
    {
        return false;
    }
    std::lock_guard<std::mutex> guard(state->observerMutex);
    return state->sourceAlive && state->observers.find(m_id) != state->observers.end();
}

TaskService::TaskService(std::shared_ptr<ITaskExecutor> executor,
                         Operators::TaskId firstTaskId)
    : m_state(std::make_shared<TaskServiceState>(std::move(executor), firstTaskId))
{
}

TaskService::~TaskService()
{
    if (m_state)
    {
        stop(5000);
        closeObservers(m_state);
    }
}

TaskSubmissionResult TaskService::submit(
    const std::shared_ptr<Operators::IOperator>& operation,
    const Operators::OperatorInput& input)
{
    TaskSubmissionResult result;
    if (!operation)
    {
        result.add(serviceDiagnostic(QStringLiteral("TASK-OPERATOR-MISSING"),
                                     QStringLiteral("No operator was supplied."),
                                     QStringLiteral("Submit a shared operator instance.")));
        return result;
    }
    if (!m_state->executor)
    {
        result.add(serviceDiagnostic(QStringLiteral("TASK-EXECUTOR-MISSING"),
                                     QStringLiteral("No task executor is available."),
                                     QStringLiteral("Construct TaskService with an executor.")));
        return result;
    }
    const auto validation = operation->validate(input);
    append(result, validation);
    if (!result.succeeded())
    {
        return result;
    }

    std::shared_ptr<Operators::Task> task;
    Operators::TaskUpdateResult started;
    auto gate = std::make_shared<ExecutionGate>();
    Common::OperationResult submitted;
    {
        std::lock_guard<std::mutex> submissionGuard(m_state->submissionMutex);
        {
            std::lock_guard<std::mutex> guard(m_state->taskMutex);
            if (!m_state->accepting)
            {
                result.add(serviceDiagnostic(QStringLiteral("TASK-SERVICE-STOPPED"),
                                             QStringLiteral("The task service is stopped."),
                                             QStringLiteral("New tasks are not accepted after stop.")));
                return result;
            }
        }

        const auto allocation = m_state->allocator.allocate();
        append(result, allocation);
        if (!allocation.id)
        {
            return result;
        }
        const Operators::TaskId taskId = *allocation.id;
        auto created = Operators::Task::create(taskId, input);
        append(result, created);
        if (!created.task)
        {
            return result;
        }
        task = std::shared_ptr<Operators::Task>(std::move(created.task));
        {
            std::lock_guard<std::mutex> guard(m_state->taskMutex);
            m_state->tasks.emplace(taskId, task);
        }
        result.taskId = taskId;
        started = task->start();
        append(result, started);

        auto state = m_state;
        submitted = m_state->executor->submit(
            [gate, state, task, operation, input] {
                gate->wait();
                finishTask(state, task, operation, input);
            });
    }
    if (started.event)
    {
        dispatch(m_state, *started.event);
    }
    gate->release();
    if (!submitted.succeeded())
    {
        append(result, submitted);
        const auto failed = task->fail(executorFailure(*result.taskId, submitted));
        if (failed.event)
        {
            dispatch(m_state, *failed.event);
        }
    }
    return result;
}

std::optional<Operators::TaskSnapshot> TaskService::findById(
    Operators::TaskId id) const
{
    std::shared_ptr<Operators::Task> task;
    {
        std::lock_guard<std::mutex> guard(m_state->taskMutex);
        const auto found = m_state->tasks.find(id);
        if (found == m_state->tasks.end())
        {
            return std::nullopt;
        }
        task = found->second;
    }
    return task->snapshot();
}

QVector<Operators::TaskSnapshot> TaskService::snapshots() const
{
    std::vector<std::shared_ptr<Operators::Task>> tasks;
    {
        std::lock_guard<std::mutex> guard(m_state->taskMutex);
        tasks.reserve(m_state->tasks.size());
        for (const auto& item : m_state->tasks)
        {
            tasks.push_back(item.second);
        }
    }
    QVector<Operators::TaskSnapshot> values;
    values.reserve(static_cast<int>(tasks.size()));
    for (const auto& task : tasks)
    {
        values.append(task->snapshot());
    }
    return values;
}

Common::OperationResult TaskService::updateProgress(Operators::TaskId id,
                                                    int progress,
                                                    const QString& phase)
{
    Common::OperationResult result;
    std::shared_ptr<Operators::Task> task;
    {
        std::lock_guard<std::mutex> guard(m_state->taskMutex);
        const auto found = m_state->tasks.find(id);
        if (found == m_state->tasks.end())
        {
            result.add(serviceDiagnostic(QStringLiteral("TASK-NOT-FOUND"),
                                         QStringLiteral("The task does not exist."),
                                         QStringLiteral("Progress was not published."), id));
            return result;
        }
        task = found->second;
    }
    const auto updated = task->updateProgress(progress, phase);
    append(result, updated);
    if (updated.event)
    {
        dispatch(m_state, *updated.event);
    }
    return result;
}

Common::OperationResult TaskService::reportDiagnostic(
    Operators::TaskId id,
    const Common::Diagnostic& diagnostic)
{
    Common::OperationResult result;
    std::shared_ptr<Operators::Task> task;
    {
        std::lock_guard<std::mutex> guard(m_state->taskMutex);
        const auto found = m_state->tasks.find(id);
        if (found == m_state->tasks.end())
        {
            result.add(serviceDiagnostic(QStringLiteral("TASK-NOT-FOUND"),
                                         QStringLiteral("The task does not exist."),
                                         QStringLiteral("The diagnostic was not published."), id));
            return result;
        }
        task = found->second;
    }
    const auto updated = task->reportDiagnostic(diagnostic);
    append(result, updated);
    if (updated.event)
    {
        dispatch(m_state, *updated.event);
    }
    return result;
}

TaskEventSubscription TaskService::subscribe(Operators::TaskEventObserver observer)
{
    if (!observer)
    {
        return {};
    }
    std::lock_guard<std::mutex> guard(m_state->observerMutex);
    if (!m_state->sourceAlive || m_state->nextObserverId == 0)
    {
        return {};
    }
    const quint64 id = m_state->nextObserverId++;
    m_state->observers.emplace(id, std::move(observer));
    return TaskEventSubscription(m_state, id);
}

Common::OperationResult TaskService::stop(int timeoutMs)
{
    {
        std::lock_guard<std::mutex> submissionGuard(m_state->submissionMutex);
        std::lock_guard<std::mutex> guard(m_state->taskMutex);
        m_state->accepting = false;
    }
    if (!m_state->executor)
    {
        Common::OperationResult result;
        result.add(serviceDiagnostic(QStringLiteral("TASK-EXECUTOR-MISSING"),
                                     QStringLiteral("No task executor is available."),
                                     QStringLiteral("Pending tasks cannot be awaited.")));
        return result;
    }
    return m_state->executor->waitForDone(timeoutMs);
}

bool TaskService::isAccepting() const noexcept
{
    std::lock_guard<std::mutex> guard(m_state->taskMutex);
    return m_state->accepting;
}
}
