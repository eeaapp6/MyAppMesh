#include "app/MainWindowGenerator.h"
#include "gui/GUIFrame/MainWindow.h"
#include "TestSupport.h"

#include <QApplication>
#include <QDockWidget>
#include <QPointer>
#include <QWidget>

#include <memory>
#include <stdexcept>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    TestSuite suite;

    {
        AppMesh::GUIFrame::MainWindow window;
        suite.expect(window.centralViewportHost() != nullptr &&
                         window.modelTreeDock() != nullptr &&
                         window.consoleDock() != nullptr &&
                         window.commandToolBar() != nullptr,
                     QStringLiteral("main window creates viewport, docks and command area"));
        suite.expect(window.viewportWidget() != nullptr &&
                         window.viewportWidget()->objectName() ==
                             QStringLiteral("viewportPlaceholder"),
                     QStringLiteral("main window remains usable without a VTK viewport"));

        QPointer<QWidget> firstModelWidget = new QWidget;
        QPointer<QWidget> secondModelWidget = new QWidget;
        suite.expect(window.setModelTreeWidget(firstModelWidget) &&
                         window.modelTreeWidget() == firstModelWidget,
                     QStringLiteral("model tree widget can be injected"));
        suite.expect(window.setModelTreeWidget(secondModelWidget) &&
                         firstModelWidget.isNull() &&
                         window.modelTreeWidget() == secondModelWidget,
                     QStringLiteral("replacement deletes exactly the previously owned widget"));
        suite.expect(!window.setConsoleWidget(secondModelWidget) &&
                         window.consoleWidget() != secondModelWidget,
                     QStringLiteral("one widget cannot be owned by two hosts"));
        suite.expect(window.setModelTreeWidget(nullptr) && secondModelWidget.isNull(),
                     QStringLiteral("null injection safely restores a placeholder"));

        window.modelTreeDock()->hide();
        suite.expect(window.modelTreeDock()->isHidden(),
                     QStringLiteral("model tree dock can be hidden"));
        window.modelTreeDock()->show();
        suite.expect(!window.modelTreeDock()->isHidden(),
                     QStringLiteral("model tree dock can be shown"));
    }

    {
        AppMesh::App::MainWindowGenerator generator(
            [] { return std::unique_ptr<QWidget>(); }, false);
        const auto result = generator.create();
        suite.expect(!result.succeeded() && generator.window() == nullptr,
                     QStringLiteral("null factory result leaves no half-initialized window"));
    }

    {
        AppMesh::App::MainWindowGenerator generator(
            []() -> std::unique_ptr<QWidget> {
                throw std::runtime_error("injected construction failure");
            },
            false);
        const auto result = generator.create();
        suite.expect(!result.succeeded() && generator.window() == nullptr,
                     QStringLiteral("factory exception leaves no half-initialized window"));
    }

    {
        AppMesh::App::MainWindowGenerator generator({}, false);
        const auto created = generator.create();
        suite.expect(created.succeeded() &&
                         dynamic_cast<AppMesh::GUIFrame::MainWindow*>(generator.window()) != nullptr,
                     QStringLiteral("production generator creates the T010 main window"));
        suite.expect(generator.destroy().succeeded() && generator.window() == nullptr,
                     QStringLiteral("production generator destroys the window deterministically"));
    }

    return suite.result();
}
