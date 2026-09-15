#pragma once

#include <QString>
#include <QVector>

#include <utility>

namespace AppMesh::App
{
struct AppDiagnostic
{
    QString category;
    QString code;
    QString message;
    QString detail;
    bool recoverable = false;
    QString path;
};

struct AppOperationResult
{
    QVector<AppDiagnostic> diagnostics;

    bool succeeded() const noexcept
    {
        return diagnostics.isEmpty();
    }

    void add(AppDiagnostic diagnostic)
    {
        diagnostics.append(std::move(diagnostic));
    }
};
}
