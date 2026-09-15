#include "MeshAPPSettings.h"

#include "SystemChecker.h"

#include <QDir>
#include <QFileInfo>
#include <QSet>
#include <QSettings>

namespace AppMesh::App
{
namespace
{
QString normalizedPath(const QString& path)
{
    return QDir::cleanPath(QFileInfo(path).absoluteFilePath());
}

AppDiagnostic configDiagnostic(const QString& code,
                               const QString& message,
                               const QString& detail,
                               const QString& path)
{
    return {QStringLiteral("configuration"), code, message, detail, true, path};
}
}

MeshAPPSettings::MeshAPPSettings(QString settingsFilePath, int maximumRecentFiles)
    : m_settingsFilePath(settingsFilePath.trimmed().isEmpty()
                             ? QString()
                             : normalizedPath(settingsFilePath)),
      m_maximumRecentFiles(qMax(0, maximumRecentFiles))
{
}

QString MeshAPPSettings::settingsFilePath() const
{
    return m_settingsFilePath;
}

SettingsResult MeshAPPSettings::validateAndNormalize(const AppSettings& settings) const
{
    SettingsResult result;
    result.settings.schemaVersion = settings.schemaVersion;

    if (settings.schemaVersion != AppSettings::CurrentSchemaVersion)
    {
        result.add(configDiagnostic(QStringLiteral("CFG-VERSION-UNSUPPORTED"),
                                    QStringLiteral("Settings version is not supported."),
                                    QStringLiteral("Expected schema %1, received %2.")
                                        .arg(AppSettings::CurrentSchemaVersion)
                                        .arg(settings.schemaVersion),
                                    m_settingsFilePath));
        return result;
    }

    if (settings.workingDirectory.trimmed().isEmpty())
    {
        result.add(configDiagnostic(QStringLiteral("CFG-WORKDIR-INVALID"),
                                    QStringLiteral("Working directory is not configured."),
                                    QStringLiteral("An existing writable directory is required."),
                                    settings.workingDirectory));
        return result;
    }

    result.settings.workingDirectory = normalizedPath(settings.workingDirectory);
    NativeSystemProbe probe;
    if (!probe.pathExists(result.settings.workingDirectory) ||
        !probe.isDirectory(result.settings.workingDirectory) ||
        !probe.isWritableDirectory(result.settings.workingDirectory))
    {
        result.add(configDiagnostic(QStringLiteral("CFG-WORKDIR-INVALID"),
                                    QStringLiteral("Working directory is invalid or not writable."),
                                    QStringLiteral("The directory must exist and accept a temporary write probe."),
                                    result.settings.workingDirectory));
        return result;
    }

    QSet<QString> seenPaths;
    for (const auto& recentPath : settings.recentFiles)
    {
        if (result.settings.recentFiles.size() >= m_maximumRecentFiles)
        {
            break;
        }
        if (recentPath.trimmed().isEmpty())
        {
            continue;
        }

        const QString normalizedRecentPath = normalizedPath(recentPath);
        const QFileInfo recentInfo(normalizedRecentPath);
        if (!recentInfo.exists() || !recentInfo.isFile())
        {
            continue;
        }

        const QString comparisonKey = normalizedRecentPath.toCaseFolded();
        if (!seenPaths.contains(comparisonKey))
        {
            seenPaths.insert(comparisonKey);
            result.settings.recentFiles.append(normalizedRecentPath);
        }
    }
    return result;
}

SettingsResult MeshAPPSettings::load() const
{
    SettingsResult result;
    if (m_settingsFilePath.isEmpty())
    {
        result.add(configDiagnostic(QStringLiteral("CFG-PATH-INVALID"),
                                    QStringLiteral("Settings file path is not configured."),
                                    QStringLiteral("Provide an absolute or resolvable INI file path."),
                                    m_settingsFilePath));
        return result;
    }

    const QFileInfo settingsInfo(m_settingsFilePath);
    if (!settingsInfo.exists())
    {
        return result;
    }
    if (!settingsInfo.isFile())
    {
        result.add(configDiagnostic(QStringLiteral("CFG-PATH-INVALID"),
                                    QStringLiteral("Settings path is not a file."),
                                    QStringLiteral("The configured path refers to a directory or unsupported object."),
                                    m_settingsFilePath));
        return result;
    }

    QSettings storage(m_settingsFilePath, QSettings::IniFormat);
    if (!storage.contains(QStringLiteral("meta/schemaVersion")))
    {
        result.add(configDiagnostic(QStringLiteral("CFG-VERSION-MISSING"),
                                    QStringLiteral("Settings version is missing."),
                                    QStringLiteral("The file cannot be safely interpreted without a schema version."),
                                    m_settingsFilePath));
        return result;
    }

    AppSettings stored;
    stored.schemaVersion = storage.value(QStringLiteral("meta/schemaVersion")).toInt();
    stored.workingDirectory = storage.value(QStringLiteral("paths/workingDirectory")).toString();
    stored.recentFiles = storage.value(QStringLiteral("history/recentFiles")).toStringList();
    result = validateAndNormalize(stored);
    if (storage.status() != QSettings::NoError)
    {
        result.add(configDiagnostic(QStringLiteral("CFG-READ-FAILED"),
                                    QStringLiteral("Settings could not be read."),
                                    QStringLiteral("QSettings reported status %1.").arg(storage.status()),
                                    m_settingsFilePath));
    }
    return result;
}

AppOperationResult MeshAPPSettings::save(const AppSettings& settings) const
{
    const auto validated = validateAndNormalize(settings);
    AppOperationResult result;
    result.diagnostics = validated.diagnostics;
    if (!result.succeeded())
    {
        return result;
    }

    if (m_settingsFilePath.isEmpty())
    {
        result.add(configDiagnostic(QStringLiteral("CFG-PATH-INVALID"),
                                    QStringLiteral("Settings file path is not configured."),
                                    QStringLiteral("Provide an absolute or resolvable INI file path."),
                                    m_settingsFilePath));
        return result;
    }

    const QFileInfo destination(m_settingsFilePath);
    if (destination.exists() && !destination.isFile())
    {
        result.add(configDiagnostic(QStringLiteral("CFG-PATH-INVALID"),
                                    QStringLiteral("Settings path is not a file."),
                                    QStringLiteral("The destination refers to a directory or unsupported object."),
                                    m_settingsFilePath));
        return result;
    }
    if (!QDir().mkpath(destination.absolutePath()))
    {
        result.add(configDiagnostic(QStringLiteral("CFG-DIRECTORY-CREATE-FAILED"),
                                    QStringLiteral("Settings directory could not be created."),
                                    QStringLiteral("Check the parent directory permissions."),
                                    destination.absolutePath()));
        return result;
    }

    QSettings storage(m_settingsFilePath, QSettings::IniFormat);
    storage.setAtomicSyncRequired(true);
    storage.clear();
    storage.setValue(QStringLiteral("meta/schemaVersion"), validated.settings.schemaVersion);
    storage.setValue(QStringLiteral("paths/workingDirectory"), validated.settings.workingDirectory);
    storage.setValue(QStringLiteral("history/recentFiles"), validated.settings.recentFiles);
    storage.sync();
    if (storage.status() != QSettings::NoError)
    {
        result.add(configDiagnostic(QStringLiteral("CFG-WRITE-FAILED"),
                                    QStringLiteral("Settings could not be persisted."),
                                    QStringLiteral("QSettings reported status %1.").arg(storage.status()),
                                    m_settingsFilePath));
    }
    return result;
}
}
