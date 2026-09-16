#pragma once

#include <QString>
#include <QVector>

#include <utility>

namespace AppMesh::Common
{
struct Diagnostic
{
    QString category;
    QString code;
    QString message;
    QString detail;
    bool recoverable = false;
    QString path;
    QString stage;
    QString objectId;
};

struct OperationResult
{
    QVector<Diagnostic> diagnostics;

    bool succeeded() const noexcept
    {
        return diagnostics.isEmpty();
    }

    void add(Diagnostic diagnostic)
    {
        diagnostics.append(std::move(diagnostic));
    }
};
}
