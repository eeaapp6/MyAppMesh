#pragma once

#include "AppDiagnostic.h"

#include <QStringList>

#include <memory>

namespace AppMesh::App
{
struct RuntimeCheckRequest
{
    QString toolsDirectory;
    QString qtDirectory;
    QString workingDirectory;
    QStringList requiredToolFiles;
    quint64 minimumFreeBytes = 1024ULL * 1024ULL * 1024ULL;
};

struct RuntimeCheckResult : AppOperationResult
{
    QString checkedToolsDirectory;
    QString checkedQtDirectory;
    QString checkedWorkingDirectory;
    quint64 availableDiskBytes = 0;
};

class ISystemProbe
{
public:
    virtual ~ISystemProbe() = default;
    virtual bool pathExists(const QString& path) const = 0;
    virtual bool isDirectory(const QString& path) const = 0;
    virtual bool isWritableDirectory(const QString& path) const = 0;
    virtual quint64 availableBytes(const QString& path, bool* available) const = 0;
};

class NativeSystemProbe final : public ISystemProbe
{
public:
    bool pathExists(const QString& path) const override;
    bool isDirectory(const QString& path) const override;
    bool isWritableDirectory(const QString& path) const override;
    quint64 availableBytes(const QString& path, bool* available) const override;
};

// The result can be flattened to QStringList when passed to
// AppFrame::FITKAbstractSysChecker::check() by the T004 startup adapter.
class SystemChecker
{
public:
    explicit SystemChecker(std::shared_ptr<const ISystemProbe> probe = {});

    RuntimeCheckResult check(const RuntimeCheckRequest& request) const;
    static QStringList defaultRequiredToolFiles();

private:
    std::shared_ptr<const ISystemProbe> m_probe;
};
}
