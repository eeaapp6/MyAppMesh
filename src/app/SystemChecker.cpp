#include "SystemChecker.h"

#include <QDir>
#include <QFileInfo>
#include <QStorageInfo>
#include <QTemporaryFile>

#include <limits>

namespace AppMesh::App
{
namespace
{
QString absoluteCleanPath(const QString& path)
{
    return QDir::cleanPath(QFileInfo(path).absoluteFilePath());
}

AppDiagnostic diagnostic(const QString& code,
                         const QString& message,
                         const QString& detail,
                         const QString& path)
{
    return {QStringLiteral("environment"), code, message, detail, false, path};
}
}

bool NativeSystemProbe::pathExists(const QString& path) const
{
    return QFileInfo::exists(path);
}

bool NativeSystemProbe::isDirectory(const QString& path) const
{
    return QFileInfo(path).isDir();
}

bool NativeSystemProbe::isWritableDirectory(const QString& path) const
{
    QTemporaryFile probeFile(QDir(path).filePath(QStringLiteral(".appmesh-write-XXXXXX")));
    probeFile.setAutoRemove(true);
    return probeFile.open();
}

quint64 NativeSystemProbe::availableBytes(const QString& path, bool* available) const
{
    const QStorageInfo storage(path);
    *available = storage.isValid() && storage.isReady() && storage.bytesAvailable() >= 0;
    if (!*available)
    {
        return 0;
    }
    return static_cast<quint64>(storage.bytesAvailable());
}

SystemChecker::SystemChecker(std::shared_ptr<const ISystemProbe> probe)
    : m_probe(probe ? std::move(probe) : std::make_shared<NativeSystemProbe>())
{
}

QStringList SystemChecker::defaultRequiredToolFiles()
{
    return {
        QStringLiteral("cmake/OCCConfig.cmake"),
        QStringLiteral("Win64/VTK942/bind/vtkCommonCore-9.4d.dll"),
        QStringLiteral("Win64/OCC/bind/TKernel.dll"),
        QStringLiteral("Win64/hdf5/bind/hdf5_D.dll"),
        QStringLiteral("Win64/SARibbon/bind/SARibbonBard.dll"),
        QStringLiteral("Win64/gmsh/gmsh.exe")};
}

RuntimeCheckResult SystemChecker::check(const RuntimeCheckRequest& request) const
{
    RuntimeCheckResult result;

    if (request.toolsDirectory.trimmed().isEmpty())
    {
        result.add(diagnostic(QStringLiteral("ENV-TOOLS-DIR-MISSING"),
                              QStringLiteral("FastCAE Tools directory is not configured."),
                              QStringLiteral("Provide an absolute Tools directory at the application boundary."),
                              request.toolsDirectory));
    }
    else
    {
        result.checkedToolsDirectory = absoluteCleanPath(request.toolsDirectory);
        if (!m_probe->pathExists(result.checkedToolsDirectory) ||
            !m_probe->isDirectory(result.checkedToolsDirectory))
        {
            result.add(diagnostic(QStringLiteral("ENV-TOOLS-DIR-MISSING"),
                                  QStringLiteral("FastCAE Tools directory is unavailable."),
                                  QStringLiteral("The configured path does not exist or is not a directory."),
                                  result.checkedToolsDirectory));
        }
        else
        {
            const auto requiredFiles = request.requiredToolFiles.isEmpty()
                ? defaultRequiredToolFiles()
                : request.requiredToolFiles;
            for (const auto& relativePath : requiredFiles)
            {
                const QString portableRelativePath =
                    QDir::cleanPath(QDir::fromNativeSeparators(relativePath.trimmed()));
                if (portableRelativePath.isEmpty() ||
                    QDir::isAbsolutePath(portableRelativePath) ||
                    portableRelativePath == QStringLiteral("..") ||
                    portableRelativePath.startsWith(QStringLiteral("../")))
                {
                    result.add(diagnostic(QStringLiteral("ENV-DEPENDENCY-PATH-INVALID"),
                                          QStringLiteral("A dependency path escapes the FastCAE Tools directory."),
                                          QStringLiteral("Dependency entries must be non-empty relative paths below Tools."),
                                          relativePath));
                    continue;
                }
                const QString dependencyPath =
                    absoluteCleanPath(QDir(result.checkedToolsDirectory).filePath(portableRelativePath));
                if (!m_probe->pathExists(dependencyPath))
                {
                    result.add(diagnostic(QStringLiteral("ENV-DEPENDENCY-MISSING"),
                                          QStringLiteral("A required FastCAE dependency is missing."),
                                          QStringLiteral("Required Tools entry: %1").arg(portableRelativePath),
                                          dependencyPath));
                }
            }
        }
    }

    if (request.qtDirectory.trimmed().isEmpty())
    {
        result.add(diagnostic(QStringLiteral("ENV-QT-DIR-MISSING"),
                              QStringLiteral("Qt directory is not configured."),
                              QStringLiteral("Provide the Qt 5.14.2 msvc2017_64 root directory."),
                              request.qtDirectory));
    }
    else
    {
        result.checkedQtDirectory = absoluteCleanPath(request.qtDirectory);
        const QString qmakePath = QDir(result.checkedQtDirectory).filePath(QStringLiteral("bin/qmake.exe"));
        if (!m_probe->isDirectory(result.checkedQtDirectory) || !m_probe->pathExists(qmakePath))
        {
            result.add(diagnostic(QStringLiteral("ENV-QT-DIR-MISSING"),
                                  QStringLiteral("Qt 5.14.2 runtime is unavailable."),
                                  QStringLiteral("Expected bin/qmake.exe below the configured Qt root."),
                                  absoluteCleanPath(qmakePath)));
        }
    }

    if (request.workingDirectory.trimmed().isEmpty())
    {
        result.add(diagnostic(QStringLiteral("ENV-WORKDIR-MISSING"),
                              QStringLiteral("Working directory is not configured."),
                              QStringLiteral("Select an existing writable directory."),
                              request.workingDirectory));
        return result;
    }

    result.checkedWorkingDirectory = absoluteCleanPath(request.workingDirectory);
    if (!m_probe->pathExists(result.checkedWorkingDirectory))
    {
        result.add(diagnostic(QStringLiteral("ENV-WORKDIR-MISSING"),
                              QStringLiteral("Working directory does not exist."),
                              QStringLiteral("Create the directory or select another location."),
                              result.checkedWorkingDirectory));
        return result;
    }
    if (!m_probe->isDirectory(result.checkedWorkingDirectory))
    {
        result.add(diagnostic(QStringLiteral("ENV-WORKDIR-NOT-DIRECTORY"),
                              QStringLiteral("Working directory path is not a directory."),
                              QStringLiteral("The configured path refers to a file or unsupported object."),
                              result.checkedWorkingDirectory));
        return result;
    }
    if (!m_probe->isWritableDirectory(result.checkedWorkingDirectory))
    {
        result.add(diagnostic(QStringLiteral("ENV-WORKDIR-NOT-WRITABLE"),
                              QStringLiteral("Working directory is not writable."),
                              QStringLiteral("Grant write access or select another directory."),
                              result.checkedWorkingDirectory));
    }

    bool storageAvailable = false;
    result.availableDiskBytes =
        m_probe->availableBytes(result.checkedWorkingDirectory, &storageAvailable);
    if (!storageAvailable)
    {
        result.add(diagnostic(QStringLiteral("ENV-DISK-UNAVAILABLE"),
                              QStringLiteral("Available disk space could not be determined."),
                              QStringLiteral("The storage volume is not ready or cannot be queried."),
                              result.checkedWorkingDirectory));
    }
    else if (result.availableDiskBytes < request.minimumFreeBytes)
    {
        result.add(diagnostic(QStringLiteral("ENV-DISK-INSUFFICIENT"),
                              QStringLiteral("Working directory has insufficient free space."),
                              QStringLiteral("Required %1 bytes; available %2 bytes.")
                                  .arg(request.minimumFreeBytes)
                                  .arg(result.availableDiskBytes),
                              result.checkedWorkingDirectory));
    }

    return result;
}
}
