/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "MainTreeWidget.h"
#include "RenderWidget.h"
#include "ActionEventHandler.h"
#include "MessageTabWidget.h"
#include "GUIWidget/ToolBarAnimation.h"
#include "GUIWidget/ToolBarColorMapper.h"
#include "GUIWidget/InformationWidget.h"
#include "GUIWidget/PropertyWidget.h"
#include "GUIWidget/ToolBarFilter.h"
#include "GUIWidget/ToolBarEvent.h"
#include "AIAssistantDockWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"
#include "FITK_Kernel/FITKAppFramework/FITKCommandLineHandler.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpPythonDriverInterface.h"
#include "FITK_Component/FITKHttpPythonDriver/HttpClient.h"

#include <QComboBox>
#include <QDebug>
#include <QLabel>
#include <QSpinBox>
#include <QToolBar>

namespace GUI
{
    MainWindow::MainWindow(QWidget * parent):
        QMainWindow(parent)
    {
        _ui = new Ui::MainWindow();
        _ui->setupUi(this);

        init();
    }

    MainWindow::~MainWindow()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void MainWindow::init()
    {
        //设置无边框
        if (FITKAPP->getCommandLineHandler()->containsValue("-httpserver"))
        {
            setWindowFlag(Qt::FramelessWindowHint, true);
        }
        
        setWindowTitle("CFDPost");
        _actionHandler = new ActionEventHandler;
        initCentralWidget();
        initAction();

        _eventWidget = new Comp::FITKTabWidget(Comp::FITKTabWidgetType::FITKTab_None, this);
        _ui->verticalLayout_Event->addWidget(_eventWidget);
        _eventWidget->hide();

        QList<QAction*> actionList = this->findChildren<QAction*>();
        for (QAction* action : actionList) {
            if (action == nullptr)continue;
            if (action->objectName() == "actionAIAgent")
            {
                connect(action, &QAction::triggered, this, &MainWindow::showAIAssistantDock);
                continue;
            }
            connect(action, SIGNAL(triggered()), _actionHandler, SLOT(execOperator()));
        }
         this->showAIAssistantDock();
    }

    void MainWindow::showAIAssistantDock()
    {
        if (_aiAssistantWidget == nullptr)
        {
            _aiAssistantWidget = new AIAssistantDockWidget(_eventWidget);
            if (!_aiAssistantWidget->isValid())
            {
                delete _aiAssistantWidget;
                _aiAssistantWidget = nullptr;
                return;
            }

            _eventWidget->addTab(_aiAssistantWidget, "AI Assistant");
            _eventWidget->setCurrentWidget(_aiAssistantWidget);
            _eventWidget->show();
        }

        if (_aiAssistantWidget->isVisible())
        {
            _aiAssistantWidget->hide();
            _eventWidget->removeTab(_eventWidget->indexOf(_aiAssistantWidget));
            if (_eventWidget->count() > 0)
                _eventWidget->show();
            else
                _eventWidget->hide();
        }
        else
        {
            _eventWidget->addTab(_aiAssistantWidget, "AI Assistant");
            _eventWidget->setCurrentWidget(_aiAssistantWidget);
            _aiAssistantWidget->show();
            _aiAssistantWidget->raise();
            _eventWidget->show();
        }
    }

    QAction* MainWindow::createAction(QString actionName, QString iconPath, QString actionTitle)
    {
        if (actionName.isEmpty())return nullptr;
        QAction* action = new QAction(this);
        action->setObjectName(actionName);
        action->setIcon(QIcon(iconPath));
        action->setToolTip(actionTitle);
        return action;
    }

    QAction * MainWindow::findAction(QString actionName)
    {
        return this->findChild<QAction*>(actionName);
    }

    RenderWidget * MainWindow::getRenderWidget()
    {
        return _renderWidget;
    }

    MainTreeWidget * MainWindow::getTreeWidget()
    {
        return _treeWidget;
    }

    PropertyWidget * MainWindow::getPropertyWidget()
    {
        return _propertyWidget;
    }

    ToolBarColorMapper * MainWindow::getToolBarColorMapper()
    {
        return _colorMapToolBar;
    }

    ToolBarAnimation* MainWindow::getToolBarAnimation()
    {
        return _animationToolBar;
    }

    ToolBarFilter* MainWindow::getToolFilter()
    {
        return _filterBar;
    }

    ToolBarEvent * MainWindow::getToolEvent()
    {
        return _filterEvent;
    }

    InformationWidget * MainWindow::getInfoWidget()
    {
        return _infoWidget;
    }

    Comp::FITKTabWidget * MainWindow::getEventWidget()
    {
        return _eventWidget;
    }

    void MainWindow::closeEvent(QCloseEvent * event)
    {
        emit this->sigMainWindowClose();
        QMainWindow::closeEvent(event);
    }

    void MainWindow::showEvent(QShowEvent* event)
    {
        QMainWindow::showEvent(event);

        //if (_isSoftwareInfoSent == true)
        //{
        //    return;
        //}

        //HttpPython::FITKHttpPythonDriverInterface* httpPythonDriver = FITKAPP->getComponents()->getComponentTByName<HttpPython::FITKHttpPythonDriverInterface>("FITKHttpPythonDriver");
        //if (httpPythonDriver == nullptr)
        //{
        //    return;
        //}

        //HttpPython::HttpClient* httpClient = httpPythonDriver->getHttpClient();
        //if (httpClient == nullptr)
        //{
        //    return;
        //}

        //httpClient->sendSoftwareInfo();
        _isSoftwareInfoSent = true;
    }

    void MainWindow::initCentralWidget()
    {
        _treeWidget = new MainTreeWidget(this);
        _renderWidget = new RenderWidget(this);
        _infoWidget = new GUI::InformationWidget(this);
        _propertyWidget = new GUI::PropertyWidget(this);
        _msgWidget = new MessageTabWidget(this);

        _ui->treeLayout->addWidget(_treeWidget);
        _ui->propertyLayout->addWidget(_propertyWidget);
        _ui->verticalLayout_Render->addWidget(_renderWidget);
        _ui->verticalLayout_Render->addWidget(_msgWidget);
        _ui->infoLayout->addWidget(_infoWidget);
    }

    void MainWindow::initAction()
    {
        QAction* action = nullptr;
        QToolBar* tooBar = new QToolBar(this);
        addToolBar(tooBar);
        action = createAction("actionOpen", "://icons/open.png", QString(tr("open")));
        tooBar->addAction(action);
        action = findAction("actionSaveImage");
        if (action)tooBar->addAction(action);
        action = createAction("actionVideo", "://icons/video.png", QString(tr("open")));
        tooBar->addAction(action);
        //AI助手显示隐藏按钮
        action = createAction("actionAIAgent", "", QString(tr("Show/Hide Agent")));
        tooBar->addAction(action);
        
        QToolBar* viewBar = new QToolBar(this);
        addToolBar(viewBar);
        action = findAction("actionViewAuto");
        if (action) viewBar->addAction(action);
        action = findAction("actionViewFront");
        if (action) viewBar->addAction(action);
        action = findAction("actionViewBack");
        if (action) viewBar->addAction(action);
        action = findAction("actionViewTop");
        if (action) viewBar->addAction(action);
        action = findAction("actionViewBottom");
        if (action) viewBar->addAction(action);
        action = findAction("actionViewLeft");
        if (action) viewBar->addAction(action);
        action = findAction("actionViewRight");
        if (action) viewBar->addAction(action);
        action = findAction("actionViewIso");
        if (action) viewBar->addAction(action);
        viewBar->addSeparator();

        _animationToolBar = new ToolBarAnimation(this);
        this->addToolBar(_animationToolBar);

        this->addToolBarBreak(Qt::TopToolBarArea);
        _colorMapToolBar = new ToolBarColorMapper(this);
        this->addToolBar(_colorMapToolBar);

        _filterBar = new ToolBarFilter(this);
        this->addToolBar(_filterBar);

        _filterEvent = new ToolBarEvent(this);
        this->addToolBar(_filterEvent);
    }
}

