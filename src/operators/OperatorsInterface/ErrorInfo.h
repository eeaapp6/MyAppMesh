#pragma once

#include "TaskTypes.h"
#include "common/AppDiagnostic.h"

namespace AppMesh::Operators
{
struct ErrorInfo
{
    QString category;
    QString code;
    QString message;
    QString detail;
    bool recoverable = false;
    TaskId taskId = InvalidTaskId;
    Model::ObjectId objectId = Model::InvalidObjectId;
    QString path;
};

Common::OperationResult validateErrorInfo(const ErrorInfo& error);
ErrorInfo errorInfoFromDiagnostic(const Common::Diagnostic& diagnostic,
                                  TaskId defaultTaskId = InvalidTaskId);
Common::Diagnostic diagnosticFromErrorInfo(
    const ErrorInfo& error,
    const QString& stage = QStringLiteral("operator"));
}
