#pragma once

#include <SARibbonMainWindow.h>
#include <QString>

class QAction;
class QDockWidget;
class QVBoxLayout;
class QWidget;
class SARibbonBar;
class SARibbonCategory;
class SARibbonPannel;

namespace AppMesh::GUIFrame
{
// T010 owns only the desktop shell. Business widgets and the future VTK
// viewport are supplied through the three injection points below.
class MainWindow final : public SARibbonMainWindow
{
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

    bool setModelTreeWidget(QWidget* widget);
    bool setConsoleWidget(QWidget* widget);
    bool setViewportWidget(QWidget* widget);

    QWidget* modelTreeWidget() const noexcept;
    QWidget* consoleWidget() const noexcept;
    QWidget* viewportWidget() const noexcept;

    QDockWidget* modelTreeDock() const noexcept;
    QDockWidget* consoleDock() const noexcept;
    QWidget* centralViewportHost() const noexcept;
    SARibbonBar* commandRibbon() const noexcept;

private:
    void createCommandArea();
    void createContentHosts();
    bool replaceHostedWidget(QWidget* host,
                             QVBoxLayout* layout,
                             QWidget*& current,
                             QWidget* replacement,
                             const QString& placeholderText,
                             const QString& placeholderObjectName);
    bool isAssignedToAnotherHost(const QWidget* widget,
                                 const QWidget* current) const noexcept;

    QWidget* m_centralViewportHost = nullptr;
    QVBoxLayout* m_viewportLayout = nullptr;
    QWidget* m_viewportWidget = nullptr;

    QDockWidget* m_modelTreeDock = nullptr;
    QWidget* m_modelTreeHost = nullptr;
    QVBoxLayout* m_modelTreeLayout = nullptr;
    QWidget* m_modelTreeWidget = nullptr;

    QDockWidget* m_consoleDock = nullptr;
    QWidget* m_consoleHost = nullptr;
    QVBoxLayout* m_consoleLayout = nullptr;
    QWidget* m_consoleWidget = nullptr;

    SARibbonCategory* m_fileCategory = nullptr;
    SARibbonCategory* m_viewCategory = nullptr;
    SARibbonCategory* m_meshCategory = nullptr;
    QAction* m_exitAction = nullptr;
};
}
