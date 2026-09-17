#include "FITKTaskExecutor.h"

#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"

#include <chrono>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <new>
#include <utility>

namespace AppMesh::OperatorsModel
{
struct FITKTaskExecutorState
{
    std::mutex mutex;
    std::condition_variable finished;
    int pending = 0;
};

namespace
{
Common::Diagnostic executorDiagnostic(const QString& code,
                                      const QString& message,
                                      const QString& detail)
{
    return {QStringLiteral("task"),
            code,
            message,
            detail,
            false,
            QStringLiteral("executor"),
            QStringLiteral("operator.executor.fitk"),
            QString(),
            QString()};
}

void finish(const std::shared_ptr<FITKTaskExecutorState>& state) noexcept
{
    std::lock_guard<std::mutex> guard(state->mutex);
    if (state->pending > 0)
    {
        --state->pending;
    }
    state->finished.notify_all();
}

class FunctionThreadTask final : public Core::FITKThreadTask
{
public:
    FunctionThreadTask(TaskWork work,
                       std::shared_ptr<FITKTaskExecutorState> state)
        : m_work(std::move(work)), m_state(std::move(state))
    {
    }

    void run() override
    {
        try
        {
            m_work();
        }
        catch (...)
        {
            // TaskService wraps operator exceptions. This final boundary keeps
            // an unexpected executor callable from escaping through QThreadPool.
        }
        finish(m_state);
    }

private:
    TaskWork m_work;
    std::shared_ptr<FITKTaskExecutorState> m_state;
};
}

FITKTaskExecutor::FITKTaskExecutor()
    : m_state(std::make_shared<FITKTaskExecutorState>())
{
}

FITKTaskExecutor::~FITKTaskExecutor()
{
    waitForDone(5000);
}

Common::OperationResult FITKTaskExecutor::submit(TaskWork work)
{
    Common::OperationResult result;
    if (!work)
    {
        result.add(executorDiagnostic(QStringLiteral("TASK-EXECUTOR-WORK-EMPTY"),
                                      QStringLiteral("The submitted task is empty."),
                                      QStringLiteral("Provide one callable for FITKThreadPool.")));
        return result;
    }

    FunctionThreadTask* task = nullptr;
    try
    {
        task = new FunctionThreadTask(std::move(work), m_state);
    }
    catch (const std::bad_alloc&)
    {
        result.add(executorDiagnostic(QStringLiteral("TASK-EXECUTOR-RESOURCE-EXHAUSTED"),
                                      QStringLiteral("The FITK task could not be allocated."),
                                      QStringLiteral("No work was submitted.")));
        return result;
    }

    {
        std::lock_guard<std::mutex> guard(m_state->mutex);
        ++m_state->pending;
    }
    try
    {
        Core::FITKThreadPool::getInstance()->execTask(task);
    }
    catch (const std::exception& exception)
    {
        task->setAutoDelete(false);
        delete task;
        finish(m_state);
        result.add(executorDiagnostic(QStringLiteral("TASK-EXECUTOR-SUBMIT-FAILED"),
                                      QStringLiteral("FITKThreadPool rejected the task."),
                                      QString::fromLocal8Bit(exception.what())));
    }
    catch (...)
    {
        task->setAutoDelete(false);
        delete task;
        finish(m_state);
        result.add(executorDiagnostic(QStringLiteral("TASK-EXECUTOR-SUBMIT-FAILED"),
                                      QStringLiteral("FITKThreadPool rejected the task."),
                                      QStringLiteral("The pool raised an unknown exception.")));
    }
    return result;
}

Common::OperationResult FITKTaskExecutor::waitForDone(int timeoutMs)
{
    Common::OperationResult result;
    if (timeoutMs < 0)
    {
        result.add(executorDiagnostic(QStringLiteral("TASK-EXECUTOR-TIMEOUT-INVALID"),
                                      QStringLiteral("The executor timeout is invalid."),
                                      QStringLiteral("Use a non-negative bounded timeout.")));
        return result;
    }
    std::unique_lock<std::mutex> lock(m_state->mutex);
    const bool completed = m_state->finished.wait_for(
        lock,
        std::chrono::milliseconds(timeoutMs),
        [this] { return m_state->pending == 0; });
    if (!completed)
    {
        result.add(executorDiagnostic(QStringLiteral("TASK-EXECUTOR-WAIT-TIMEOUT"),
                                      QStringLiteral("Submitted FITK tasks did not finish in time."),
                                      QStringLiteral("The service remains safe, but work is still pending.")));
    }
    return result;
}
}
