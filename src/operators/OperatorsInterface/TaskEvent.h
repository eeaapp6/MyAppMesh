#pragma once

#include "ErrorInfo.h"

#include <QDateTime>

#include <functional>
#include <optional>

namespace AppMesh::Operators
{
enum class TaskEventType : quint8
{
    Started,
    ProgressChanged,
    DiagnosticReported,
    Succeeded,
    Failed
};

struct TaskEvent
{
    TaskId taskId = InvalidTaskId;
    quint64 sequence = 0;
    TaskEventType type = TaskEventType::Started;
    QDateTime timestamp;
    TaskStatus status = TaskStatus::Created;
    std::optional<int> progress;
    std::optional<QString> phase;
    std::optional<ResultReference> result;
    std::optional<ErrorInfo> error;
    std::optional<Common::Diagnostic> diagnostic;
};

// A future scheduler chooses the callback thread and owns observer lifetime.
// The contract itself stores no observer and performs no dispatch.
using TaskEventObserver = std::function<void(const TaskEvent&)>;
}
