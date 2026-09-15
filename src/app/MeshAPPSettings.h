#pragma once

#include "AppDiagnostic.h"

#include <QStringList>

namespace AppMesh::App
{
struct AppSettings
{
    static constexpr int CurrentSchemaVersion = 1;

    int schemaVersion = CurrentSchemaVersion;
    QString workingDirectory;
    QStringList recentFiles;
};

struct SettingsResult : AppOperationResult
{
    AppSettings settings;
};

// Keeps APPMesh-owned versioning and validation separate from the FastCAE
// FITKAppSettings storage API. T004 may adapt this object to FITKAppSettings.
class MeshAPPSettings
{
public:
    explicit MeshAPPSettings(QString settingsFilePath, int maximumRecentFiles = 10);

    QString settingsFilePath() const;
    SettingsResult load() const;
    AppOperationResult save(const AppSettings& settings) const;

private:
    SettingsResult validateAndNormalize(const AppSettings& settings) const;

    QString m_settingsFilePath;
    int m_maximumRecentFiles;
};
}
