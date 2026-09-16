#include "AppInitializer.h"
#include "ComponentFactory.h"
#include "MainWindowGenerator.h"
#include "MeshApp.h"
#include "PreWindowInitializer.h"
#include "PyRegister.h"
#include "SignalProcessor.h"
#include "TestSupport.h"

#include <QApplication>
#include <QTemporaryDir>
#include <QWidget>

#include <memory>

namespace
{
enum class FailurePoint
{
    None,
    FITKRuntime,
    Environment,
    GlobalData,
    Component,
    Python,
    MainWindow,
    PreWindow,
    Plugins,
    AppInitializer,
    Operators,
    CommandLineProcess
};

AppMesh::App::AppOperationResult failure(const QString& code, const QString& detail)
{
    AppMesh::App::AppOperationResult result;
    result.add({QStringLiteral("test"), code, QStringLiteral("Injected failure."), detail, false, {}, {}});
    return result;
}

class Service final : public AppMesh::App::ManagedService
{
};

AppMesh::App::ServiceCreator serviceCreator()
{
    return [] { return std::unique_ptr<AppMesh::App::ManagedService>(new Service); };
}

class Probe final : public AppMesh::App::ISystemProbe
{
public:
    bool available = true;

    bool pathExists(const QString&) const override { return available; }
    bool isDirectory(const QString&) const override { return available; }
    bool isWritableDirectory(const QString&) const override { return available; }
    quint64 availableBytes(const QString&, bool* storageAvailable) const override
    {
        *storageAvailable = available;
        return available ? 16ULL * 1024ULL * 1024ULL * 1024ULL : 0;
    }
};

class RegistrationAdapter final : public AppMesh::App::IFastCAERegistrationAdapter
{
public:
    FailurePoint failurePoint = FailurePoint::None;

    AppMesh::App::AppOperationResult initializeRuntime() override
    {
        return failurePoint == FailurePoint::FITKRuntime
            ? failure(QStringLiteral("TEST-FITK-RUNTIME-FAILED"),
                      QStringLiteral("injected FITK runtime failure"))
            : AppMesh::App::AppOperationResult{};
    }

    bool registerGlobalData(const QString& key,
                            AppMesh::App::ManagedService&,
                            QString* detail) override
    {
        if (failurePoint == FailurePoint::GlobalData)
        {
            *detail = QStringLiteral("injected global-data failure");
            return false;
        }
        globalKeys.append(key);
        return true;
    }

    void unregisterGlobalData(const QString& key,
                              AppMesh::App::ManagedService&) noexcept override
    {
        globalKeys.removeAll(key);
    }

    bool registerComponent(const QString& key,
                           AppMesh::App::ManagedService&,
                           QString* detail) override
    {
        if (failurePoint == FailurePoint::Component)
        {
            *detail = QStringLiteral("injected component failure");
            return false;
        }
        componentKeys.append(key);
        return true;
    }

    void unregisterComponent(const QString& key,
                             AppMesh::App::ManagedService&) noexcept override
    {
        componentKeys.removeAll(key);
    }

    QStringList globalKeys;
    QStringList componentKeys;
};

class LifecycleBoundary final : public AppMesh::App::ILifecycleBoundary
{
public:
    explicit LifecycleBoundary(FailurePoint point) : m_point(point) {}

    AppMesh::App::AppOperationResult initialize() override
    {
        if (m_point == FailurePoint::Plugins)
        {
            return failure(QStringLiteral("TEST-PLUGINS-FAILED"),
                           QStringLiteral("injected plugin initialization failure"));
        }
        m_initialized = true;
        return {};
    }

    AppMesh::App::AppOperationResult shutdown() override
    {
        m_initialized = false;
        if (failShutdown)
        {
            return failure(QStringLiteral("TEST-PLUGIN-ROLLBACK-FAILED"),
                           QStringLiteral("injected plugin rollback failure"));
        }
        return {};
    }

    bool failShutdown = false;

private:
    FailurePoint m_point;
    bool m_initialized = false;
};

class OperatorBoundary final : public AppMesh::App::IOperatorBoundary
{
public:
    FailurePoint failurePoint = FailurePoint::None;

    AppMesh::App::AppOperationResult initialize() override
    {
        if (failurePoint == FailurePoint::Operators)
        {
            return failure(QStringLiteral("TEST-OPERATORS-FAILED"),
                           QStringLiteral("injected operator registration failure"));
        }
        initialized = true;
        return {};
    }
    AppMesh::App::AppOperationResult stopAccepting() override
    {
        ++stopCount;
        return {};
    }
    AppMesh::App::AppOperationResult finishCurrent() override
    {
        ++finishCount;
        return {};
    }
    AppMesh::App::AppOperationResult shutdown() override
    {
        initialized = false;
        ++shutdownCount;
        return {};
    }

    bool initialized = false;
    int stopCount = 0;
    int finishCount = 0;
    int shutdownCount = 0;
};

class CommandLineBoundary final : public AppMesh::App::ICommandLineBoundary
{
public:
    FailurePoint failurePoint = FailurePoint::None;

    AppMesh::App::AppOperationResult parse(const QStringList& arguments,
                                           AppMesh::App::ParsedCommandLine* parsed) override
    {
        parsed->smokeTest = arguments.contains(QStringLiteral("--smoke-test"));
        return {};
    }
    AppMesh::App::AppOperationResult process(const AppMesh::App::ParsedCommandLine&) override
    {
        return failurePoint == FailurePoint::CommandLineProcess
            ? failure(QStringLiteral("TEST-COMMAND-LINE-FAILED"),
                      QStringLiteral("injected command-line processing failure"))
            : AppMesh::App::AppOperationResult{};
    }
};

class EventLoop final : public AppMesh::App::IEventLoop
{
public:
    int exec() override
    {
        ++entries;
        return 0;
    }
    int entries = 0;
};

class Observer final : public AppMesh::App::IStartupObserver
{
public:
    void stageStarted(const QString& stage) override
    {
        events.append(QStringLiteral("start:") + stage);
    }
    void stageRolledBack(const QString& stage) override
    {
        events.append(QStringLiteral("rollback:") + stage);
    }
    QStringList events;
};

bool hasDiagnostic(const AppMesh::App::AppOperationResult& result,
                   const QString& stage,
                   const QString& code)
{
    for (const auto& diagnostic : result.diagnostics)
    {
        if (diagnostic.stage == stage && diagnostic.code == code)
        {
            return true;
        }
    }
    return false;
}

class Scenario
{
public:
    explicit Scenario(FailurePoint failurePoint)
        : settings(temporaryDirectory.filePath(QStringLiteral("config/appmesh.ini"))),
          probe(std::make_shared<Probe>()),
          checker(probe),
          python(true,
                 action(FailurePoint::Python, QStringLiteral("TEST-PYTHON-FAILED")),
                 successAction()),
          mainWindow([failurePoint] {
              return failurePoint == FailurePoint::MainWindow
                  ? std::unique_ptr<QWidget>()
                  : std::unique_ptr<QWidget>(new QWidget);
          }, false),
          signalProcessor(false),
          preWindow(true,
                    action(FailurePoint::PreWindow, QStringLiteral("TEST-PRE-WINDOW-FAILED")),
                    successAction()),
          plugins(failurePoint),
          initializer(true,
                      action(FailurePoint::AppInitializer,
                             QStringLiteral("TEST-APP-INITIALIZER-FAILED")),
                      successAction()),
          app(settings,
              checker,
              globalData,
              components,
              registration,
              python,
              mainWindow,
              preWindow,
              signalProcessor,
              plugins,
              initializer,
              operators,
              commandLine,
              eventLoop,
              &observer),
          m_failurePoint(failurePoint)
    {
        probe->available = failurePoint != FailurePoint::Environment;
        registration.failurePoint = failurePoint;
        operators.failurePoint = failurePoint;
        commandLine.failurePoint = failurePoint;
        globalData.add({QStringLiteral("runtime"), serviceCreator()});
        components.add({QStringLiteral("application"),
                        QStringList() << QStringLiteral("runtime"),
                        {},
                        serviceCreator()});

        AppMesh::App::AppSettings initial;
        initial.workingDirectory = temporaryDirectory.path();
        settings.save(initial);

        request.arguments = QStringList() << QStringLiteral("APPMesh.exe")
                                          << QStringLiteral("--smoke-test");
        request.defaultWorkingDirectory = temporaryDirectory.path();
        request.runtimeCheck.toolsDirectory = QStringLiteral("tools");
        request.runtimeCheck.qtDirectory = QStringLiteral("qt");
        request.runtimeCheck.requiredToolFiles = QStringList() << QStringLiteral("marker");
        request.runtimeCheck.minimumFreeBytes = 1;
    }

    AppMesh::App::StageAction action(FailurePoint point, const QString& code)
    {
        return [this, point, code] {
            return m_failurePoint == point
                ? failure(code, QStringLiteral("injected startup stage failure"))
                : AppMesh::App::AppOperationResult{};
        };
    }

    static AppMesh::App::StageAction successAction()
    {
        return [] { return AppMesh::App::AppOperationResult{}; };
    }

    QTemporaryDir temporaryDirectory;
    AppMesh::App::MeshAPPSettings settings;
    std::shared_ptr<Probe> probe;
    AppMesh::App::SystemChecker checker;
    AppMesh::App::GlobalDataFactory globalData;
    AppMesh::App::ComponentFactory components;
    RegistrationAdapter registration;
    AppMesh::App::PyRegister python;
    AppMesh::App::MainWindowGenerator mainWindow;
    AppMesh::App::SignalProcessor signalProcessor;
    AppMesh::App::PreWindowInitializer preWindow;
    LifecycleBoundary plugins;
    AppMesh::App::AppInitializer initializer;
    OperatorBoundary operators;
    CommandLineBoundary commandLine;
    EventLoop eventLoop;
    Observer observer;
    AppMesh::App::MeshApp app;
    AppMesh::App::StartupRequest request;

private:
    FailurePoint m_failurePoint;
};
}

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    TestSuite suite;

    Scenario success(FailurePoint::None);
    const auto successResult = success.app.run(success.request);
    const QStringList expectedSuccess = {
        QStringLiteral("start:fitk.application"),
        QStringLiteral("start:command-line.parse"),
        QStringLiteral("start:settings.load"),
        QStringLiteral("start:system.check"),
        QStringLiteral("start:global-data.initialize"),
        QStringLiteral("start:components.initialize"),
        QStringLiteral("start:python.register"),
        QStringLiteral("start:main-window.create"),
        QStringLiteral("start:pre-window.initialize"),
        QStringLiteral("start:plugins.initialize"),
        QStringLiteral("start:application.initialize"),
        QStringLiteral("start:operators.register"),
        QStringLiteral("start:command-line.process"),
        QStringLiteral("start:event-loop.enter"),
        QStringLiteral("start:operations.stop"),
        QStringLiteral("start:tasks.finish"),
        QStringLiteral("start:settings.save"),
        QStringLiteral("rollback:operators.unregister"),
        QStringLiteral("rollback:application.shutdown"),
        QStringLiteral("rollback:plugins.shutdown"),
        QStringLiteral("rollback:pre-window.shutdown"),
        QStringLiteral("rollback:signals.disconnect"),
        QStringLiteral("rollback:main-window.destroy"),
        QStringLiteral("rollback:python.unregister"),
        QStringLiteral("rollback:components.shutdown"),
        QStringLiteral("rollback:global-data.shutdown"),
        QStringLiteral("rollback:fitk.application.shutdown")};
    suite.expect(successResult.succeeded(), QStringLiteral("normal startup and shutdown succeed"));
    suite.expect(successResult.eventLoopEntered && success.eventLoop.entries == 1,
                 QStringLiteral("event loop is entered exactly once after successful startup"));
    suite.expect(success.observer.events == expectedSuccess,
                 QStringLiteral("startup and shutdown order is exact"));
    suite.expect(success.operators.stopCount == 1 && success.operators.finishCount == 1,
                 QStringLiteral("shutdown stops submissions and drains the task boundary"));

    const int eventsAfterFirstShutdown = success.observer.events.size();
    suite.expect(success.app.shutdown().succeeded(), QStringLiteral("repeated shutdown is safe"));
    suite.expect(success.observer.events.size() == eventsAfterFirstShutdown &&
                     success.operators.shutdownCount == 1,
                 QStringLiteral("repeated shutdown performs no duplicate unregistration"));
    const auto restoredSettings = success.settings.load();
    suite.expect(restoredSettings.succeeded() &&
                     restoredSettings.settings.workingDirectory == success.temporaryDirectory.path(),
                 QStringLiteral("startup loads and shutdown saves MeshAPPSettings"));

    Scenario fitkFailure(FailurePoint::FITKRuntime);
    const auto fitkResult = fitkFailure.app.run(fitkFailure.request);
    suite.expect(!fitkResult.eventLoopEntered && fitkFailure.eventLoop.entries == 0 &&
                     fitkFailure.observer.events ==
                         (QStringList() << QStringLiteral("start:fitk.application")),
                 QStringLiteral("FITK runtime failure blocks every later startup stage"));
    suite.expect(hasDiagnostic(fitkResult,
                               QStringLiteral("fitk.application"),
                               QStringLiteral("TEST-FITK-RUNTIME-FAILED")),
                 QStringLiteral("FITK runtime failure retains structured stage and code"));

    Scenario environmentFailure(FailurePoint::Environment);
    const auto environmentResult = environmentFailure.app.run(environmentFailure.request);
    suite.expect(!environmentResult.eventLoopEntered && environmentFailure.eventLoop.entries == 0,
                 QStringLiteral("environment failure never enters the event loop"));
    suite.expect(environmentFailure.registration.globalKeys.isEmpty() &&
                     environmentFailure.registration.componentKeys.isEmpty(),
                 QStringLiteral("environment failure creates no global data or components"));
    suite.expect(hasDiagnostic(environmentResult,
                               QStringLiteral("system.check"),
                               QStringLiteral("ENV-TOOLS-DIR-MISSING")),
                 QStringLiteral("environment failure retains stage and structured code"));

    Scenario globalFailure(FailurePoint::GlobalData);
    const auto globalResult = globalFailure.app.run(globalFailure.request);
    suite.expect(!globalResult.eventLoopEntered && globalFailure.registration.componentKeys.isEmpty(),
                 QStringLiteral("global-data failure prevents component creation and event loop entry"));
    suite.expect(hasDiagnostic(globalResult,
                               QStringLiteral("global-data.initialize"),
                               QStringLiteral("APP-GLOBAL-DATA-REGISTER-FAILED")),
                 QStringLiteral("global-data failure retains original stage and code"));

    Scenario componentFailure(FailurePoint::Component);
    const auto componentResult = componentFailure.app.run(componentFailure.request);
    suite.expect(!componentResult.eventLoopEntered && componentFailure.registration.globalKeys.isEmpty(),
                 QStringLiteral("component failure rolls back global data"));
    suite.expect(componentFailure.observer.events.mid(componentFailure.observer.events.size() - 3) ==
                     (QStringList() << QStringLiteral("rollback:global-data.shutdown")
                                    << QStringLiteral("rollback:settings.save")
                                    << QStringLiteral("rollback:fitk.application.shutdown")),
                 QStringLiteral("component failure rollback is reverse ordered"));

    Scenario pythonFailure(FailurePoint::Python);
    const auto pythonResult = pythonFailure.app.run(pythonFailure.request);
    suite.expect(!pythonResult.eventLoopEntered && pythonFailure.registration.globalKeys.isEmpty() &&
                     pythonFailure.registration.componentKeys.isEmpty(),
                 QStringLiteral("Python registration failure rolls back components and global data"));
    suite.expect(hasDiagnostic(pythonResult,
                               QStringLiteral("python.register"),
                               QStringLiteral("TEST-PYTHON-FAILED")),
                 QStringLiteral("Python failure is structured and stage-specific"));

    Scenario windowFailure(FailurePoint::MainWindow);
    const auto windowResult = windowFailure.app.run(windowFailure.request);
    suite.expect(!windowResult.eventLoopEntered && windowFailure.eventLoop.entries == 0,
                 QStringLiteral("main-window creation failure prevents event loop entry"));
    suite.expect(hasDiagnostic(windowResult,
                               QStringLiteral("main-window.create"),
                               QStringLiteral("APP-MAIN-WINDOW-CREATE-FAILED")),
                 QStringLiteral("main-window failure retains structured diagnostic"));

    Scenario preWindowFailure(FailurePoint::PreWindow);
    const auto preWindowResult = preWindowFailure.app.run(preWindowFailure.request);
    suite.expect(!preWindowResult.eventLoopEntered,
                 QStringLiteral("PreWindowInitializer failure prevents event loop entry"));
    suite.expect(!preWindowFailure.signalProcessor.isConnected(),
                 QStringLiteral("PreWindowInitializer failure disconnects partially connected signals"));

    Scenario initializerFailure(FailurePoint::AppInitializer);
    initializerFailure.plugins.failShutdown = true;
    const auto initializerResult = initializerFailure.app.run(initializerFailure.request);
    suite.expect(!initializerResult.eventLoopEntered,
                 QStringLiteral("AppInitializer failure prevents event loop entry"));
    suite.expect(initializerResult.diagnostics.size() >= 2 &&
                     initializerResult.diagnostics.first().code ==
                         QStringLiteral("TEST-APP-INITIALIZER-FAILED") &&
                     hasDiagnostic(initializerResult,
                                   QStringLiteral("plugins.shutdown"),
                                   QStringLiteral("TEST-PLUGIN-ROLLBACK-FAILED")),
                 QStringLiteral("rollback diagnostics append without replacing the original failure"));
    suite.expect(initializerFailure.observer.events.contains(
                     QStringLiteral("rollback:plugins.shutdown")) &&
                     initializerFailure.observer.events.indexOf(
                         QStringLiteral("rollback:plugins.shutdown")) <
                         initializerFailure.observer.events.indexOf(
                             QStringLiteral("rollback:global-data.shutdown")),
                 QStringLiteral("AppInitializer failure rolls completed stages back in reverse order"));
    suite.expect(initializerFailure.app.shutdown().succeeded(),
                 QStringLiteral("shutdown after failed startup is safe"));
    const auto repeatedStart = initializerFailure.app.start(initializerFailure.request);
    suite.expect(hasDiagnostic(repeatedStart,
                               QStringLiteral("startup"),
                               QStringLiteral("APP-STARTUP-STATE-INVALID")),
                 QStringLiteral("repeated startup after failure is safely rejected"));

    Scenario pluginFailure(FailurePoint::Plugins);
    const auto pluginResult = pluginFailure.app.run(pluginFailure.request);
    suite.expect(!pluginResult.eventLoopEntered && pluginFailure.eventLoop.entries == 0 &&
                     hasDiagnostic(pluginResult,
                                   QStringLiteral("plugins.initialize"),
                                   QStringLiteral("TEST-PLUGINS-FAILED")),
                 QStringLiteral("plugin-manager failure blocks application initialization and event loop"));

    Scenario operatorFailure(FailurePoint::Operators);
    const auto operatorResult = operatorFailure.app.run(operatorFailure.request);
    suite.expect(!operatorResult.eventLoopEntered && operatorFailure.eventLoop.entries == 0 &&
                     hasDiagnostic(operatorResult,
                                   QStringLiteral("operators.register"),
                                   QStringLiteral("TEST-OPERATORS-FAILED")),
                 QStringLiteral("operator-repository failure rolls back prior stages"));

    Scenario commandFailure(FailurePoint::CommandLineProcess);
    const auto commandResult = commandFailure.app.run(commandFailure.request);
    suite.expect(!commandResult.eventLoopEntered && commandFailure.eventLoop.entries == 0 &&
                     hasDiagnostic(commandResult,
                                   QStringLiteral("command-line.process"),
                                   QStringLiteral("TEST-COMMAND-LINE-FAILED")),
                 QStringLiteral("command-line failure never enters the event loop"));

    return suite.result();
}
