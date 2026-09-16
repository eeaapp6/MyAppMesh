#include "FITKComponentMapping.h"
#include "TestSupport.h"

#include <QCoreApplication>
#include <QSet>

#include <algorithm>

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;

    const auto validation = AppMesh::App::validateFITKComponentMappings();
    suite.expect(validation.succeeded(),
                 QStringLiteral("FITK component mapping is internally consistent"));

    const auto& mappings = AppMesh::App::fitkComponentMappings();
    QSet<QString> types;
    QSet<QString> keys;
    for (const auto& mapping : mappings)
    {
        suite.expect(!types.contains(mapping.fitkType),
                     QStringLiteral("mapping item is unique: %1").arg(mapping.fitkType));
        suite.expect(!keys.contains(mapping.registrationKey),
                     QStringLiteral("mapping registration key is unique: %1")
                         .arg(mapping.registrationKey));
        types.insert(mapping.fitkType);
        keys.insert(mapping.registrationKey);

        if (!mapping.realFITKEnabled)
        {
            suite.expect(!mapping.disabledReason.trimmed().isEmpty(),
                         QStringLiteral("disabled mapping has an explicit reason: %1")
                             .arg(mapping.stage));
        }
    }

    const QStringList expectedStartup = {
        QStringLiteral("fitk.application"),
        QStringLiteral("settings-system-check"),
        QStringLiteral("global-data-model-data"),
        QStringLiteral("component-factory"),
        QStringLiteral("python-boundary"),
        QStringLiteral("main-window-graph-data"),
        QStringLiteral("pre-window-signals"),
        QStringLiteral("plugins-manager"),
        QStringLiteral("app-initializer"),
        QStringLiteral("operator-repo"),
        QStringLiteral("command-line-workbench"),
        QStringLiteral("event-loop")};
    suite.expect(AppMesh::App::fitkStartupOrder() == expectedStartup,
                 QStringLiteral("canonical startup order is unique and complete"));

    QStringList expectedShutdown = expectedStartup;
    std::reverse(expectedShutdown.begin(), expectedShutdown.end());
    suite.expect(AppMesh::App::fitkShutdownOrder() == expectedShutdown,
                 QStringLiteral("shutdown order is the strict startup reverse"));

    const QStringList requiredConcepts = {
        QStringLiteral("FITKApplication"),
        QStringLiteral("FITKGlobalData"),
        QStringLiteral("FITKComponentManager"),
        QStringLiteral("FITKPluginsManager"),
        QStringLiteral("FITKOperatorRepo"),
        QStringLiteral("FITKRenderWindowVTK"),
        QStringLiteral("FITKAdaptor"),
        QStringLiteral("FITKGeoCommandList"),
        QStringLiteral("FITKThreadPool"),
        QStringLiteral("HDF5/IO"),
        QStringLiteral("Python bridge")};
    for (const auto& concept : requiredConcepts)
    {
        suite.expect(AppMesh::App::fitkMappingForType(concept) != nullptr,
                     QStringLiteral("mapping covers required concept: %1").arg(concept));
    }

    return suite.result();
}
