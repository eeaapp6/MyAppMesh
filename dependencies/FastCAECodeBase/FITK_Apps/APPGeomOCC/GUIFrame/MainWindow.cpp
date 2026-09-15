/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "RenderWidget.h"
#include "ActionEventHandler.h"
#include "TreeWidget.h"
#include "MessageTabWidget.h"
#include "AIAssistantDockWidget.h"

#include <QString>
#include <QMenu>
#include <QDebug>
#include <QToolBar>
#include <QHash>
#include <QApplication>
#include <QCoreApplication>
#include <QPainter>
#include <QColor>
#include <QSplitter>
#include <QGridLayout>
#include <QStatusBar>
#include <QLabel>
#include <QToolBar>
#include <QDialog>

#include "FITK_Kernel/FITKCore/FITKGUIRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractGlobalDataFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"
#include "FITK_Kernel/FITKAppFramework/FITKCommandLineHandler.h"

namespace GUI
{
    MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
        , m_Ui(new Ui::MainWindow)
    {
        m_Ui->setupUi(this);
        _currentWidget = new QWidget(this);

        init();

        devTest();
    }

    void MainWindow::devTest()
    {
        // 隐藏或显示开发工具栏。
        //@{
        m_Ui->toolBarDev->setVisible(FITKGLODATA->getRunTimeSetting()->getValue("DEV").toBool());
        //@}
    }

    MainWindow::~MainWindow()
    {
        delete m_Ui;
    }

    void MainWindow::closeEvent(QCloseEvent* event)
    {
        // 关闭全部子窗口并析构后关闭主窗体。 Added by ChengHaotian. 2024/08/23
        Core::FITKGUIRepo::getInstance()->closeAllDialog();

        //QList<QDialog*> dlgs = Core::FITKGUIRepo::getInstance()->getGUIObjects<QDialog>();
        //for (QDialog* dlg : dlgs)
        //{
        //    if (dlg)
        //    {
        //        dlg->setAttribute(Qt::WA_DeleteOnClose, false);
        //        dlg->close();
        //        delete dlg;
        //    }
        //}

        QMainWindow::closeEvent(event);
    }

    void MainWindow::updateGeometryTree()
    {
        if (m_TreeWidget == nullptr) return;
        m_TreeWidget->updateTree();
    }

    ActionEventHandler * MainWindow::getActionEventHandle() const
    {
        return m_ActionHandler;
    }

    QAction * MainWindow::createAction(const QString & toolTip, const QString & objectName, const QString & iconPath, const QString & iconText)
    {
        // 实例化一个action
        QAction* tempAction = new QAction(this);

        // 设置action的ObjectName
        tempAction->setObjectName(objectName);
        // 设置action的ToolTip
        tempAction->setToolTip(toolTip);
        // 设置action的图标
        changeAction(tempAction, iconPath, iconText);

        return tempAction;
    }

    void MainWindow::init()
    {
        //设置无边框
        if (FITKAPP->getCommandLineHandler()->containsValue("-httpserver"))
        {
            setWindowFlag(Qt::FramelessWindowHint, true);
        }

        m_ActionHandler = new ActionEventHandler;

        initCentralWidget();

        QList<QAction*> actionList = this->findChildren<QAction*>();
        for (QAction* action : actionList) {
            if (action == nullptr)continue;
            if (action->objectName() == "actionAIAgent")
            {
                connect(action, &QAction::triggered, this, &MainWindow::showAIAssistantDock);
                continue;
            }
            connect(action, SIGNAL(triggered()), this->getActionEventHandle(), SLOT(execOperator()));
        }

        // 初始化界面按钮状态。
        setSketchMode(false);
        this->showAIAssistantDock();
    }

    void MainWindow::initCentralWidget()
    {
        // 子部件水平排布
        QSplitter *spliterLayout = new QSplitter(Qt::Horizontal);
        spliterLayout->setMouseTracking(true);
        spliterLayout->setHandleWidth(5);

        m_TreeWidget = new TreeWidget(this);
        m_RenderWidget = new RenderWidget(this);
        m_MsgWidget = new MessageTabWidget(this);

        // 添加树界面
        spliterLayout->addWidget(m_TreeWidget);

        // 添加日志与可视化区布局。
        QSplitter* vLayout = new QSplitter(Qt::Vertical);
        spliterLayout->setMouseTracking(true);
        spliterLayout->setHandleWidth(5);
        vLayout->addWidget(m_RenderWidget);
        vLayout->addWidget(m_MsgWidget);

        // 添加右侧布局。
        spliterLayout->addWidget(vLayout);

        // 设置大小
        spliterLayout->setSizes({ 200, 1300 });

        auto mainLayout = new QGridLayout();
        mainLayout->setObjectName("CentralGridLayout");
        mainLayout->setContentsMargins(5, 5, 5, 0);
        mainLayout->addWidget(spliterLayout);

        _currentWidget->setLayout(mainLayout);
        setCentralWidget(_currentWidget);
    }

    void MainWindow::showAIAssistantDock()
    {
        AIAssistantDockWidget* aiDock = this->findChild<AIAssistantDockWidget*>("AIAssistantDock");
        if (!aiDock)
        {
            aiDock = new AIAssistantDockWidget(this);
            if (!aiDock->isValid())
            {
                delete aiDock;
                return;
            }

            this->addDockWidget(Qt::RightDockWidgetArea, aiDock);
            // QList<QDockWidget*> docks{ m_TreeWidget, aiDock };
            // QList<int> sizes{ 420, 360 };
            // this->resizeDocks(docks, sizes, Qt::Horizontal);
        }
        else if (aiDock->isVisible())
        {
            aiDock->hide();
            return;
        }
        else if (this->dockWidgetArea(aiDock) != Qt::RightDockWidgetArea || aiDock->isFloating())
        {
            aiDock->setFloating(false);
            this->addDockWidget(Qt::RightDockWidgetArea, aiDock);

            // QList<QDockWidget*> docks{ m_TreeWidget, aiDock };
            // QList<int> sizes{ 420, 360 };
            // this->resizeDocks(docks, sizes, Qt::Horizontal);
        }

        aiDock->show();
        aiDock->raise();
        aiDock->activateWindow();
    }

    bool MainWindow::changeAction(QAction * action, const QString iconPath, const QString & iconText)
    {
        if (action == nullptr)return false;
        //图片修改
        if (!iconPath.isEmpty()) {
            action->setIcon(QIcon(iconPath));
        };

        //图注修改
        if (iconText.isEmpty()) {
            action->setText(action->toolTip());
        }
        else {
            action->setText(iconText);
        }

        return true;
    }

    void MainWindow::setBarVisible(bool visible)
    {
        m_barVisibility = visible;

        QList<QToolBar*> toolBars = findChildren<QToolBar*>();
        for (QToolBar* toolBar : toolBars)
        {
            if (toolBar)
            {
                toolBar->setVisible(visible);
            }
        }

        m_Ui->menuBar->setVisible(visible);
    }

    void MainWindow::setSketchMode(bool isOn)
    {
        // 工具栏除草图与视图以及文件外全部禁用。
        QList<QToolBar*> toolBars = findChildren<QToolBar*>();
        for (QToolBar* toolBar : toolBars)
        {
            if (!toolBar)
            {
                continue;
            }

            // 防止禁用子窗口工具栏。
            if (toolBar->parent() != this)
            {
                continue;
            }

            if (toolBar != m_Ui->toolBarView && 
                toolBar != m_Ui->toolBarSketcher &&
                toolBar != m_Ui->toolBarProfile &&
                toolBar != m_Ui->toolBarOperate)
            {
                toolBar->setEnabled(!isOn);
            }
            else if (toolBar == m_Ui->toolBarProfile)
            {
                toolBar->setEnabled(isOn);
            }
        }

        // 菜单栏禁用建模相关按钮。
        m_Ui->menuCreate->setEnabled(!isOn);
        m_Ui->menuEdit->setEnabled(!isOn);
        m_Ui->menuSketcher->setEnabled(isOn);
    }

    QAction* MainWindow::getSketchAction()
    {
        return m_Ui->actionSketch2DCreate;
    }

    RenderWidget * MainWindow::getRenderWidget() const
    {
        return m_RenderWidget;
    }

    TreeWidget* MainWindow::getTreeWidget()
    {
        return m_TreeWidget;
    }
}
