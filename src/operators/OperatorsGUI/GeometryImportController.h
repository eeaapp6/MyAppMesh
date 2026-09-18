#pragma once

#include "operators/OperatorsModel/ImportGeometryOperator.h"
#include "operators/OperatorsModel/TaskService.h"

#include <QObject>
#include <QPointer>
#include <QSet>

#include <memory>
#include <functional>

namespace AppMesh::Gui
{
class ConsoleWidget;
class ModelTree;
}

namespace AppMesh::OperatorsGUI
{
struct GeometryImportRequest
{
    quint64 requestToken = 0;
    QString filePath;
    QString workDirectory;
    QString formatKey;
    QString requestedName;
    QVariantMap metadata;
};

struct GeometryImportRouteResult : Common::OperationResult
{
    std::optional<Operators::TaskId> taskId;
};

class GeometryImportController final : public QObject
{
public:
    using GeometryRefreshHandler = std::function<void(Model::ObjectId)>;

    GeometryImportController(OperatorsModel::TaskService& taskService,
                             std::shared_ptr<Operators::IOperator> importOperator,
                             Gui::ConsoleWidget* console,
                             Gui::ModelTree* modelTree,
                             GeometryRefreshHandler geometryRefreshHandler = {},
                             QObject* parent = nullptr);
    ~GeometryImportController() override;

    GeometryImportController(const GeometryImportController&) = delete;
    GeometryImportController& operator=(const GeometryImportController&) = delete;

    GeometryImportRouteResult routeDialogResult(const GeometryImportRequest& request,
                                                bool accepted);
    int submittedRequestCount() const noexcept;

private:
    void queueEvent(const Operators::TaskEvent& event);
    void handleEvent(const Operators::TaskEvent& event);
    void report(const Common::Diagnostic& diagnostic);

    OperatorsModel::TaskService& m_taskService;
    std::shared_ptr<Operators::IOperator> m_importOperator;
    QPointer<Gui::ConsoleWidget> m_console;
    QPointer<Gui::ModelTree> m_modelTree;
    GeometryRefreshHandler m_geometryRefreshHandler;
    OperatorsModel::TaskEventSubscription m_subscription;
    QSet<quint64> m_processedRequestTokens;
    QSet<Operators::TaskId> m_tasks;
};
}
