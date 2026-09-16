#include "MeshApp.h"

#include "AppInitializer.h"
#include "ComponentFactory.h"
#include "GlobalDataFactory.h"
#include "MainWindowGenerator.h"
#include "PreWindowInitializer.h"
#include "PyRegister.h"
#include "SignalProcessor.h"

namespace AppMesh::App
{
namespace
{
AppDiagnostic stateDiagnostic(const QString& code,
                              const QString& message,
                              const QString& detail,
                              const QString& stage)
{
    return {QStringLiteral("application"), code, message, detail, false, QString(), stage};
}
}

MeshApp::MeshApp(MeshAPPSettings& settings,
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
                 IStartupObserver* observer)
    : m_settingsStorage(settings),
      m_systemChecker(systemChecker),
      m_globalData(globalData),
      m_components(components),
      m_registrationAdapter(registrationAdapter),
      m_python(python),
      m_mainWindow(mainWindow),
      m_preWindow(preWindow),
      m_signals(signals),
      m_plugins(plugins),
      m_initializer(initializer),
      m_operators(operators),
      m_commandLine(commandLine),
      m_eventLoop(eventLoop),
      m_observer(observer)
{
}

void MeshApp::beginStage(const QString& stage)
{
    if (m_observer)
    {
        m_observer->stageStarted(stage);
    }
}

void MeshApp::rolledBack(const QString& stage)
{
    if (m_observer)
    {
        m_observer->stageRolledBack(stage);
    }
}

bool MeshApp::appendStageResult(MeshAppResult& target,
                                const QString& stage,
                                const AppOperationResult& stageResult)
{
    for (auto diagnostic : stageResult.diagnostics)
    {
        if (diagnostic.stage.isEmpty())
        {
            diagnostic.stage = stage;
        }
        target.add(std::move(diagnostic));
    }
    return stageResult.succeeded();
}

void MeshApp::appendRollbackResult(AppOperationResult& target,
                                   const QString& stage,
                                   const AppOperationResult& rollbackResult)
{
    for (auto diagnostic : rollbackResult.diagnostics)
    {
        if (diagnostic.stage.isEmpty())
        {
            diagnostic.stage = stage;
        }
        target.add(std::move(diagnostic));
    }
}

void MeshApp::clearCompletedStages()
{
    m_settingsLoaded = false;
    m_fitkRuntimeInitialized = false;
    m_globalDataInitialized = false;
    m_componentsInitialized = false;
    m_pythonRegistered = false;
    m_windowCreated = false;
    m_preWindowInitialized = false;
    m_pluginsInitialized = false;
    m_appInitialized = false;
    m_operatorsInitialized = false;
}

MeshAppResult MeshApp::start(const StartupRequest& request)
{
    MeshAppResult result;
    if (m_state != State::Idle)
    {
        result.add(stateDiagnostic(QStringLiteral("APP-STARTUP-STATE-INVALID"),
                                   QStringLiteral("Application startup cannot be repeated in the current state."),
                                   QStringLiteral("Shutdown a running application before starting it again."),
                                   QStringLiteral("startup")));
        return result;
    }

    clearCompletedStages();
    m_state = State::Starting;

    beginStage(QStringLiteral("fitk.application"));
    if (!appendStageResult(result,
                           QStringLiteral("fitk.application"),
                           m_registrationAdapter.initializeRuntime()))
    {
        rollbackFailedStart(result);
        return result;
    }
    m_fitkRuntimeInitialized = true;

    beginStage(QStringLiteral("command-line.parse"));
    if (!appendStageResult(result,
                           QStringLiteral("command-line.parse"),
                           m_commandLine.parse(request.arguments, &m_parsed)))
    {
        rollbackFailedStart(result);
        return result;
    }

    beginStage(QStringLiteral("settings.load"));
    const auto loaded = m_settingsStorage.load();
    if (!appendStageResult(result, QStringLiteral("settings.load"), loaded))
    {
        rollbackFailedStart(result);
        return result;
    }
    m_activeSettings = loaded.settings;
    if (m_activeSettings.workingDirectory.trimmed().isEmpty())
    {
        m_activeSettings.workingDirectory = request.defaultWorkingDirectory;
    }
    if (!m_parsed.workingDirectory.trimmed().isEmpty())
    {
        m_activeSettings.workingDirectory = m_parsed.workingDirectory;
    }
    m_settingsLoaded = true;

    RuntimeCheckRequest runtimeCheck = request.runtimeCheck;
    runtimeCheck.workingDirectory = m_activeSettings.workingDirectory;
    beginStage(QStringLiteral("system.check"));
    if (!appendStageResult(result,
                           QStringLiteral("system.check"),
                           m_systemChecker.check(runtimeCheck)))
    {
        rollbackFailedStart(result);
        return result;
    }

    beginStage(QStringLiteral("global-data.initialize"));
    if (!appendStageResult(result,
                           QStringLiteral("global-data.initialize"),
                           m_globalData.initialize(m_registrationAdapter)))
    {
        rollbackFailedStart(result);
        return result;
    }
    m_globalDataInitialized = true;

    beginStage(QStringLiteral("components.initialize"));
    if (!appendStageResult(result,
                           QStringLiteral("components.initialize"),
                           m_components.initialize(m_globalData, m_registrationAdapter)))
    {
        rollbackFailedStart(result);
        return result;
    }
    m_componentsInitialized = true;

    beginStage(QStringLiteral("python.register"));
    if (!appendStageResult(result,
                           QStringLiteral("python.register"),
                           m_python.registerBindings()))
    {
        rollbackFailedStart(result);
        return result;
    }
    m_pythonRegistered = true;

    beginStage(QStringLiteral("main-window.create"));
    if (!appendStageResult(result,
                           QStringLiteral("main-window.create"),
                           m_mainWindow.create()))
    {
        rollbackFailedStart(result);
        return result;
    }
    m_windowCreated = true;

    beginStage(QStringLiteral("pre-window.initialize"));
    if (!appendStageResult(result,
                           QStringLiteral("pre-window.initialize"),
                           m_preWindow.initialize(m_signals)))
    {
        rollbackFailedStart(result);
        return result;
    }
    m_preWindowInitialized = true;

    beginStage(QStringLiteral("plugins.initialize"));
    if (!appendStageResult(result,
                           QStringLiteral("plugins.initialize"),
                           m_plugins.initialize()))
    {
        rollbackFailedStart(result);
        return result;
    }
    m_pluginsInitialized = true;

    beginStage(QStringLiteral("application.initialize"));
    if (!appendStageResult(result,
                           QStringLiteral("application.initialize"),
                           m_initializer.initialize()))
    {
        rollbackFailedStart(result);
        return result;
    }
    m_appInitialized = true;

    beginStage(QStringLiteral("operators.register"));
    if (!appendStageResult(result,
                           QStringLiteral("operators.register"),
                           m_operators.initialize()))
    {
        rollbackFailedStart(result);
        return result;
    }
    m_operatorsInitialized = true;

    beginStage(QStringLiteral("command-line.process"));
    if (!appendStageResult(result,
                           QStringLiteral("command-line.process"),
                           m_commandLine.process(m_parsed)))
    {
        rollbackFailedStart(result);
        return result;
    }

    m_state = State::Running;
    return result;
}

MeshAppResult MeshApp::run(const StartupRequest& request)
{
    MeshAppResult result = start(request);
    if (!result.succeeded())
    {
        return result;
    }

    beginStage(QStringLiteral("event-loop.enter"));
    result.eventLoopEntered = true;
    result.exitCode = m_eventLoop.exec();

    const auto shutdownResult = shutdown();
    for (const auto& diagnostic : shutdownResult.diagnostics)
    {
        result.add(diagnostic);
    }
    if (!shutdownResult.succeeded() && result.exitCode == 0)
    {
        result.exitCode = 1;
    }
    return result;
}

void MeshApp::rollbackFailedStart(MeshAppResult& result)
{
    if (m_operatorsInitialized)
    {
        rolledBack(QStringLiteral("operators.unregister"));
        appendRollbackResult(result, QStringLiteral("operators.unregister"), m_operators.shutdown());
    }
    if (m_appInitialized)
    {
        rolledBack(QStringLiteral("application.shutdown"));
        appendRollbackResult(result, QStringLiteral("application.shutdown"), m_initializer.shutdown());
    }
    if (m_pluginsInitialized)
    {
        rolledBack(QStringLiteral("plugins.shutdown"));
        appendRollbackResult(result, QStringLiteral("plugins.shutdown"), m_plugins.shutdown());
    }
    if (m_preWindowInitialized)
    {
        rolledBack(QStringLiteral("pre-window.shutdown"));
        appendRollbackResult(result, QStringLiteral("pre-window.shutdown"), m_preWindow.shutdown());
        if (m_signals.isConnected())
        {
            rolledBack(QStringLiteral("signals.disconnect"));
            appendRollbackResult(result,
                                 QStringLiteral("signals.disconnect"),
                                 m_signals.disconnectSignals());
        }
    }
    if (m_windowCreated)
    {
        rolledBack(QStringLiteral("main-window.destroy"));
        appendRollbackResult(result, QStringLiteral("main-window.destroy"), m_mainWindow.destroy());
    }
    if (m_pythonRegistered)
    {
        rolledBack(QStringLiteral("python.unregister"));
        appendRollbackResult(result, QStringLiteral("python.unregister"), m_python.unregisterBindings());
    }
    if (m_componentsInitialized)
    {
        rolledBack(QStringLiteral("components.shutdown"));
        m_components.shutdown(m_registrationAdapter);
    }
    if (m_globalDataInitialized)
    {
        rolledBack(QStringLiteral("global-data.shutdown"));
        m_globalData.shutdown(m_registrationAdapter);
    }
    if (m_settingsLoaded)
    {
        rolledBack(QStringLiteral("settings.save"));
        appendRollbackResult(result,
                             QStringLiteral("settings.save"),
                             m_settingsStorage.save(m_activeSettings));
    }
    if (m_fitkRuntimeInitialized)
    {
        rolledBack(QStringLiteral("fitk.application.shutdown"));
        appendRollbackResult(result,
                             QStringLiteral("fitk.application.shutdown"),
                             m_registrationAdapter.shutdownRuntime());
    }
    clearCompletedStages();
    m_state = State::Failed;
}

AppOperationResult MeshApp::shutdown()
{
    AppOperationResult result;
    if (m_state != State::Running)
    {
        return result;
    }
    m_state = State::ShuttingDown;

    beginStage(QStringLiteral("operations.stop"));
    appendRollbackResult(result, QStringLiteral("operations.stop"), m_operators.stopAccepting());
    appendRollbackResult(result, QStringLiteral("operations.stop"), m_signals.stopAccepting());

    beginStage(QStringLiteral("tasks.finish"));
    appendRollbackResult(result, QStringLiteral("tasks.finish"), m_operators.finishCurrent());
    appendRollbackResult(result, QStringLiteral("tasks.finish"), m_signals.finishCurrent());

    if (m_settingsLoaded)
    {
        beginStage(QStringLiteral("settings.save"));
        appendRollbackResult(result,
                             QStringLiteral("settings.save"),
                             m_settingsStorage.save(m_activeSettings));
    }
    if (m_operatorsInitialized)
    {
        rolledBack(QStringLiteral("operators.unregister"));
        appendRollbackResult(result, QStringLiteral("operators.unregister"), m_operators.shutdown());
    }
    if (m_appInitialized)
    {
        rolledBack(QStringLiteral("application.shutdown"));
        appendRollbackResult(result, QStringLiteral("application.shutdown"), m_initializer.shutdown());
    }
    if (m_pluginsInitialized)
    {
        rolledBack(QStringLiteral("plugins.shutdown"));
        appendRollbackResult(result, QStringLiteral("plugins.shutdown"), m_plugins.shutdown());
    }
    if (m_preWindowInitialized)
    {
        rolledBack(QStringLiteral("pre-window.shutdown"));
        appendRollbackResult(result, QStringLiteral("pre-window.shutdown"), m_preWindow.shutdown());
        if (m_signals.isConnected())
        {
            rolledBack(QStringLiteral("signals.disconnect"));
            appendRollbackResult(result,
                                 QStringLiteral("signals.disconnect"),
                                 m_signals.disconnectSignals());
        }
    }
    if (m_windowCreated)
    {
        rolledBack(QStringLiteral("main-window.destroy"));
        appendRollbackResult(result, QStringLiteral("main-window.destroy"), m_mainWindow.destroy());
    }
    if (m_pythonRegistered)
    {
        rolledBack(QStringLiteral("python.unregister"));
        appendRollbackResult(result, QStringLiteral("python.unregister"), m_python.unregisterBindings());
    }
    if (m_componentsInitialized)
    {
        rolledBack(QStringLiteral("components.shutdown"));
        m_components.shutdown(m_registrationAdapter);
    }
    if (m_globalDataInitialized)
    {
        rolledBack(QStringLiteral("global-data.shutdown"));
        m_globalData.shutdown(m_registrationAdapter);
    }
    if (m_fitkRuntimeInitialized)
    {
        rolledBack(QStringLiteral("fitk.application.shutdown"));
        appendRollbackResult(result,
                             QStringLiteral("fitk.application.shutdown"),
                             m_registrationAdapter.shutdownRuntime());
    }

    clearCompletedStages();
    m_state = State::Idle;
    return result;
}

bool MeshApp::isRunning() const noexcept
{
    return m_state == State::Running;
}

const AppSettings& MeshApp::activeSettings() const noexcept
{
    return m_activeSettings;
}
}
