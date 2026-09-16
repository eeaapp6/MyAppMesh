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

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreAPI.h"

#include <QApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QStandardPaths>
#include <QTemporaryDir>

#include <hdf5.h>

#include <iostream>
#include <memory>

namespace
{
class RuntimeService final : public AppMesh::App::ManagedService
{
};

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
         [] { return std::unique_ptr<AppMesh::App::ManagedService>(new RuntimeService); }});
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
    AppMesh::App::MainWindowGenerator mainWindow({}, !smokeTest);
    AppMesh::App::SignalProcessor signalProcessor(false);
    AppMesh::App::PreWindowInitializer preWindow(false);
    AppMesh::App::DisabledLifecycleBoundary plugins(QStringLiteral("FastCAE plugins"));
    AppMesh::App::AppInitializer initializer(false);
    AppMesh::App::DisabledOperatorBoundary operators;
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
