#include "app/MainWindowGenerator.h"
#include "gui/GUIFrame/MainWindow.h"
#include "TestSupport.h"

#include <QApplication>
#include <QAction>
#include <QDockWidget>
#include <QPointer>
#include <QWidget>

#include <SARibbonBar.h>
#include <SARibbonCategory.h>
#include <SARibbonMainWindow.h>
#include <SARibbonPannel.h>

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
                         window.commandRibbon() != nullptr &&
                         dynamic_cast<SARibbonMainWindow*>(&window) != nullptr,
                     QStringLiteral("main window is a real SARibbon window with viewport and docks"));
        auto* fileCategory = window.commandRibbon()->categoryByObjectName(
            QStringLiteral("fileProjectCategory"));
        auto* viewCategory = window.commandRibbon()->categoryByObjectName(
            QStringLiteral("viewCategory"));
        auto* meshCategory = window.commandRibbon()->categoryByObjectName(
            QStringLiteral("meshCategory"));
        suite.expect(fileCategory && viewCategory && meshCategory &&
                         fileCategory->pannelByObjectName(QStringLiteral("projectPanel")) &&
                         viewCategory->pannelByObjectName(QStringLiteral("viewPanelsPanel")) &&
                         meshCategory->pannelByObjectName(QStringLiteral("meshGenerationPanel")),
                     QStringLiteral("ribbon exposes File/Project, View and Mesh categories and panels"));
        for (const auto* name : {"newProjectAction", "openProjectAction",
                                 "saveProjectAction", "generateMeshAction"})
        {
            auto* action = window.findChild<QAction*>(QString::fromLatin1(name));
            suite.expect(action && !action->isEnabled(),
                         QStringLiteral("unfinished ribbon action remains disabled: %1")
                             .arg(QString::fromLatin1(name)));
        }
        suite.expect(window.findChild<QAction*>(QStringLiteral("exitAction")) != nullptr,
                     QStringLiteral("ribbon exposes an enabled Exit action"));
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

    {
        QPointer<QWidget> ownedWindow;
        bool cleanupObservedDestroyedWindow = false;
        AppMesh::App::MainWindowGenerator generator(
            [&ownedWindow] {
                auto window = std::make_unique<QWidget>();
                ownedWindow = window.get();
                return window;
            },
            false,
            [&] {
                cleanupObservedDestroyedWindow = ownedWindow.isNull();
                return AppMesh::App::AppOperationResult{};
            });
        suite.expect(generator.create().succeeded() && generator.destroy().succeeded() &&
                         cleanupObservedDestroyedWindow,
                     QStringLiteral("window cleanup runs after QObject dependents are destroyed"));
    }

    return suite.result();
}
