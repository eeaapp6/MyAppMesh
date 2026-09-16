#pragma once

#include "AppDiagnostic.h"

#include <QString>
#include <QStringList>

#include <vector>

namespace AppMesh::App
{
struct FITKComponentMapping
{
    QString stage;
    QString fitkType;
    QString registrationKey;
    QString library;
    QStringList dependencies;
    int startupOrder = -1;
    int shutdownOrder = -1;
    bool realFITKEnabled = false;
    QString disabledReason;
};

const std::vector<FITKComponentMapping>& fitkComponentMappings();
const FITKComponentMapping* fitkMappingForType(const QString& fitkType);
QStringList fitkStartupOrder();
QStringList fitkShutdownOrder();
AppOperationResult validateFITKComponentMappings();
}
