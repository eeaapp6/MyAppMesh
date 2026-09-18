#include "GeometryImportController.h"

#include "gui/GUIWidget/ConsoleWidget.h"
#include "gui/GUIWidget/ModelTree.h"

#include <QMetaObject>
#include <QThread>

namespace AppMesh::OperatorsGUI
{
namespace
{
Common::Diagnostic controllerDiagnostic(const QString& code,
                                        const QString& message,
                                        const QString& detail,
                                        const QString& path = QString())
{
    return {QStringLiteral("gui"),
            code,
            message,
            detail,
            false,
            path,
            QStringLiteral("operators.gui.geometry-import"),
            QString(),
            QString()};
}
}

GeometryImportController::GeometryImportController(
    OperatorsModel::TaskService& taskService,
    std::shared_ptr<Operators::IOperator> importOperator,
    Gui::ConsoleWidget* console,
    Gui::ModelTree* modelTree,
    GeometryRefreshHandler geometryRefreshHandler,
    QObject* parent)
    : QObject(parent),
      m_taskService(taskService),
      m_importOperator(std::move(importOperator)),
      m_console(console),
      m_modelTree(modelTree),
      m_geometryRefreshHandler(std::move(geometryRefreshHandler))
{
    const QPointer<GeometryImportController> guard(this);
    m_subscription = m_taskService.subscribe(
        [guard](const Operators::TaskEvent& event) {
            if (guard)
            {
                guard->queueEvent(event);
            }
        });
}

GeometryImportController::~GeometryImportController()
{
    m_subscription.reset();
}

GeometryImportRouteResult GeometryImportController::routeDialogResult(
    const GeometryImportRequest& request,
    bool accepted)
{
    GeometryImportRouteResult result;
    if (QThread::currentThread() != thread())
    {
        result.add(controllerDiagnostic(QStringLiteral("GUI-IMPORT-THREAD-INVALID"),
                                        QStringLiteral("Geometry import routing must run on the GUI thread."),
                                        QStringLiteral("Queue the accepted dialog value to the controller.")));
        return result;
    }
    if (!accepted)
    {
        return result;
    }
    if (request.requestToken == 0)
    {
        result.add(controllerDiagnostic(QStringLiteral("GUI-IMPORT-REQUEST-TOKEN-INVALID"),
                                        QStringLiteral("The geometry import request token is invalid."),
                                        QStringLiteral("Use one non-zero token per dialog interaction.")));
        return result;
    }
    if (m_processedRequestTokens.contains(request.requestToken))
    {
        result.add(controllerDiagnostic(QStringLiteral("GUI-IMPORT-REQUEST-DUPLICATE"),
                                        QStringLiteral("The geometry import request was already handled."),
                                        QStringLiteral("An accepted dialog may submit only once.")));
        return result;
    }
    m_processedRequestTokens.insert(request.requestToken);

    Operators::OperatorInput input;
    input.operationKey = OperatorsModel::ImportGeometryOperator::operationKey();
    input.description = QStringLiteral("Import geometry");
    input.parameters.insert(OperatorsModel::ImportGeometryOperator::filePathParameter(),
                            request.filePath);
    input.parameters.insert(OperatorsModel::ImportGeometryOperator::workDirectoryParameter(),
                            request.workDirectory);
    input.parameters.insert(OperatorsModel::ImportGeometryOperator::formatKeyParameter(),
                            request.formatKey);
    input.parameters.insert(OperatorsModel::ImportGeometryOperator::requestedNameParameter(),
                            request.requestedName);
    input.parameters.insert(OperatorsModel::ImportGeometryOperator::metadataParameter(),
                            request.metadata);

    const auto submitted = m_taskService.submit(m_importOperator, input);
    for (const auto& diagnostic : submitted.diagnostics)
    {
        result.add(diagnostic);
        report(diagnostic);
    }
    if (submitted.taskId)
    {
        result.taskId = submitted.taskId;
        m_tasks.insert(*submitted.taskId);
    }
    return result;
}

int GeometryImportController::submittedRequestCount() const noexcept
{
    return m_tasks.size();
}

void GeometryImportController::queueEvent(const Operators::TaskEvent& event)
{
    const QPointer<GeometryImportController> guard(this);
    QMetaObject::invokeMethod(
        this,
        [guard, event] {
            if (guard)
            {
                guard->handleEvent(event);
            }
        },
        Qt::QueuedConnection);
}

void GeometryImportController::handleEvent(const Operators::TaskEvent& event)
{
    if (!m_tasks.contains(event.taskId))
    {
        return;
    }
    if (event.diagnostic)
    {
        report(*event.diagnostic);
    }
    if (event.type == Operators::TaskEventType::Failed && event.error)
    {
        report(Operators::diagnosticFromErrorInfo(
            *event.error, QStringLiteral("operators.gui.geometry-import")));
    }
    if (event.type == Operators::TaskEventType::Succeeded)
    {
        if (m_console)
        {
            m_console->appendInfo(QStringLiteral("Geometry import succeeded."),
                                  QStringLiteral("task=%1").arg(event.taskId));
        }
        if (m_modelTree && m_modelTree->isBound())
        {
            const auto refreshed = m_modelTree->refreshFromRuntime();
            for (const auto& diagnostic : refreshed.diagnostics)
            {
                report(diagnostic);
            }
        }
        if (m_geometryRefreshHandler && event.result && event.result->objectId != Model::InvalidObjectId)
        {
            m_geometryRefreshHandler(event.result->objectId);
        }
    }
}

void GeometryImportController::report(const Common::Diagnostic& diagnostic)
{
    if (m_console)
    {
        m_console->appendDiagnostic(diagnostic);
    }
}
}
