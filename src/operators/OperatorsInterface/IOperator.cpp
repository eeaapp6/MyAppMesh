#include "IOperator.h"

#include <exception>

namespace AppMesh::Operators
{
namespace
{
OperatorResult failureFromError(ErrorInfo error,
                                QVector<Common::Diagnostic> diagnostics)
{
    auto result = OperatorResult::failure(std::move(error), std::move(diagnostics));
    return std::move(*result);
}

ErrorInfo executionError(const QString& code,
                         const QString& message,
                         const QString& detail,
                         TaskId taskId)
{
    return {QStringLiteral("operator"),
            code,
            message,
            detail,
            false,
            taskId,
            Model::InvalidObjectId,
            QStringLiteral("execute")};
}
}

OperatorResult executeSafely(IOperator& operation,
                             const OperatorInput& input,
                             TaskId taskId)
{
    try
    {
        const auto validation = operation.validate(input);
        if (!validation.succeeded())
        {
            ErrorInfo error = errorInfoFromDiagnostic(validation.diagnostics.front(), taskId);
            if (!validateErrorInfo(error).succeeded())
            {
                error = executionError(
                    QStringLiteral("TASK-OPERATOR-VALIDATION-FAILED"),
                    QStringLiteral("Operator input validation failed."),
                    QStringLiteral("The validation result did not provide a complete diagnostic."),
                    taskId);
            }
            return failureFromError(std::move(error), validation.diagnostics);
        }

        OperatorResult result = operation.execute(input);
        const auto error = result.error();
        if (!result.succeeded() && error && error->taskId == InvalidTaskId &&
            taskId != InvalidTaskId)
        {
            ErrorInfo associatedError = *error;
            associatedError.taskId = taskId;
            return failureFromError(std::move(associatedError), result.diagnostics());
        }
        return result;
    }
    catch (const std::exception& exception)
    {
        ErrorInfo error = executionError(
            QStringLiteral("TASK-OPERATOR-EXCEPTION"),
            QStringLiteral("The operator raised an exception."),
            QString::fromLocal8Bit(exception.what()),
            taskId);
        return failureFromError(error,
                                {diagnosticFromErrorInfo(
                                    error, QStringLiteral("operator.execute"))});
    }
    catch (...)
    {
        ErrorInfo error = executionError(
            QStringLiteral("TASK-OPERATOR-UNKNOWN-EXCEPTION"),
            QStringLiteral("The operator raised an unknown exception."),
            QStringLiteral("No exception detail is available."),
            taskId);
        return failureFromError(error,
                                {diagnosticFromErrorInfo(
                                    error, QStringLiteral("operator.execute"))});
    }
}
}
