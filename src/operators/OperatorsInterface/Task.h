#pragma once

#include "TaskEvent.h"

#include <QMutex>

#include <limits>
#include <memory>
#include <optional>

namespace AppMesh::Operators
{
class Task;

struct TaskIdAllocationResult : Common::OperationResult
{
    std::optional<TaskId> id;
};

class TaskIdAllocator final
{
public:
    explicit TaskIdAllocator(TaskId firstId = 1);
    TaskIdAllocationResult allocate();

private:
    QMutex m_mutex;
    TaskId m_next = 1;
    bool m_exhausted = false;
};

struct TaskSnapshot
{
    TaskId id = InvalidTaskId;
    OperatorInput input;
    TaskStatus status = TaskStatus::Created;
    QDateTime createdAt;
    QDateTime startedAt;
    QDateTime completedAt;
    std::optional<int> progress;
    std::optional<QString> phase;
    std::optional<ResultReference> result;
    std::optional<ErrorInfo> error;
    QVector<Common::Diagnostic> diagnostics;
};

struct TaskCreateResult : Common::OperationResult
{
    std::unique_ptr<Task> task;
};

struct TaskUpdateResult : Common::OperationResult
{
    std::optional<TaskEvent> event;
};

class Task final
{
public:
    static TaskCreateResult create(TaskId id, const OperatorInput& input);

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
    Task(Task&&) = delete;
    Task& operator=(Task&&) = delete;

    TaskSnapshot snapshot() const;
    TaskUpdateResult start();
    TaskUpdateResult updateProgress(int progress, const QString& phase = QString());
    TaskUpdateResult reportDiagnostic(const Common::Diagnostic& diagnostic);
    TaskUpdateResult succeed(std::optional<ResultReference> result = std::nullopt);
    TaskUpdateResult fail(const ErrorInfo& error);

private:
    Task(TaskId id, OperatorInput input);

    TaskUpdateResult invalidUpdateLocked(const QString& code,
                                         const QString& message,
                                         const QString& detail) const;
    TaskEvent eventLocked(TaskEventType type) const;

    mutable QMutex m_mutex;
    TaskSnapshot m_snapshot;
    quint64 m_nextEventSequence = 1;
};
}
