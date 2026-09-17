#include "ErrorInfo.h"

namespace AppMesh::Operators
{
namespace
{
template<typename Id>
Id parseId(const QString& value)
{
    if (value.isEmpty())
    {
        return 0;
    }
    bool ok = false;
    const qulonglong parsed = value.toULongLong(&ok);
    return ok ? static_cast<Id>(parsed) : 0;
}

Common::Diagnostic validationDiagnostic(const QString& code,
                                        const QString& message,
                                        const QString& detail)
{
    return {QStringLiteral("validation"),
            code,
            message,
            detail,
            false,
            QStringLiteral("error"),
            QStringLiteral("operator.error.validate"),
            QString(),
            QString()};
}
}

Common::OperationResult validateErrorInfo(const ErrorInfo& error)
{
    Common::OperationResult result;
    if (error.code.trimmed().isEmpty())
    {
        result.add(validationDiagnostic(
            QStringLiteral("TASK-ERROR-CODE-EMPTY"),
            QStringLiteral("An operator error requires a diagnostic code."),
            QStringLiteral("ErrorInfo.code must not be empty.")));
    }
    if (error.message.trimmed().isEmpty())
    {
        result.add(validationDiagnostic(
            QStringLiteral("TASK-ERROR-MESSAGE-EMPTY"),
            QStringLiteral("An operator error requires a message."),
            QStringLiteral("ErrorInfo.message must not be empty.")));
    }
    return result;
}

ErrorInfo errorInfoFromDiagnostic(const Common::Diagnostic& diagnostic,
                                  TaskId defaultTaskId)
{
    ErrorInfo error;
    error.category = diagnostic.category;
    error.code = diagnostic.code;
    error.message = diagnostic.message;
    error.detail = diagnostic.detail;
    error.recoverable = diagnostic.recoverable;
    error.taskId = diagnostic.taskId.isEmpty()
        ? defaultTaskId
        : parseId<TaskId>(diagnostic.taskId);
    error.objectId = parseId<Model::ObjectId>(diagnostic.objectId);
    error.path = diagnostic.path;
    return error;
}

Common::Diagnostic diagnosticFromErrorInfo(const ErrorInfo& error,
                                           const QString& stage)
{
    return {error.category,
            error.code,
            error.message,
            error.detail,
            error.recoverable,
            error.path,
            stage,
            error.objectId == Model::InvalidObjectId
                ? QString()
                : QString::number(error.objectId),
            error.taskId == InvalidTaskId
                ? QString()
                : QString::number(error.taskId)};
}
}
