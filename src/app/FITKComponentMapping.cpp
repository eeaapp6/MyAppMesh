#include "FITKComponentMapping.h"

#include <QSet>

#include <algorithm>
#include <map>

namespace AppMesh::App
{
namespace
{
AppDiagnostic mappingDiagnostic(const QString& code,
                                const QString& message,
                                const QString& detail,
                                const QString& stage)
{
    return {QStringLiteral("component"), code, message, detail, false, QString(), stage};
}
}

const std::vector<FITKComponentMapping>& fitkComponentMappings()
{
    static const std::vector<FITKComponentMapping> mappings = {
        {QStringLiteral("fitk.application"), QStringLiteral("FITKApplication"),
         QStringLiteral("fitk.application"), QStringLiteral("FITKAppFramework"), {},
         0, 11, true, {}},
        {QStringLiteral("settings-system-check"), QStringLiteral("SystemChecker/MeshAPPSettings"),
         QStringLiteral("appmesh.settings-system-check"), QStringLiteral("APPMesh"),
         {QStringLiteral("fitk.application")}, 1, 10, true, {}},
        {QStringLiteral("global-data-model-data"), QStringLiteral("FITKGlobalData"),
         QStringLiteral("fitk.global-data"), QStringLiteral("FITKAppFramework"),
         {QStringLiteral("fitk.application")}, 2, 9, true, {}},
        {QStringLiteral("global-data-model-data"), QStringLiteral("ModelData boundary"),
         QStringLiteral("appmesh.model-data.boundary"), QStringLiteral("APPMesh"),
         {QStringLiteral("fitk.global-data")}, 2, 9, false,
         QStringLiteral("T005 owns the concrete APPMesh ModelData implementation.")},
        {QStringLiteral("component-factory"), QStringLiteral("FITKComponentManager"),
         QStringLiteral("fitk.component-manager"), QStringLiteral("FITKAppFramework"),
         {QStringLiteral("fitk.global-data")}, 3, 8, true, {}},
        {QStringLiteral("component-factory"), QStringLiteral("FITKThreadPool"),
         QStringLiteral("fitk.thread-pool"), QStringLiteral("FITKCore"),
         {QStringLiteral("fitk.application")}, 3, 8, true, {}},
        {QStringLiteral("python-boundary"), QStringLiteral("Python bridge"),
         QStringLiteral("fitk.python.boundary"), QStringLiteral("FITKPython"),
         {QStringLiteral("fitk.component-manager")}, 4, 7, false,
         QStringLiteral("T033 owns concrete Python wrapper registration.")},
        {QStringLiteral("main-window-graph-data"), QStringLiteral("FITKRenderWindowVTK"),
         QStringLiteral("fitk.render-window-vtk"), QStringLiteral("FITKRenderWindowVTK"),
         {QStringLiteral("fitk.component-manager")}, 5, 6, false,
         QStringLiteral("T010 and T026 own the formal window and GraphData implementation.")},
        {QStringLiteral("main-window-graph-data"), QStringLiteral("FITKAdaptor"),
         QStringLiteral("fitk.graph-adaptor"), QStringLiteral("FITKAdaptor"),
         {QStringLiteral("fitk.render-window-vtk")}, 5, 6, false,
         QStringLiteral("T026 owns concrete GraphData adaptation.")},
        {QStringLiteral("pre-window-signals"), QStringLiteral("FITKSignalTransfer"),
         QStringLiteral("fitk.signal-transfer"), QStringLiteral("FITKAppFramework"),
         {QStringLiteral("fitk.component-manager")}, 6, 5, true, {}},
        {QStringLiteral("plugins-manager"), QStringLiteral("FITKPluginsManager"),
         QStringLiteral("fitk.plugins-manager"), QStringLiteral("FITKAppFramework"),
         {QStringLiteral("fitk.component-manager")}, 7, 4, true, {}},
        {QStringLiteral("app-initializer"), QStringLiteral("HDF5/IO"),
         QStringLiteral("fitk.hdf5-io.boundary"), QStringLiteral("APPMesh"),
         {QStringLiteral("fitk.plugins-manager")}, 8, 3, false,
         QStringLiteral("T029 owns concrete project HDF5/IO registration.")},
        {QStringLiteral("operator-repo"), QStringLiteral("FITKOperatorRepo"),
         QStringLiteral("fitk.operator-repo"), QStringLiteral("FITKCore"),
         {QStringLiteral("fitk.component-manager")}, 9, 2, true, {}},
        {QStringLiteral("operator-repo"), QStringLiteral("FITKGeoCommandList"),
         QStringLiteral("fitk.geo-command-list"), QStringLiteral("FITKInterfaceGeometry"),
         {QStringLiteral("fitk.operator-repo")}, 9, 2, false,
         QStringLiteral("T015 and T016 own geometry commands and import adapters.")},
        {QStringLiteral("command-line-workbench"), QStringLiteral("CommandLine/Workbench boundary"),
         QStringLiteral("appmesh.command-line-workbench"), QStringLiteral("APPMesh"),
         {QStringLiteral("fitk.operator-repo")}, 10, 1, true, {}},
        {QStringLiteral("event-loop"), QStringLiteral("Qt/FITK event loop"),
         QStringLiteral("fitk.event-loop"), QStringLiteral("FITKAppFramework"),
         {QStringLiteral("appmesh.command-line-workbench")}, 11, 0, true, {}}};
    return mappings;
}

const FITKComponentMapping* fitkMappingForType(const QString& fitkType)
{
    const auto& mappings = fitkComponentMappings();
    const auto iterator = std::find_if(mappings.cbegin(), mappings.cend(),
                                       [&fitkType](const FITKComponentMapping& mapping) {
                                           return mapping.fitkType == fitkType;
                                       });
    return iterator == mappings.cend() ? nullptr : &*iterator;
}

QStringList fitkStartupOrder()
{
    return {QStringLiteral("fitk.application"),
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
}

QStringList fitkShutdownOrder()
{
    QStringList order = fitkStartupOrder();
    std::reverse(order.begin(), order.end());
    return order;
}

AppOperationResult validateFITKComponentMappings()
{
    AppOperationResult result;
    QSet<QString> stages;
    QSet<QString> keys;
    QSet<QString> availableDependencies;
    std::map<QString, int> startupByKey;

    const auto startupStages = fitkStartupOrder();
    for (const auto& mapping : fitkComponentMappings())
    {
        if (mapping.stage.trimmed().isEmpty() || mapping.fitkType.trimmed().isEmpty() ||
            mapping.registrationKey.trimmed().isEmpty() || mapping.library.trimmed().isEmpty())
        {
            result.add(mappingDiagnostic(QStringLiteral("FITK-MAPPING-INCOMPLETE"),
                                         QStringLiteral("A FITK component mapping is incomplete."),
                                         mapping.fitkType,
                                         mapping.stage));
        }
        if (keys.contains(mapping.registrationKey))
        {
            result.add(mappingDiagnostic(QStringLiteral("FITK-MAPPING-KEY-DUPLICATE"),
                                         QStringLiteral("A FITK registration key is duplicated."),
                                         mapping.registrationKey,
                                         mapping.stage));
        }
        keys.insert(mapping.registrationKey);
        stages.insert(mapping.stage);
        availableDependencies.insert(mapping.registrationKey);
        startupByKey.emplace(mapping.registrationKey, mapping.startupOrder);

        if (!mapping.realFITKEnabled && mapping.disabledReason.trimmed().isEmpty())
        {
            result.add(mappingDiagnostic(QStringLiteral("FITK-MAPPING-DISABLED-REASON-MISSING"),
                                         QStringLiteral("A disabled FITK mapping has no reason."),
                                         mapping.fitkType,
                                         mapping.stage));
        }
        if (mapping.startupOrder < 0 || mapping.startupOrder >= startupStages.size() ||
            mapping.shutdownOrder != startupStages.size() - 1 - mapping.startupOrder)
        {
            result.add(mappingDiagnostic(QStringLiteral("FITK-MAPPING-ORDER-INVALID"),
                                         QStringLiteral("FITK startup/shutdown order is invalid."),
                                         mapping.fitkType,
                                         mapping.stage));
        }
    }

    for (const auto& mapping : fitkComponentMappings())
    {
        for (const auto& dependency : mapping.dependencies)
        {
            if (!availableDependencies.contains(dependency))
            {
                result.add(mappingDiagnostic(QStringLiteral("FITK-MAPPING-DEPENDENCY-MISSING"),
                                             QStringLiteral("A FITK mapping dependency is missing."),
                                             dependency,
                                             mapping.stage));
            }
            else if (startupByKey.at(dependency) > mapping.startupOrder)
            {
                result.add(mappingDiagnostic(QStringLiteral("FITK-MAPPING-DEPENDENCY-ORDER-INVALID"),
                                             QStringLiteral("A FITK mapping dependency starts after its consumer."),
                                             dependency,
                                             mapping.stage));
            }
        }
    }
    return result;
}
}
