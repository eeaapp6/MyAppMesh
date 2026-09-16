#pragma once

#include "AppDiagnostic.h"

#include <QStringList>

#include <functional>

class QCoreApplication;

namespace AppMesh::App
{
using StageAction = std::function<AppOperationResult()>;

class OptionalLifecycleStage
{
public:
    OptionalLifecycleStage(QString name,
                           bool enabled,
                           StageAction startAction = {},
                           StageAction stopAction = {});

    AppOperationResult start();
    AppOperationResult stop();
    bool isActive() const noexcept;
    bool isEnabled() const noexcept;

private:
    QString m_name;
    bool m_enabled = false;
    bool m_active = false;
    StageAction m_startAction;
    StageAction m_stopAction;
};

struct ParsedCommandLine
{
    bool smokeTest = false;
    bool workbench = false;
    QString workingDirectory;
    QStringList inputs;
};

class ICommandLineBoundary
{
public:
    virtual ~ICommandLineBoundary() = default;
    virtual AppOperationResult parse(const QStringList& arguments,
                                     ParsedCommandLine* parsed) = 0;
    virtual AppOperationResult process(const ParsedCommandLine& parsed) = 0;
};

class BasicCommandLineBoundary final : public ICommandLineBoundary
{
public:
    AppOperationResult parse(const QStringList& arguments,
                             ParsedCommandLine* parsed) override;
    AppOperationResult process(const ParsedCommandLine& parsed) override;
};

class ILifecycleBoundary
{
public:
    virtual ~ILifecycleBoundary() = default;
    virtual AppOperationResult initialize() = 0;
    virtual AppOperationResult shutdown() = 0;
};

class DisabledLifecycleBoundary final : public ILifecycleBoundary
{
public:
    explicit DisabledLifecycleBoundary(QString capability);
    AppOperationResult initialize() override;
    AppOperationResult shutdown() override;
    bool isInitialized() const noexcept;

private:
    QString m_capability;
    bool m_initialized = false;
};

class IOperatorBoundary : public ILifecycleBoundary
{
public:
    virtual AppOperationResult stopAccepting() = 0;
    virtual AppOperationResult finishCurrent() = 0;
};

class DisabledOperatorBoundary final : public IOperatorBoundary
{
public:
    AppOperationResult initialize() override;
    AppOperationResult stopAccepting() override;
    AppOperationResult finishCurrent() override;
    AppOperationResult shutdown() override;

private:
    bool m_initialized = false;
};

class IEventLoop
{
public:
    virtual ~IEventLoop() = default;
    virtual int exec() = 0;
};

class QtEventLoop final : public IEventLoop
{
public:
    QtEventLoop(QCoreApplication& application, bool bypass);
    int exec() override;

private:
    QCoreApplication& m_application;
    bool m_bypass = false;
};

class IStartupObserver
{
public:
    virtual ~IStartupObserver() = default;
    virtual void stageStarted(const QString& stage) = 0;
    virtual void stageRolledBack(const QString& stage) = 0;
};
}
