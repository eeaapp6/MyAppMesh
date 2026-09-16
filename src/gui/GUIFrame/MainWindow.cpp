#include "MainWindow.h"

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QThread>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

namespace AppMesh::GUIFrame
{
namespace
{
QWidget* createPlaceholder(const QString& text,
                           const QString& objectName)
{
    auto* placeholder = new QLabel(text);
    placeholder->setObjectName(objectName);
    static_cast<QLabel*>(placeholder)->setAlignment(Qt::AlignCenter);
    return placeholder;
}

QAction* createDisabledAction(QObject* parent,
                              const QString& text,
                              const QString& objectName)
{
    auto* action = new QAction(text, parent);
    action->setObjectName(objectName);
    action->setEnabled(false);
    return action;
}
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    Q_ASSERT(!qApp || QThread::currentThread() == qApp->thread());

    setObjectName(QStringLiteral("appMeshMainWindow"));
    setWindowTitle(QStringLiteral("APPMesh"));
    resize(1200, 800);

    createContentHosts();
    createCommandArea();
}

void MainWindow::createContentHosts()
{
    m_centralViewportHost = new QWidget(this);
    m_centralViewportHost->setObjectName(QStringLiteral("centralViewportHost"));
    m_viewportLayout = new QVBoxLayout(m_centralViewportHost);
    m_viewportLayout->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(m_centralViewportHost);

    m_modelTreeDock = new QDockWidget(tr("Model"), this);
    m_modelTreeDock->setObjectName(QStringLiteral("modelTreeDock"));
    m_modelTreeDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_modelTreeHost = new QWidget(m_modelTreeDock);
    m_modelTreeHost->setObjectName(QStringLiteral("modelTreeHost"));
    m_modelTreeLayout = new QVBoxLayout(m_modelTreeHost);
    m_modelTreeLayout->setContentsMargins(0, 0, 0, 0);
    m_modelTreeDock->setWidget(m_modelTreeHost);
    addDockWidget(Qt::LeftDockWidgetArea, m_modelTreeDock);

    m_consoleDock = new QDockWidget(tr("Console"), this);
    m_consoleDock->setObjectName(QStringLiteral("consoleDock"));
    m_consoleDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    m_consoleHost = new QWidget(m_consoleDock);
    m_consoleHost->setObjectName(QStringLiteral("consoleHost"));
    m_consoleLayout = new QVBoxLayout(m_consoleHost);
    m_consoleLayout->setContentsMargins(0, 0, 0, 0);
    m_consoleDock->setWidget(m_consoleHost);
    addDockWidget(Qt::BottomDockWidgetArea, m_consoleDock);

    setViewportWidget(nullptr);
    setModelTreeWidget(nullptr);
    setConsoleWidget(nullptr);
}

void MainWindow::createCommandArea()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->setObjectName(QStringLiteral("fileMenu"));
    auto* newAction = createDisabledAction(this,
                                           tr("New Project"),
                                           QStringLiteral("newProjectAction"));
    auto* openAction = createDisabledAction(this,
                                            tr("Open Project"),
                                            QStringLiteral("openProjectAction"));
    auto* saveAction = createDisabledAction(this,
                                            tr("Save Project"),
                                            QStringLiteral("saveProjectAction"));
    m_exitAction = new QAction(tr("Exit"), this);
    m_exitAction->setObjectName(QStringLiteral("exitAction"));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    m_fileMenu->addAction(newAction);
    m_fileMenu->addAction(openAction);
    m_fileMenu->addAction(saveAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_viewMenu = menuBar()->addMenu(tr("&View"));
    m_viewMenu->setObjectName(QStringLiteral("viewMenu"));
    m_viewMenu->addAction(m_modelTreeDock->toggleViewAction());
    m_viewMenu->addAction(m_consoleDock->toggleViewAction());

    m_meshMenu = menuBar()->addMenu(tr("&Mesh"));
    m_meshMenu->setObjectName(QStringLiteral("meshMenu"));
    auto* generateAction = createDisabledAction(this,
                                                tr("Generate Mesh"),
                                                QStringLiteral("generateMeshAction"));
    m_meshMenu->addAction(generateAction);

    m_commandToolBar = addToolBar(tr("Main Commands"));
    m_commandToolBar->setObjectName(QStringLiteral("mainCommandToolBar"));
    m_commandToolBar->setMovable(false);
    m_commandToolBar->addAction(newAction);
    m_commandToolBar->addAction(openAction);
    m_commandToolBar->addSeparator();
    m_commandToolBar->addAction(generateAction);
}

bool MainWindow::replaceHostedWidget(QWidget* host,
                                     QVBoxLayout* layout,
                                     QWidget*& current,
                                     QWidget* replacement,
                                     const QString& placeholderText,
                                     const QString& placeholderObjectName)
{
    if (!host || !layout || (qApp && QThread::currentThread() != qApp->thread()))
    {
        return false;
    }
    if (replacement && replacement == current)
    {
        return true;
    }
    if (replacement == this || replacement == host ||
        isAssignedToAnotherHost(replacement, current) ||
        (current && replacement && current->isAncestorOf(replacement)))
    {
        return false;
    }

    QWidget* next = replacement;
    if (!next)
    {
        next = createPlaceholder(placeholderText, placeholderObjectName);
    }

    QWidget* previous = current;
    current = nullptr;
    if (previous)
    {
        layout->removeWidget(previous);
        previous->setParent(nullptr);
        delete previous;
    }

    next->setParent(host);
    layout->addWidget(next);
    current = next;
    return true;
}

bool MainWindow::isAssignedToAnotherHost(const QWidget* widget,
                                         const QWidget* current) const noexcept
{
    if (!widget)
    {
        return false;
    }
    return (widget == m_viewportWidget && widget != current) ||
           (widget == m_modelTreeWidget && widget != current) ||
           (widget == m_consoleWidget && widget != current);
}

bool MainWindow::setModelTreeWidget(QWidget* widget)
{
    return replaceHostedWidget(m_modelTreeHost,
                               m_modelTreeLayout,
                               m_modelTreeWidget,
                               widget,
                               tr("Model tree is not connected"),
                               QStringLiteral("modelTreePlaceholder"));
}

bool MainWindow::setConsoleWidget(QWidget* widget)
{
    return replaceHostedWidget(m_consoleHost,
                               m_consoleLayout,
                               m_consoleWidget,
                               widget,
                               tr("Console is not connected"),
                               QStringLiteral("consolePlaceholder"));
}

bool MainWindow::setViewportWidget(QWidget* widget)
{
    return replaceHostedWidget(m_centralViewportHost,
                               m_viewportLayout,
                               m_viewportWidget,
                               widget,
                               tr("3D viewport is not connected"),
                               QStringLiteral("viewportPlaceholder"));
}

QWidget* MainWindow::modelTreeWidget() const noexcept
{
    return m_modelTreeWidget;
}

QWidget* MainWindow::consoleWidget() const noexcept
{
    return m_consoleWidget;
}

QWidget* MainWindow::viewportWidget() const noexcept
{
    return m_viewportWidget;
}

QDockWidget* MainWindow::modelTreeDock() const noexcept
{
    return m_modelTreeDock;
}

QDockWidget* MainWindow::consoleDock() const noexcept
{
    return m_consoleDock;
}

QWidget* MainWindow::centralViewportHost() const noexcept
{
    return m_centralViewportHost;
}

QToolBar* MainWindow::commandToolBar() const noexcept
{
    return m_commandToolBar;
}
}
