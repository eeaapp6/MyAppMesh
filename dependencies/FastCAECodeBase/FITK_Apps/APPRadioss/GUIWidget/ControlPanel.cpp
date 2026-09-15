/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ControlPanel.h" 
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"
#include "GUIWidget/TreeWidgetAssembly.h"
#include "GUIWidget/TreeWidgetGroup.h"
#include "GUIWidget/TreeWidgetProperty.h"
#include "GUIWidget/TreeWidgetSolution.h"
#include "ResultTreeWidget.h"
#include <QGridLayout>
#include <QSplitter>
#include <QTabWidget>
#include <QLabel>

#define Safe_Del(p) if(p) { delete p; p = nullptr; }

namespace GUI
{
    ControlPanel::ControlPanel(QWidget* p) : QDockWidget(p)
    {
        // 初始化部件树
        _treeAssembly = new TreeWidgetAssembly(this);
        _treeAssembly->setObjectName("Assembly");
        // 初始化模型树
        _treeGroup = new TreeWidgetGroup(this);
        _treeGroup->setObjectName("Group");
        // 初始化属性树
        _treeProperty = new TreeWidgetProperty(this);
        _treeProperty->setObjectName("Property");
        // 初始化解决方案树
        _treeSolution = new TreeWidgetSolution(this);
        _treeSolution->setObjectName("Solution");
        //初始化结果树
        _resultTreeWidget = new ResultTreeWidget(this);
        _resultTreeWidget->setObjectName("Result");

        //增加标签头
        QWidget* modelTabWidget = new QWidget(this);
        QVBoxLayout* modelTabLayout = new QVBoxLayout(modelTabWidget);
        modelTabLayout->setContentsMargins(0, 0, 0, 0);
        modelTabLayout->setSpacing(0);
        modelTabLayout->addWidget(_treeAssembly);
       
        // 子部件垂直排布
        QSplitter *spliterLayout = new QSplitter(Qt::Vertical, this);
        spliterLayout->setMouseTracking(true);
        spliterLayout->setHandleWidth(5);
        // 添加tab页
        _tab = new Comp::FITKTabWidget(Comp::FITKTabWidgetType::FITKTab_None, this);
        _tab->tabBar()->setDocumentMode(true);
        _tab->addTab(modelTabWidget, QObject::tr("Assembly"));
        _tab->addTab(_treeGroup, QObject::tr("Group"));
        _tab->addTab(_treeProperty, QObject::tr("Property"));
        _tab->addTab(_treeSolution, QObject::tr("Solution"));
        _tab->addTab(_resultTreeWidget, QObject::tr("Result"));
        connect(_tab, SIGNAL(currentChanged(int)), this, SLOT(slot_TabCurrentChanged(int)));
        spliterLayout->addWidget(_tab);

        //添加属性界面
       // spliterLayout->addWidget(_paramWidget);
        // 设置大小
        spliterLayout->setSizes({ 12000, 6000 });
        this->setWidget(spliterLayout);
    }

    ControlPanel::~ControlPanel()
    {
        Safe_Del(_treeAssembly);
        Safe_Del(_treeGroup);
        Safe_Del(_treeProperty);
        Safe_Del(_treeSolution);
        Safe_Del(_resultTreeWidget);
       // Safe_Del(_paramWidget);
        Safe_Del(_tab);
    }

    GUI::TreeWidgetAssembly* ControlPanel::getAssemblyTree()
    {
        // 装配树
        return _treeAssembly;
    }

    TreeWidgetGroup* ControlPanel::getGroupTree()
    {
        // 分组树
        return _treeGroup;
    }

    TreeWidgetProperty* ControlPanel::getPropertyTree()
    {
        // 属性树
        return _treeProperty;
    }

    TreeWidgetSolution* ControlPanel::getSolutionTree()
    {
        // 解决方案树
        return _treeSolution;
    }

    ResultTreeWidget * ControlPanel::getResultTree()
    {
        // 结果树
        return _resultTreeWidget;
    }

    void ControlPanel::setPage(const QString& name)
    {
        if (!_tab)return;
        int index = -1;
        for (int i = 0; i < _tab->count(); ++i) {
            if (_tab->widget(i)->objectName() == name)
                _tab->setCurrentIndex(i);
        }
        
    }

    void ControlPanel::slot_TabCurrentChanged(int index)
    {
        if (_treeAssembly == nullptr || _treeGroup == nullptr || _treeProperty == nullptr || _treeSolution == nullptr) return;
        QString actionName = "";
        if (index == 0) {
            _treeAssembly->updateTreeWidget();
            actionName = "OperatorChangePreGraph";
        }
        else if (index == 1) {
            _treeGroup->updateTreeWidget();
            actionName = "OperatorChangePreGraph";
        }
        else if (index == 2) {
            _treeProperty->updateTreeWidget();
            actionName = "OperatorChangePreGraph";
        }
        else if (index == 3) {
            _treeSolution->updateTreeWidget();
            actionName = "OperatorChangePreGraph";
        }
        else if (index == 4) {
            _resultTreeWidget->updateTreeWidget();
            actionName = "OperatorChangePostGraph";
        }
        else return;
        // Model待添加
        
        Core::FITKActionOperator* actionOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>(actionName);
        if (actionOper == nullptr) {
            return;
        }
        QObject* obj = new QObject();
        obj->setObjectName(actionName);
        actionOper->setEmitter(obj);
        if (actionOper) {
            actionOper->actionTriggered();
        }

        if (obj) {
            delete obj;
            obj = nullptr;
        }
    }

}   
