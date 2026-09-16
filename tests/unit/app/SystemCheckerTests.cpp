#include "SystemChecker.h"
#include "TestSupport.h"

#include <QCoreApplication>
#include <QTemporaryDir>

#include <memory>

namespace
{
class ControlledProbe final : public AppMesh::App::ISystemProbe
{
public:
    bool pathExists(const QString& path) const override
    {
        return !path.endsWith(QStringLiteral("missing.dll"));
    }

    bool isDirectory(const QString&) const override
    {
        return directory;
    }

    bool isWritableDirectory(const QString&) const override
    {
        return writable;
    }

    quint64 availableBytes(const QString&, bool* available) const override
    {
        *available = storageAvailable;
        return freeBytes;
    }

    bool directory = true;
    bool writable = true;
    bool storageAvailable = true;
    quint64 freeBytes = 4096;
};

bool hasCode(const AppMesh::App::RuntimeCheckResult& result, const QString& code)
{
    for (const auto& diagnostic : result.diagnostics)
    {
        if (diagnostic.code == code)
        {
            return true;
        }
    }
    return false;
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;

    QTemporaryDir workDirectory;
    AppMesh::App::RuntimeCheckRequest nativeRequest;
    nativeRequest.toolsDirectory = QStringLiteral(APPMESH_TEST_TOOLS_DIR);
    nativeRequest.qtDirectory = QStringLiteral(APPMESH_TEST_QT_ROOT);
    nativeRequest.workingDirectory = workDirectory.path();
    nativeRequest.minimumFreeBytes = 1;

    AppMesh::App::SystemChecker nativeChecker;
    const QStringList defaultDependencies =
        AppMesh::App::SystemChecker::defaultRequiredToolFiles();
    const QStringList expectedDebugDependencies =
        QStringList() << QStringLiteral("cmake/OCCConfig.cmake")
                      << QStringLiteral("Win64/VTK942/bind/vtkCommonCore-9.4d.dll")
                      << QStringLiteral("Win64/OCC/bind/TKernel.dll")
                      << QStringLiteral("Win64/hdf5/bind/hdf5_D.dll")
                      << QStringLiteral("Win64/SARibbon/bind/SARibbonBard.dll")
                      << QStringLiteral("Win64/gmsh/gmsh.exe");
    suite.expect(defaultDependencies == expectedDebugDependencies,
                 QStringLiteral("default dependency set contains only the verified Debug baseline"));
    bool containsNonDebugDependency = false;
    for (const auto& dependency : defaultDependencies)
    {
        const QString portable = dependency.toLower();
        containsNonDebugDependency = containsNonDebugDependency ||
            portable.contains(QStringLiteral("/bin/")) ||
            portable.contains(QStringLiteral("/lib/")) ||
            portable.endsWith(QStringLiteral("hdf5.dll")) ||
            portable.endsWith(QStringLiteral("saribbonbar.dll"));
    }
    suite.expect(!containsNonDebugDependency,
                 QStringLiteral("default dependency set contains no non-Debug path"));

    const auto nativeResult = nativeChecker.check(nativeRequest);
    suite.expect(nativeResult.succeeded(), QStringLiteral("validated host dependencies and writable work directory"));
    suite.expect(nativeResult.availableDiskBytes > 0, QStringLiteral("reported available disk bytes"));

    auto missingProbe = std::make_shared<ControlledProbe>();
    AppMesh::App::SystemChecker missingChecker(missingProbe);
    auto controlledRequest = nativeRequest;
    controlledRequest.requiredToolFiles = QStringList() << QStringLiteral("missing.dll");
    const auto missingResult = missingChecker.check(controlledRequest);
    suite.expect(!missingResult.succeeded(), QStringLiteral("missing dependency fails validation"));
    suite.expect(hasCode(missingResult, QStringLiteral("ENV-DEPENDENCY-MISSING")),
                 QStringLiteral("missing dependency is structured and diagnosable"));

    auto readOnlyProbe = std::make_shared<ControlledProbe>();
    readOnlyProbe->writable = false;
    AppMesh::App::SystemChecker readOnlyChecker(readOnlyProbe);
    controlledRequest.requiredToolFiles = QStringList() << QStringLiteral("present.dll");
    const auto readOnlyResult = readOnlyChecker.check(controlledRequest);
    suite.expect(hasCode(readOnlyResult, QStringLiteral("ENV-WORKDIR-NOT-WRITABLE")),
                 QStringLiteral("unwritable work directory is rejected"));

    auto lowSpaceProbe = std::make_shared<ControlledProbe>();
    lowSpaceProbe->freeBytes = 64;
    AppMesh::App::SystemChecker lowSpaceChecker(lowSpaceProbe);
    controlledRequest.minimumFreeBytes = 128;
    const auto lowSpaceResult = lowSpaceChecker.check(controlledRequest);
    suite.expect(hasCode(lowSpaceResult, QStringLiteral("ENV-DISK-INSUFFICIENT")),
                 QStringLiteral("insufficient disk space is rejected"));

    controlledRequest.minimumFreeBytes = 1;
    controlledRequest.requiredToolFiles = QStringList() << QStringLiteral("../escape.dll");
    const auto escapedDependencyResult = missingChecker.check(controlledRequest);
    suite.expect(hasCode(escapedDependencyResult, QStringLiteral("ENV-DEPENDENCY-PATH-INVALID")),
                 QStringLiteral("dependency paths cannot escape the Tools boundary"));

    return suite.result();
}
