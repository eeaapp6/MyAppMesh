/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "CentralWidget.h"
#include "ui_CentralWidget.h"
#include "GUIWidget/PreGraphWidget.h"
#include "GUIWidget/PostGraphWidget.h"
#include "GUIWidget/GraphMdiArea.h"
#include <QSplitter>
#include <QToolBar>

namespace GUI
{
    CentralWidget::CentralWidget(QWidget * parent):QWidget(parent)
    {
        //创建对象
        _ui = new Ui::CentralWidget;
        _ui->setupUi(this);
     
        //添加MDI区域
        _graphArea = new GraphMdiArea(this);
        _graphArea->setLayoutType(Comp::FITKVportsLayoutType::Tab);
        _graphArea->showTabBar(false);
        //添加前处理窗口
        _preWidgetId = _graphArea->addSubWidget(new PreGraphWidget(this), "pre");
        //添加后处理窗口
        _postWidgetId << _graphArea->addSubWidget(new PostGraphWidget(this), "post");
        _ui->layout->addWidget(_graphArea);


        //默认设置前处理窗口为显示主窗口
        setCurGraphWidgetToPre();
    }

    CentralWidget::~CentralWidget()
    { 
        if (_graphArea) delete _graphArea;
        if (_ui) delete _ui;
    }
  
    GraphMdiArea* CentralWidget::getGraphMidArea()
    {
        return _graphArea;
    }

    void CentralWidget::setCurGraphWidgetToPre()
    {
        if (_graphArea == nullptr) {
            return;
        }

        QMdiSubWindow* preWidget = _graphArea->getSubWidget(_preWidgetId);
        if (preWidget == nullptr) {
            _preWidgetId = _graphArea->addSubWidget(new PreGraphWidget(this), "pre");
            preWidget = _graphArea->getSubWidget(_preWidgetId);
        }
        _graphArea->setActiveSubWindow(preWidget);
    }

    void CentralWidget::setCurGraphWidgetToPost()
    {
        if (_graphArea == nullptr) {
            return;
        }

        QMdiSubWindow* posWidget = _graphArea->getSubWidget(_postWidgetId.at(0));//默认第一个
        if (posWidget == nullptr) {
            _postWidgetId << _graphArea->addSubWidget(new PostGraphWidget(this), "post");
            posWidget = _graphArea->getSubWidget(_postWidgetId.at(0));
        }
        _graphArea->setActiveSubWindow(posWidget);
    }

    void CentralWidget::setCurGraphWidget(QWidget* widget)
    {
        if (_graphArea == nullptr) {
            return;
        }
        QMdiSubWindow* subWidget = _graphArea->getSubWidget(widget);
        if (widget == nullptr) {
            return;
        }
        _graphArea->setActiveSubWindow(subWidget);
    }

    void CentralWidget::addPostWidgetId(int id)
    {
        if (!_postWidgetId.contains(id))
        {
            _postWidgetId.append(id);
        }
    }
}

