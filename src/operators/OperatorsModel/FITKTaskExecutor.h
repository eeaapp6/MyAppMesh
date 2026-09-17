#pragma once

#include "ITaskExecutor.h"

#include <memory>

namespace AppMesh::OperatorsModel
{
struct FITKTaskExecutorState;

class FITKTaskExecutor final : public ITaskExecutor
{
public:
    FITKTaskExecutor();
    ~FITKTaskExecutor() override;

    FITKTaskExecutor(const FITKTaskExecutor&) = delete;
    FITKTaskExecutor& operator=(const FITKTaskExecutor&) = delete;

    Common::OperationResult submit(TaskWork work) override;
    Common::OperationResult waitForDone(int timeoutMs) override;

private:
    std::shared_ptr<FITKTaskExecutorState> m_state;
};
}
