#pragma once

#include "ApplicationBoundaries.h"
#include "MeshAPPSettings.h"
#include "SystemChecker.h"

namespace AppMesh::App
{
class AppInitializer;
class ComponentFactory;
class GlobalDataFactory;
class IFastCAERegistrationAdapter;
class MainWindowGenerator;
class PreWindowInitializer;
class PyRegister;
class SignalProcessor;

struct StartupRequest
{
    QStringList arguments;
    RuntimeCheckRequest runtimeCheck;
    QString defaultWorkingDirectory;
};

struct MeshAppResult : AppOperationResult
{
    bool eventLoopEntered = false;
    int exitCode = 0;
};

class MeshApp
{
public:
    MeshApp(MeshAPPSettings& settings,
            SystemChecker& systemChecker,
            GlobalDataFactory& globalData,
            ComponentFactory& components,
            IFastCAERegistrationAdapter& registrationAdapter,
            PyRegister& python,
            MainWindowGenerator& mainWindow,
            PreWindowInitializer& preWindow,
            SignalProcessor& signals,
            ILifecycleBoundary& plugins,
            AppInitializer& initializer,
            IOperatorBoundary& operators,
            ICommandLineBoundary& commandLine,
            IEventLoop& eventLoop,
            IStartupObserver* observer = nullptr);

    MeshAppResult start(const StartupRequest& request);
    MeshAppResult run(const StartupRequest& request);
    AppOperationResult shutdown();

    bool isRunning() const noexcept;
    const AppSettings& activeSettings() const noexcept;

private:
    enum class State
    {
        Idle,
        Starting,
        Running,
        Failed,
        ShuttingDown
    };

    bool appendStageResult(MeshAppResult& target,
                           const QString& stage,
                           const AppOperationResult& stageResult);
    void appendRollbackResult(AppOperationResult& target,
                              const QString& stage,
                              const AppOperationResult& rollbackResult);
    void beginStage(const QString& stage);
    void rolledBack(const QString& stage);
    void rollbackFailedStart(MeshAppResult& result);
    void clearCompletedStages();

    MeshAPPSettings& m_settingsStorage;
    SystemChecker& m_systemChecker;
    GlobalDataFactory& m_globalData;
    ComponentFactory& m_components;
    IFastCAERegistrationAdapter& m_registrationAdapter;
    PyRegister& m_python;
    MainWindowGenerator& m_mainWindow;
    PreWindowInitializer& m_preWindow;
    SignalProcessor& m_signals;
    ILifecycleBoundary& m_plugins;
    AppInitializer& m_initializer;
    IOperatorBoundary& m_operators;
    ICommandLineBoundary& m_commandLine;
    IEventLoop& m_eventLoop;
    IStartupObserver* m_observer = nullptr;

    State m_state = State::Idle;
    ParsedCommandLine m_parsed;
    AppSettings m_activeSettings;
    bool m_settingsLoaded = false;
    bool m_fitkRuntimeInitialized = false;
    bool m_globalDataInitialized = false;
    bool m_componentsInitialized = false;
    bool m_pythonRegistered = false;
    bool m_windowCreated = false;
    bool m_preWindowInitialized = false;
    bool m_pluginsInitialized = false;
    bool m_appInitialized = false;
    bool m_operatorsInitialized = false;
};
}
