#include "Task.h"

#include <QMutexLocker>

namespace AppMesh::Operators
{
namespace
{
Common::Diagnostic taskDiagnostic(const QString& code,
                                  const QString& message,
                                  const QString& detail,
                                  TaskId taskId = InvalidTaskId)
{
    return {QStringLiteral("task"),
            code,
            message,
            detail,
            false,
            QStringLiteral("task"),
            QStringLiteral("operator.task"),
            QString(),
            taskId == InvalidTaskId ? QString() : QString::number(taskId)};
}
}

TaskIdAllocator::TaskIdAllocator(TaskId firstId)
    : m_next(firstId == InvalidTaskId ? 1 : firstId)
{
}

TaskIdAllocationResult TaskIdAllocator::allocate()
{
    QMutexLocker locker(&m_mutex);
    TaskIdAllocationResult result;
    if (m_exhausted)
    {
        result.add(taskDiagnostic(
            QStringLiteral("TASK-ID-EXHAUSTED"),
            QStringLiteral("No task identifier is available."),
            QStringLiteral("The process-local monotonic TaskId range is exhausted.")));
        return result;
    }

    result.id = m_next;
    if (m_next == std::numeric_limits<TaskId>::max())
    {
        m_exhausted = true;
    }
    else
    {
        ++m_next;
    }
    return result;
}

TaskCreateResult Task::create(TaskId id, const OperatorInput& input)
{
    TaskCreateResult result;
    if (id == InvalidTaskId)
    {
        result.add(taskDiagnostic(
            QStringLiteral("TASK-ID-INVALID"),
            QStringLiteral("A task requires a valid identifier."),
            QStringLiteral("TaskId zero is reserved as invalid.")));
    }
    if (input.operationKey.trimmed().isEmpty())
    {
        result.add(taskDiagnostic(
            QStringLiteral("TASK-OPERATION-EMPTY"),
            QStringLiteral("A task requires an operation key."),
            QStringLiteral("OperatorInput.operationKey must not be empty."),
            id));
    }
    if (!result.succeeded())
    {
        return result;
    }
    result.task.reset(new Task(id, input));
    return result;
}

Task::Task(TaskId id, OperatorInput input)
{
    input.operationKey = input.operationKey.trimmed();
    m_snapshot.id = id;
    m_snapshot.input = std::move(input);
    m_snapshot.createdAt = QDateTime::currentDateTimeUtc();
}

TaskSnapshot Task::snapshot() const
{
    QMutexLocker locker(&m_mutex);
    return m_snapshot;
}

TaskUpdateResult Task::start()
{
    QMutexLocker locker(&m_mutex);
    if (m_snapshot.status != TaskStatus::Created)
    {
        return invalidUpdateLocked(
            QStringLiteral("TASK-START-INVALID"),
            QStringLiteral("The task cannot start from its current state."),
            QStringLiteral("Only Created can transition to Executing."));
    }
    m_snapshot.status = TaskStatus::Executing;
    m_snapshot.startedAt = QDateTime::currentDateTimeUtc();
    TaskUpdateResult result;
    result.event = eventLocked(TaskEventType::Started);
    ++m_nextEventSequence;
    return result;
}

TaskUpdateResult Task::updateProgress(int progress, const QString& phase)
{
    QMutexLocker locker(&m_mutex);
    if (m_snapshot.status != TaskStatus::Executing)
    {
        return invalidUpdateLocked(
            QStringLiteral("TASK-PROGRESS-STATE-INVALID"),
            QStringLiteral("Task progress cannot be updated in the current state."),
            QStringLiteral("Progress is mutable only while Executing."));
    }
    if (progress < 0 || progress > 100)
    {
        return invalidUpdateLocked(
            QStringLiteral("TASK-PROGRESS-RANGE-INVALID"),
            QStringLiteral("Task progress is outside the supported range."),
            QStringLiteral("Progress must be between 0 and 100 inclusive."));
    }

    m_snapshot.progress = progress;
    if (!phase.trimmed().isEmpty())
    {
        m_snapshot.phase = phase.trimmed();
    }
    TaskUpdateResult result;
    result.event = eventLocked(TaskEventType::ProgressChanged);
    ++m_nextEventSequence;
    return result;
}

TaskUpdateResult Task::reportDiagnostic(const Common::Diagnostic& diagnostic)
{
    QMutexLocker locker(&m_mutex);
    if (m_snapshot.status != TaskStatus::Executing)
    {
        return invalidUpdateLocked(
            QStringLiteral("TASK-DIAGNOSTIC-STATE-INVALID"),
            QStringLiteral("A diagnostic cannot be reported in the current state."),
            QStringLiteral("Business diagnostics are accepted only while Executing."));
    }
    if (diagnostic.code.trimmed().isEmpty() || diagnostic.message.trimmed().isEmpty())
    {
        return invalidUpdateLocked(
            QStringLiteral("TASK-DIAGNOSTIC-INVALID"),
            QStringLiteral("The task diagnostic is incomplete."),
            QStringLiteral("Diagnostic code and message must not be empty."));
    }

    Common::Diagnostic copied = diagnostic;
    if (copied.taskId.isEmpty())
    {
        copied.taskId = QString::number(m_snapshot.id);
    }
    m_snapshot.diagnostics.append(copied);
    TaskUpdateResult result;
    TaskEvent event = eventLocked(TaskEventType::DiagnosticReported);
    event.diagnostic = copied;
    result.event = std::move(event);
    ++m_nextEventSequence;
    return result;
}

TaskUpdateResult Task::succeed(std::optional<ResultReference> resultReference)
{
    QMutexLocker locker(&m_mutex);
    if (m_snapshot.status != TaskStatus::Executing)
    {
        return invalidUpdateLocked(
            QStringLiteral("TASK-SUCCEED-INVALID"),
            QStringLiteral("The task cannot succeed from its current state."),
            QStringLiteral("Only Executing can transition to Succeeded."));
    }
    m_snapshot.status = TaskStatus::Succeeded;
    m_snapshot.completedAt = QDateTime::currentDateTimeUtc();
    m_snapshot.result = std::move(resultReference);
    m_snapshot.error.reset();
    TaskUpdateResult result;
    result.event = eventLocked(TaskEventType::Succeeded);
    ++m_nextEventSequence;
    return result;
}

TaskUpdateResult Task::fail(const ErrorInfo& error)
{
    QMutexLocker locker(&m_mutex);
    if (m_snapshot.status != TaskStatus::Executing)
    {
        return invalidUpdateLocked(
            QStringLiteral("TASK-FAIL-INVALID"),
            QStringLiteral("The task cannot fail from its current state."),
            QStringLiteral("Only Executing can transition to Failed."));
    }
    const auto validation = validateErrorInfo(error);
    if (!validation.succeeded())
    {
        TaskUpdateResult result;
        result.diagnostics = validation.diagnostics;
        return result;
    }
    if (error.taskId != InvalidTaskId && error.taskId != m_snapshot.id)
    {
        return invalidUpdateLocked(
            QStringLiteral("TASK-ERROR-ID-MISMATCH"),
            QStringLiteral("The failure belongs to another task."),
            QStringLiteral("ErrorInfo.taskId must be invalid or match this TaskId."));
    }

    ErrorInfo copied = error;
    copied.taskId = m_snapshot.id;
    m_snapshot.status = TaskStatus::Failed;
    m_snapshot.completedAt = QDateTime::currentDateTimeUtc();
    m_snapshot.result.reset();
    m_snapshot.error = copied;
    m_snapshot.diagnostics.append(
        diagnosticFromErrorInfo(copied, QStringLiteral("operator.task.failed")));
    TaskUpdateResult result;
    TaskEvent event = eventLocked(TaskEventType::Failed);
    event.error = copied;
    result.event = std::move(event);
    ++m_nextEventSequence;
    return result;
}

TaskUpdateResult Task::invalidUpdateLocked(const QString& code,
                                           const QString& message,
                                           const QString& detail) const
{
    TaskUpdateResult result;
    result.add(taskDiagnostic(code, message, detail, m_snapshot.id));
    return result;
}

TaskEvent Task::eventLocked(TaskEventType type) const
{
    TaskEvent event;
    event.taskId = m_snapshot.id;
    event.sequence = m_nextEventSequence;
    event.type = type;
    event.timestamp = QDateTime::currentDateTimeUtc();
    event.status = m_snapshot.status;
    event.progress = m_snapshot.progress;
    event.phase = m_snapshot.phase;
    event.result = m_snapshot.result;
    event.error = m_snapshot.error;
    return event;
}

}
