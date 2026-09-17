#include "app/MainWindowGenerator.h"
#include "common/AppDiagnostic.h"
#include "gui/GUIFrame/MainWindow.h"
#include "gui/GUIWidget/ConsoleWidget.h"
#include "gui/GUIWidget/ModelTree.h"
#include "model/ModelData/GeometryManager.h"
#include "model/ModelData/MeshManager.h"
#include "TestSupport.h"

#include <QApplication>
#include <QPointer>
#include <QThread>
#include <QWidget>

#include <memory>
#include <stdexcept>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    TestSuite suite;
    suite.expect(QThread::currentThread() == application.thread(),
                 QStringLiteral("QApplication is created on the GUI thread"));

    AppMesh::Model::ApplicationRuntime runtime;
    AppMesh::Model::GeometryManager geometryManager(runtime);
    AppMesh::Model::MeshManager meshManager(runtime, geometryManager);

    QPointer<AppMesh::Gui::ModelTree> treeGuard;
    QPointer<AppMesh::Gui::ConsoleWidget> consoleGuard;
    QPointer<QWidget> viewportGuard;
    {
        auto window = std::make_unique<AppMesh::GUIFrame::MainWindow>();
        auto tree = std::make_unique<AppMesh::Gui::ModelTree>();
        auto console = std::make_unique<AppMesh::Gui::ConsoleWidget>();
        auto viewport = std::make_unique<QWidget>();
        viewport->setObjectName(QStringLiteral("t013PlaceholderViewport"));

        treeGuard = tree.get();
        consoleGuard = console.get();
        viewportGuard = viewport.get();

        const auto bindResult = tree->bind(&runtime, &geometryManager, &meshManager);
        tree->setDiagnosticHandler([guard = consoleGuard](
                                       const AppMesh::Common::Diagnostic& diagnostic) {
            if (guard)
            {
                guard->appendDiagnostic(diagnostic);
            }
        });

        const bool assembled = bindResult.succeeded() &&
            window->setModelTreeWidget(tree.get()) &&
            window->setConsoleWidget(console.get()) &&
            window->setViewportWidget(viewport.get());
        if (assembled)
        {
            tree.release();
            console.release();
            viewport.release();
        }

        suite.expect(assembled,
                     QStringLiteral("MainWindow accepts the ModelData-bound GUI assembly"));
        suite.expect(window->commandRibbon() && window->modelTreeDock() &&
                         window->consoleDock() && window->centralViewportHost(),
                     QStringLiteral("command area, model tree, console and viewport hosts exist"));
        suite.expect(window->modelTreeWidget() == treeGuard &&
                         window->consoleWidget() == consoleGuard &&
                         window->viewportWidget() == viewportGuard,
                     QStringLiteral("each injected widget occupies exactly one host"));
        suite.expect(treeGuard && treeGuard->parent() != nullptr && consoleGuard &&
                         consoleGuard->parent() != nullptr && viewportGuard &&
                         viewportGuard->parent() != nullptr,
                     QStringLiteral("MainWindow has unique QObject ownership of injected widgets"));

        AppMesh::Model::CreateObjectRequest request;
        request.name = QStringLiteral("T013 Runtime Object");
        request.type = AppMesh::Model::DataObjectType::Generic;
        const auto created = runtime.createObject(request);
        application.processEvents();
        suite.expect(created.succeeded() && created.object.has_value() && treeGuard &&
                         treeGuard->objectModel()->indexForObject(created.object->id).isValid(),
                     QStringLiteral("ModelTree observes its bound ApplicationRuntime"));

        const AppMesh::Common::Diagnostic diagnostic{
            QStringLiteral("gui"),
            QStringLiteral("T013-SMOKE-DIAGNOSTIC"),
            QStringLiteral("Smoke diagnostic"),
            QStringLiteral("Console diagnostic routing is active."),
            true,
            QString(),
            QStringLiteral("gui.smoke"),
            created.object ? QString::number(created.object->id) : QString()};
        consoleGuard->appendDiagnostic(diagnostic);
        suite.expect(consoleGuard->plainText().contains(diagnostic.code) &&
                         consoleGuard->lastMutationOccurredOnGuiThread(),
                     QStringLiteral("diagnostics are visible in ConsoleWidget on the GUI thread"));

        window.reset();
        suite.expect(treeGuard.isNull() && consoleGuard.isNull() && viewportGuard.isNull(),
                     QStringLiteral("destroying MainWindow destroys each hosted widget exactly once"));
        suite.expect(runtime.validateIndexes().succeeded() &&
                         geometryManager.validateIndexes().succeeded() &&
                         meshManager.validateIndexes().succeeded(),
                     QStringLiteral("widgets are destroyed before their non-owning ModelData services"));
    }

    {
        auto placeholderWindow = std::make_unique<AppMesh::GUIFrame::MainWindow>();
        suite.expect(placeholderWindow->viewportWidget() &&
                         placeholderWindow->viewportWidget()->objectName() ==
                             QStringLiteral("viewportPlaceholder"),
                     QStringLiteral("the pre-GraphData viewport is an explicit placeholder"));
    }

    const int topLevelsBeforeFailure = QApplication::topLevelWidgets().size();
    AppMesh::App::MainWindowGenerator failingGenerator(
        []() -> std::unique_ptr<QWidget> {
            throw std::runtime_error("controlled T013 window creation failure");
        },
        false);
    const auto failedCreation = failingGenerator.create();
    suite.expect(!failedCreation.succeeded() && !failedCreation.diagnostics.isEmpty() &&
                     failedCreation.diagnostics.front().code ==
                         QStringLiteral("APP-MAIN-WINDOW-CREATE-FAILED") &&
                     failingGenerator.window() == nullptr &&
                     QApplication::topLevelWidgets().size() == topLevelsBeforeFailure,
                 QStringLiteral("window creation failure is diagnosed without a partial window"));

    application.processEvents();
    suite.expect(QApplication::topLevelWidgets().isEmpty(),
                 QStringLiteral("QApplication can drain GUI teardown without leaked top-level widgets"));
    return suite.result();
}
