#pragma once

#include "common/AppDiagnostic.h"

#include <functional>

namespace AppMesh::OperatorsModel
{
using TaskWork = std::function<void()>;

class ITaskExecutor
{
public:
    virtual ~ITaskExecutor() = default;

    virtual Common::OperationResult submit(TaskWork work) = 0;
    virtual Common::OperationResult waitForDone(int timeoutMs) = 0;
};
}
