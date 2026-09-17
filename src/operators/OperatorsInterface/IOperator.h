#pragma once

#include "OperatorResult.h"

namespace AppMesh::Operators
{
class IOperator
{
public:
    virtual ~IOperator() = default;

    virtual QString key() const = 0;
    virtual Common::OperationResult validate(const OperatorInput& input) const = 0;
    virtual OperatorResult execute(const OperatorInput& input) = 0;
};

// This is an exception/validation boundary only. It performs no scheduling and
// makes no promise about the calling thread.
OperatorResult executeSafely(IOperator& operation,
                             const OperatorInput& input,
                             TaskId taskId = InvalidTaskId);
}
