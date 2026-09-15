/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "NodeGroupWidget.h"
#include "ui_NodeGroupWidget.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include <QAction>

namespace GUI
{
    NodeGroupWidget::NodeGroupWidget(QWidget* parent) : FITKWidget(parent)
        , _ui(new Ui::NodeGroupWidget)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸

        // 获取可用的节点组列表
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKRadiossMeshModel* meshModel = caseObj->getMeshModel();
        if (meshModel == nullptr) return;
        _componentManager = meshModel->getComponentManager();
    }

    NodeGroupWidget::~NodeGroupWidget()
    {
        if (_ui)
            delete _ui;
        _ui = nullptr;
    }

    void NodeGroupWidget::setFirstNodeGroup(QString name, int id)
    {
        _firstNodeGroupName = name;
        _firstNodeGroupId = id;
        _ui->comboBox_nodeGroup->setFirstItem(_firstNodeGroupName, _firstNodeGroupId);
        init();
    }

    QString NodeGroupWidget::getCurrentName()
    {
        return  _ui->comboBox_nodeGroup->currentText();
    }

    void NodeGroupWidget::init()
    {
        _ui->comboBox_nodeGroup->updateItem();
        // 初始化时调整一次宽度
        _ui->comboBox_nodeGroup->adjustComboBoxWidth();
        //初始化下拉框选项
        this->findNodeGroup(-1);
    }
    void NodeGroupWidget::on_comboBox_nodeGroup_currentIndexChanged(int index)
    {
        //获取操作器
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr)return;
        //清除高亮
        operGraph->clearAllHight();
        //检查集合ID
        int idSet = this->getNodeGroupId();
        if (idSet < 1)return;
        //高亮
        operGraph->hightObject(idSet);
    }
    void NodeGroupWidget::on_pushButton_create_clicked()
    {
        //_ui->comboBox_nodeGroup->setCurrentIndex(0);
        //获取操作器
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr)return;
        //清除高亮
        operGraph->clearAllHight();
        //获取动作名称
        QString createActionName;
        switch (_type)
        {
        case GUI::Set_Node: createActionName = "actionSetNodesCreate";break;
        case GUI::Set_Element: createActionName = "actionSetElementCreate"; break;
        case GUI::Surface_Node: createActionName = "actionSurfNodeCreate"; break;
        case GUI::Surface_Element: createActionName = "actionSurfElementCreate"; break;
        default:
            break;
        }
        Core::FITKActionOperator* opt = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>(createActionName);
        if (opt == nullptr)return;
        connect(opt, &Core::FITKActionOperator::operatorAbstractSig1, this, &NodeGroupWidget::onComboBoxAboutToShow);
        QAction* action = new QAction();
        action->setObjectName(createActionName);
        opt->setEmitter(action);
        opt->execGUI();
    }
    void NodeGroupWidget::onComboBoxAboutToShow(Core::FITKAbstractOperator * oper)
    {
        disconnect(oper, &Core::FITKActionOperator::operatorAbstractSig1, this, &NodeGroupWidget::onComboBoxAboutToShow);
        //获取创建的对象ID
        int objID = -1;
        switch (_type)
        {
        case GUI::Set_Node:
        case GUI::Set_Element:oper->argValue<int>("SetId", objID);
            break;
        case GUI::Surface_Node:
        case GUI::Surface_Element:oper->argValue<int>("SurfId", objID);
            break;
        default:
            break;
        }
        //更新下拉框的数据
        _ui->comboBox_nodeGroup->updateItem();
        // 初始化时调整一次宽度
        _ui->comboBox_nodeGroup->adjustComboBoxWidth();
        findNodeGroup(objID);
    }
    void NodeGroupWidget::setWidgetGroupType(WidgetGroupType type)
    {
        _type = type;
        _ui->comboBox_nodeGroup->setWidgetGroupType(type);
    }
    int NodeGroupWidget::getNodeGroupId()
    {
        return _ui->comboBox_nodeGroup->currentData().toInt();
    }
    void NodeGroupWidget::findNodeGroup(int nodeGroupId)
    {
        if (_componentManager == nullptr) return;
        //查找并设置组数据
        int index = _ui->comboBox_nodeGroup->findData(nodeGroupId);
        _ui->comboBox_nodeGroup->setCurrentIndex(index < 0 ? 0 : index);
        
    }

    NodeGroupComboBox::NodeGroupComboBox(QWidget *parent) : QComboBox(parent)
    {
        // 获取可用的节点组列表
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKRadiossMeshModel* meshModel = caseObj->getMeshModel();
        if (meshModel == nullptr) return;
        _componentManager = meshModel->getComponentManager();
    }

    void NodeGroupComboBox::setWidgetGroupType(WidgetGroupType type)
    {
        _type = type;
    }
    void NodeGroupComboBox::updateItem()
    {
        this->blockSignals(true);
        // 清空组合框
        this->clear();
        // 添加第一项
        if (!_name.isEmpty()) this->addItem(_name, _id);
        if (_componentManager == nullptr) return;
        switch (_type)
        {
        case GUI::Set_Node:
        {
            QList<Interface::FITKModelSet*> setList = _componentManager->getUnInternalModelSet(Interface::FITKModelEnum::FITKModelSetType::FMSNode);
            int count = setList.size();
            // 添加节点组选项
            for (int i = 0; i < count; i++)
            {
                Interface::FITKModelSet* set = setList[i];
                this->addItem(set->getDataObjectName(), set->getDataObjectID());
            }
            break;
        }
        case GUI::Set_Element:
        {
            QList<Interface::FITKModelSet*> setList = _componentManager->getUnInternalModelSet(Interface::FITKModelEnum::FITKModelSetType::FMSElem);
            int count = setList.size();
            // 添加节点组选项
            for (int i = 0; i < count; i++)
            {
                Interface::FITKModelSet* set = setList[i];
                this->addItem(set->getDataObjectName(), set->getDataObjectID());
            }
            break;
        }
        case GUI::Surface_Node:
        {
            QList<Interface::FITKMeshSurface*> surfList = _componentManager->getUnInternalMeshSurface(Interface::FITKMeshSurface::MeshSurfaceType::SurNode);
            int count = surfList.size();
            // 添加节点组选项
            for (int i = 0; i < count; i++)
            {
                Interface::FITKMeshSurface* surf = surfList[i];
                this->addItem(surf->getDataObjectName(), surf->getDataObjectID());
            }
            break;
        }
        case GUI::Surface_Element:
        {
            QList<Interface::FITKMeshSurface*> surfList = _componentManager->getUnInternalMeshSurface(Interface::FITKMeshSurface::MeshSurfaceType::SurEle);
            int count = surfList.size();
            // 添加节点组选项
            for (int i = 0; i < count; i++)
            {
                Interface::FITKMeshSurface* surf = surfList[i];
                this->addItem(surf->getDataObjectName(), surf->getDataObjectID());
            }
            break;
        }
        default:
            break;
        }
        this->blockSignals(false);
    }
    void NodeGroupComboBox::showPopup()
    {
        this->updateItem();

        adjustComboBoxWidth();
        //获取操作器
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr)return;
        //清除高亮
        operGraph->clearAllHight();
        //检查集合ID
        int idSet = this->currentData().toInt();
        //高亮
        if (idSet > 0) operGraph->hightObject(idSet);
        // 执行原展开逻辑
        QComboBox::showPopup();
        
    }
    void NodeGroupComboBox::setFirstItem(QString name, int id)
    {
        _name = name;
        _id = id;
    }
    void NodeGroupComboBox::adjustComboBoxWidth()
    {
        int maxWidth = 0;
        QFontMetrics fontMetrics(this->font()); // 使用当前字体计算文本宽度

        // 遍历所有项，找到最长文本的宽度
        for (int i = 0; i < count(); ++i) {
            QString text = itemText(i);
            int textWidth = fontMetrics.horizontalAdvance(text); // 文本宽度
            maxWidth = qMax(maxWidth, textWidth);
        }
        // 增加一些边距（避免文本紧贴边框）
        maxWidth += 20; 
        // 设置组合框的最小宽度为最长项的宽度
        setMinimumWidth(maxWidth);
    }
}


