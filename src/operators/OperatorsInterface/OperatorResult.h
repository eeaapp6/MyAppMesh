#pragma once

#include "ErrorInfo.h"

#include <optional>

namespace AppMesh::Operators
{
class OperatorResult final
{
public:
    static OperatorResult success(
        std::optional<ResultReference> result = std::nullopt,
        QVector<Common::Diagnostic> diagnostics = {});
    static std::optional<OperatorResult> failure(
        std::optional<ErrorInfo> error,
        QVector<Common::Diagnostic> diagnostics = {});

    bool succeeded() const noexcept;
    std::optional<ResultReference> result() const;
    std::optional<ErrorInfo> error() const;
    QVector<Common::Diagnostic> diagnostics() const;

private:
    OperatorResult(bool succeeded,
                   std::optional<ResultReference> result,
                   std::optional<ErrorInfo> error,
                   QVector<Common::Diagnostic> diagnostics);

    bool m_succeeded = false;
    std::optional<ResultReference> m_result;
    std::optional<ErrorInfo> m_error;
    QVector<Common::Diagnostic> m_diagnostics;
};
}
