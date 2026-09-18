#include "AppInitializer.h"
#include "ApplicationBoundaries.h"
#include "ComponentFactory.h"
#include "FITKComponentMapping.h"
#include "FITKFastCAERegistrationAdapter.h"
#include "GlobalDataFactory.h"
#include "MainWindowGenerator.h"
#include "MeshAPPSettings.h"
#include "MeshApp.h"
#include "PreWindowInitializer.h"
#include "PyRegister.h"
#include "SignalProcessor.h"
#include "SystemChecker.h"
#include "gui/GUIFrame/MainWindow.h"
#include "gui/GUIWidget/ConsoleWidget.h"
#include "gui/GUIWidget/ModelTree.h"
#include "io/GeometryIO/GeometryIO.h"
#include "model/ModelData/ApplicationRuntime.h"
#include "model/ModelData/GeometryManager.h"
#include "model/ModelData/MeshManager.h"
#include "operators/OperatorsGUI/GeometryImportController.h"
#include "operators/OperatorsModel/FITKTaskExecutor.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreAPI.h"

#include <QApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QPointer>

#include <hdf5.h>

#include <iostream>
#include <memory>
#include <stdexcept>

namespace
{
class ApplicationComponent final : public AppMesh::App::ManagedService
{
};

class StreamStartupObserver final : public AppMesh::App::IStartupObserver
{
public:
    void stageStarted(const QString& stage) override
    {
        std::cout << "APPMesh lifecycle start:" << stage.toStdString() << std::endl;
    }

    void stageRolledBack(const QString& stage) override
    {
        std::cout << "APPMesh lifecycle rollback:" << stage.toStdString() << std::endl;
    }
};

bool hasRawArgument(int argc, char* argv[], const QString& expected)
{
    for (int index = 1; index < argc; ++index)
    {
        if (QString::fromLocal8Bit(argv[index]) == expected)
        {
            return true;
        }
    }
    return false;
}

void printDiagnostics(const AppMesh::App::AppOperationResult& result)
{
    for (const auto& diagnostic : result.diagnostics)
    {
        std::cerr << diagnostic.stage.toStdString() << " | "
                  << diagnostic.code.toStdString() << " | "
                  << diagnostic.message.toStdString() << " | "
                  << diagnostic.detail.toStdString() << std::endl;
    }
}

int verifyHdf5Runtime()
{
    unsigned int major = 0;
    unsigned int minor = 0;
    unsigned int release = 0;
    if (H5get_libversion(&major, &minor, &release) < 0)
    {
        std::cerr << "HDF5 runtime query failed" << std::endl;
        return 2;
    }
    if (major != H5_VERS_MAJOR || minor != H5_VERS_MINOR)
    {
        std::cerr << "HDF5 header/runtime mismatch" << std::endl;
        return 3;
    }

    std::cout << "APPMesh bootstrap OK; config=" << APPMESH_BUILD_CONFIG
              << "; Qt=" << qVersion()
              << "; HDF5=" << major << '.' << minor << '.' << release
              << "; lifecycle=MeshApp"
              << "; adapter=FITKFastCAERegistrationAdapter"
              << "; FITKAppFramework=" << FITKAppFrameworkVersion
              << "; FITKCore=" << FITKCoreVersion << std::endl;
    return 0;
}
}

int main(int argc, char* argv[])
{
    const bool smokeTest = hasRawArgument(argc, argv, QStringLiteral("--smoke-test"));
    if (smokeTest && qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM"))
    {
        qputenv("QT_QPA_PLATFORM", QByteArrayLiteral("offscreen"));
    }

    AppFrame::FITKApplication application(argc, argv);
    QApplication::setApplicationName(QStringLiteral("APPMesh"));
    QApplication::setApplicationVersion(QStringLiteral(APPMESH_VERSION));

    QTemporaryDir smokeSettingsDirectory;
    const QString settingsPath = smokeTest
        ? smokeSettingsDirectory.filePath(QStringLiteral("appmesh-smoke.ini"))
        : QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation))
              .filePath(QStringLiteral("appmesh.ini"));

    AppMesh::App::MeshAPPSettings settings(settingsPath);
    AppMesh::App::SystemChecker systemChecker;
    AppMesh::App::GlobalDataFactory globalData;
    AppMesh::App::ComponentFactory components;
    AppMesh::App::FITKFastCAERegistrationAdapter registrationAdapter(application);

    const auto mappingValidation = AppMesh::App::validateFITKComponentMappings();
    if (!mappingValidation.succeeded())
    {
        printDiagnostics(mappingValidation);
        return 1;
    }

    const auto* globalMapping = AppMesh::App::fitkMappingForType(QStringLiteral("FITKGlobalData"));
    const auto* componentMapping =
        AppMesh::App::fitkMappingForType(QStringLiteral("FITKComponentManager"));
    if (!globalMapping || !componentMapping)
    {
        std::cerr << "Required FITK component mapping is missing" << std::endl;
        return 1;
    }

    auto result = globalData.add(
        {globalMapping->registrationKey,
         [] {
             return std::unique_ptr<AppMesh::App::ManagedService>(
                 new AppMesh::Model::ApplicationRuntime);
         }});
    if (!result.succeeded())
    {
        printDiagnostics(result);
        return 1;
    }
    result = components.add(
        {componentMapping->registrationKey,
         QStringList() << globalMapping->registrationKey,
         {},
         [] { return std::unique_ptr<AppMesh::App::ManagedService>(new ApplicationComponent); }});
    if (!result.succeeded())
    {
        printDiagnostics(result);
        return 1;
    }

    AppMesh::App::PyRegister python(false);
    std::unique_ptr<AppMesh::Model::GeometryManager> geometryManager;
    std::unique_ptr<AppMesh::Model::MeshManager> meshManager;
    std::unique_ptr<AppMesh::OperatorsModel::TaskService> taskService;
    auto geometryRepository = std::make_shared<AppMesh::GeometryIO::FITKGeometryRepository>();
    auto geometryReaders = std::make_shared<AppMesh::OperatorsModel::GeometryReaderRegistry>();
    const auto readerRegistration =
        AppMesh::GeometryIO::registerFirstReleaseGeometryReaders(*geometryReaders,
                                                                  geometryRepository);
    if (!readerRegistration.succeeded())
    {
        printDiagnostics(readerRegistration);
        return 1;
    }
    const QString runtimeKey = globalMapping->registrationKey;
    AppMesh::App::MainWindowGenerator mainWindow(
        [&globalData, &geometryManager, &meshManager, &taskService,
         geometryReaders, geometryRepository, runtimeKey] {
            auto* runtime = dynamic_cast<AppMesh::Model::ApplicationRuntime*>(
                globalData.instance(runtimeKey));
            if (!runtime)
            {
                throw std::runtime_error(
                    "ApplicationRuntime is unavailable during GUI composition.");
            }

            // A recreated window must release the previous typed managers in
            // reverse dependency order after its widgets have already gone.
            if (taskService)
            {
                taskService->drain();
            }
            taskService.reset();
            meshManager.reset();
            geometryManager.reset();

            auto geometry = std::make_unique<AppMesh::Model::GeometryManager>(*runtime);
            const std::weak_ptr<AppMesh::GeometryIO::FITKGeometryRepository> weakRepository(
                geometryRepository);
            geometry->setExternalResourceReleaser(
                [weakRepository](const AppMesh::Model::GeometryObject& object) {
                    if (object.adapter.providerKey == QStringLiteral("fitk.occ"))
                    {
                        if (const auto repository = weakRepository.lock())
                            repository->release(object.adapter.modelKey);
                    }
                });
            auto mesh = std::make_unique<AppMesh::Model::MeshManager>(*runtime, *geometry);
            auto window = std::make_unique<AppMesh::GUIFrame::MainWindow>();
            auto tree = std::make_unique<AppMesh::Gui::ModelTree>();
            auto console = std::make_unique<AppMesh::Gui::ConsoleWidget>();
            const QPointer<AppMesh::Gui::ConsoleWidget> consoleGuard(console.get());
            tree->setDiagnosticHandler(
                [consoleGuard](const AppMesh::Common::Diagnostic& diagnostic) {
                    if (consoleGuard)
                    {
                        consoleGuard->appendDiagnostic(diagnostic);
                    }
                });
            const auto bindResult = tree->bind(runtime, geometry.get(), mesh.get());
            if (!bindResult.succeeded() || !window->setModelTreeWidget(tree.get()))
            {
                throw std::runtime_error("The main-window widgets could not be assembled.");
            }
            tree.release();
            if (!window->setConsoleWidget(console.get()))
            {
                throw std::runtime_error("The console widget could not be assembled.");
            }
            console.release();

            auto executor = std::make_shared<AppMesh::OperatorsModel::FITKTaskExecutor>();
            auto tasks = std::make_unique<AppMesh::OperatorsModel::TaskService>(executor);
            auto importOperator =
                std::make_shared<AppMesh::OperatorsModel::ImportGeometryOperator>(*geometry,
                                                                                  geometryReaders);
            new AppMesh::OperatorsGUI::GeometryImportController(*tasks,
                                                                 importOperator,
                                                                 consoleGuard,
                                                                 static_cast<AppMesh::Gui::ModelTree*>(
                                                                     window->modelTreeWidget()),
                                                                 {},
                                                                 window.get());

            geometryManager = std::move(geometry);
            meshManager = std::move(mesh);
            taskService = std::move(tasks);
            return std::unique_ptr<QWidget>(window.release());
        },
        !smokeTest,
        [&taskService, &meshManager, &geometryManager] {
            if (taskService)
            {
                taskService->drain();
            }
            taskService.reset();
            meshManager.reset();
            geometryManager.reset();
            return AppMesh::App::AppOperationResult{};
        });
    AppMesh::App::SignalProcessor signalProcessor(false);
    AppMesh::App::PreWindowInitializer preWindow(false);
    AppMesh::App::DisabledLifecycleBoundary plugins(QStringLiteral("FastCAE plugins"));
    AppMesh::App::AppInitializer initializer(false);
    AppMesh::App::TaskServiceOperatorBoundary operators(
        [&taskService] { return taskService.get(); });
    AppMesh::App::BasicCommandLineBoundary commandLine;
    AppMesh::App::QtEventLoop eventLoop(application, smokeTest);
    StreamStartupObserver observer;

    AppMesh::App::MeshApp meshApp(settings,
                                  systemChecker,
                                  globalData,
                                  components,
                                  registrationAdapter,
                                  python,
                                  mainWindow,
                                  preWindow,
                                  signalProcessor,
                                  plugins,
                                  initializer,
                                  operators,
                                  commandLine,
                                  eventLoop,
                                  &observer);

    AppMesh::App::StartupRequest request;
    request.arguments = application.arguments();
    request.defaultWorkingDirectory = QDir::currentPath();
    request.runtimeCheck.toolsDirectory = QStringLiteral(APPMESH_TOOLS_DIR);
    request.runtimeCheck.qtDirectory = QLibraryInfo::location(QLibraryInfo::PrefixPath);

    const auto runResult = meshApp.run(request);
    if (!runResult.succeeded())
    {
        printDiagnostics(runResult);
        return runResult.exitCode == 0 ? 1 : runResult.exitCode;
    }

    if (smokeTest)
    {
        return verifyHdf5Runtime();
    }
    return runResult.exitCode;
}
