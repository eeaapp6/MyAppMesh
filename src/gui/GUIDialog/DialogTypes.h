#pragma once

#include "common/AppDiagnostic.h"
#include <QString>

#include <QVariantMap>

namespace AppMesh::Gui
{
struct WorkDirectoryRequest
{
    QString directory;
    bool createIfMissing = false;
};

struct GeneratorRequest
{
    QString generatorId;
    QVariantMap parameters;
};

enum class ProjectDialogMode
{
    Open,
    Save
};

struct ProjectRequest
{
    ProjectDialogMode mode = ProjectDialogMode::Open;
    QString projectPath;
    bool overwriteExisting = false;
};
}
