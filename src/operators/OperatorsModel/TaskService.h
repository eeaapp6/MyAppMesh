#pragma once

#include "ITaskExecutor.h"
#include "operators/OperatorsInterface/IOperator.h"
#include "operators/OperatorsInterface/Task.h"

#include <functional>
#include <memory>
#include <optional>

namespace AppMesh::OperatorsModel
{
struct TaskServiceState;

class TaskEventSubscription final
{
public:
    TaskEventSubscription() = default;
    TaskEventSubscription(const TaskEventSubscription&) = delete;
    TaskEventSubscription& operator=(const TaskEventSubscription&) = delete;
    TaskEventSubscription(TaskEventSubscription&& other) noexcept;
    TaskEventSubscription& operator=(TaskEventSubscription&& other) noexcept;
    ~TaskEventSubscription();

    void reset() noexcept;
    bool isActive() const noexcept;

private:
    friend class TaskService;
    TaskEventSubscription(std::weak_ptr<TaskServiceState> state, quint64 id) noexcept;

    std::weak_ptr<TaskServiceState> m_state;
    quint64 m_id = 0;
};

struct TaskSubmissionResult : Common::OperationResult
{
    std::optional<Operators::TaskId> taskId;
};

class TaskService final
{
public:
    explicit TaskService(std::shared_ptr<ITaskExecutor> executor,
                         Operators::TaskId firstTaskId = 1);
    ~TaskService();

    TaskService(const TaskService&) = delete;
    TaskService& operator=(const TaskService&) = delete;

    TaskSubmissionResult submit(const std::shared_ptr<Operators::IOperator>& operation,
                                const Operators::OperatorInput& input);
    std::optional<Operators::TaskSnapshot> findById(Operators::TaskId id) const;
    QVector<Operators::TaskSnapshot> snapshots() const;
    TaskEventSubscription subscribe(Operators::TaskEventObserver observer);
    Common::OperationResult updateProgress(Operators::TaskId id,
                                           int progress,
                                           const QString& phase = QString());
    Common::OperationResult reportDiagnostic(Operators::TaskId id,
                                             const Common::Diagnostic& diagnostic);

    Common::OperationResult stop(int timeoutMs = 5000);
    void stopAccepting() noexcept;
    void drain() noexcept;
    bool isAccepting() const noexcept;

private:
    std::shared_ptr<TaskServiceState> m_state;
};
}
