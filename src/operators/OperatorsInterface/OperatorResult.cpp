#include "OperatorResult.h"

namespace AppMesh::Operators
{
OperatorResult OperatorResult::success(
    std::optional<ResultReference> result,
    QVector<Common::Diagnostic> diagnostics)
{
    return OperatorResult(true, std::move(result), std::nullopt, std::move(diagnostics));
}

std::optional<OperatorResult> OperatorResult::failure(
    std::optional<ErrorInfo> error,
    QVector<Common::Diagnostic> diagnostics)
{
    if (!error || !validateErrorInfo(*error).succeeded())
    {
        return std::nullopt;
    }
    return OperatorResult(false, std::nullopt, std::move(error), std::move(diagnostics));
}

OperatorResult::OperatorResult(bool succeeded,
                               std::optional<ResultReference> result,
                               std::optional<ErrorInfo> error,
                               QVector<Common::Diagnostic> diagnostics)
    : m_succeeded(succeeded),
      m_result(std::move(result)),
      m_error(std::move(error)),
      m_diagnostics(std::move(diagnostics))
{
}

bool OperatorResult::succeeded() const noexcept
{
    return m_succeeded;
}

std::optional<ResultReference> OperatorResult::result() const
{
    return m_result;
}

std::optional<ErrorInfo> OperatorResult::error() const
{
    return m_error;
}

QVector<Common::Diagnostic> OperatorResult::diagnostics() const
{
    return m_diagnostics;
}
}
