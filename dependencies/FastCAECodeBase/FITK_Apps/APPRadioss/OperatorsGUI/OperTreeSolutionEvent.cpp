/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperTreeSolutionEvent.h"
#include "GUIWidget/GUIEnumType.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRadiossData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"
#include "GUIWidget/TreeWidgetSolution.h"

namespace GUIOper
{
    void OperTreeSolutionEvent::updateTree()
    {
        // 获取主窗口
        GUI::MainWindow* mainWindow = GUI::MainWindow::GetMainWindowFromFramework();
        if (mainWindow == nullptr) return;

        // 获取控制面板
        GUI::ControlPanel* controlPanel = mainWindow->getControlPanel();
        if (controlPanel == nullptr) return;

        // 获取解决方案树
        GUI::TreeWidgetSolution* tree = controlPanel->getSolutionTree();
        if (tree == nullptr) return;

        // 更新解决方案树
        tree->updateTreeWidget();
    }

    void OperTreeSolutionEvent::on_itemClicked(QTreeWidget* w, QTreeWidgetItem* item, int col)
    {
        Q_UNUSED(col);

        //点击树节点事件处理
        EventOper::GraphEventOperator* oper = getGraphEventOperator();
        if (oper == nullptr) return;
        oper->clearAllHight();

        //点击树节点事件处理(高亮接口)
        if (item == nullptr || w == nullptr) return;

        // 初始化ID
        int id = item->data(1, 0).toInt();
        if (id > 0) oper->hightObject(id);

        // 获取Radioss案例数据对象
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return;

        // 获取解决方案管理器
        Radioss::FITKRadiossSolutionManager* managerSolution = dataCase->getSolutionManager();
        if (managerSolution == nullptr) return;

        // 获取当前解决方案
        Radioss::FITKRadiossSolution* currentSolution = managerSolution->getCurrentSolution();
        if (currentSolution == nullptr) return;
        
        // 获取当前解决方案ID
        int idSolution = item->data(2, 0).toInt();
        // 判断当前解决方案ID是否一致，如果不一致，则更新当前解决方案
        if (currentSolution->getDataObjectID() != idSolution)
        {
            // 更新当前解决方案
            managerSolution->setCurrentSolution(idSolution);
            // 更新树
            updateTree();
        }
    }

    void OperTreeSolutionEvent::on_itemStateChanged(QTreeWidget* w, QTreeWidgetItem* item, bool state, int column)
    {
        // 检查参数
        if (item == nullptr) return;

        // 获取节点类型
        int type = item->type();
        // 获取数据ID
        int id = item->data(1, 0).toInt();
        // 获取数据对象可见状态
        bool isCheck = item->checkState(0) == Qt::CheckState::Checked ? true : false;
        
        // 获取Radioss案例数据对象
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return;

        // 获取解决方案管理器
        Radioss::FITKRadiossSolutionManager* managerSolution = dataCase->getSolutionManager();
        if (managerSolution == nullptr) return;

        // 获取当前解决方案
        Radioss::FITKRadiossSolution* currentSolution = managerSolution->getCurrentSolution();
        if (currentSolution == nullptr) return;

        // 获取可视化对象操作器
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;

        // 根据节点类型处理数据对象可见状态
        if (type == GUI::TreeWidgetSolutionType::TWSolutionChildInitialConditionInitialField)
        {
            // 初始场
            Radioss::FITKInitialFieldManager* manager = currentSolution->getInitialFieldManager();
            if (manager == nullptr) return;

            Radioss::FITKAbstractInitialField* data = manager->getDataByID(id);
            if (data == nullptr) return;

            bool isActive = data->isEnable();

            isActive ? data->setVisible(isCheck) : data->setVisible(false);
            isActive ? oper->setModelVisible(id, isCheck, false) : oper->setModelVisible(id, false, false);
            if (!isActive) item->setCheckState(0, Qt::CheckState::Unchecked);
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildConstraintRigidWall)
        {
            // 刚性墙
            Radioss::FITKRadiossRWallManager* manager = currentSolution->getRWallManager();
            if (manager == nullptr) return;

            Radioss::FITKAbstractRWall* data = manager->getDataByID(id);
            if (data == nullptr) return;

            bool isActive = data->isEnable();

            isActive ? data->setVisible(isCheck) : data->setVisible(false);
            isActive ? oper->setModelVisible(id, isCheck, false) : oper->setModelVisible(id, false, false);
            if (!isActive) item->setCheckState(0, Qt::CheckState::Unchecked);
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildConnectionRBody)
        {
            //刚性连接
            Radioss::FITKConnectionManager* manager = currentSolution->getConnectionManager();
            if (manager == nullptr) return;

            Radioss::FITKAbstractConnection* data = manager->getDataByID(id);
            if (data == nullptr) return;

            bool isActive = data->isEnable();

            isActive ? data->setVisible(isCheck) : data->setVisible(false);
            isActive ? oper->setModelVisible(id, isCheck, false) : oper->setModelVisible(id, false, false);
            if (!isActive) item->setCheckState(0, Qt::CheckState::Unchecked);
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildConstraintGravity)
        {
            // 重力场
            Radioss::FITKGravityManager* manager = currentSolution->getGravityManager();
            if (manager == nullptr) return;

            Radioss::FITKGravity* data = manager->getDataByID(id);
            if (data == nullptr) return;

            bool isActive = data->isEnable();

            isActive ? data->setVisible(isCheck) : data->setVisible(false);
            isActive ? oper->setModelVisible(id, isCheck, false) : oper->setModelVisible(id, false, false);
            if (!isActive) item->setCheckState(0, Qt::CheckState::Unchecked);
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildBoundaryBCS)
        {
            // 边界
            Radioss::FITKRadiossBCSManager* manager = currentSolution->getBCSManager();
            if (manager == nullptr) return;

            Radioss::FITKAbstractBCS* data = manager->getDataByID(id);
            if (data == nullptr) return;

            bool isActive = data->isEnable();

            isActive ? data->setVisible(isCheck) : data->setVisible(false);
            isActive ? oper->setModelVisible(id, isCheck, false) : oper->setModelVisible(id, false, false);
            if (!isActive) item->setCheckState(0, Qt::CheckState::Unchecked);
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildInteractionKinematicTied
            || type == GUI::TreeWidgetSolutionType::TWSolutionChildInteractionMultiUsageImpact)
        {
            // 相互作用(绑定和多用途冲击)
            Radioss::FITKInteractionManager* manager = currentSolution->getInteractionManager();
            if (manager == nullptr) return;

            Radioss::FITKAbstractInteraction* data = manager->getDataByID(id);
            if (data == nullptr) return;

            bool isActive = data->isEnable();

            isActive ? data->setVisible(isCheck) : data->setVisible(false);
            isActive ? oper->setModelVisible(id, isCheck, false) : oper->setModelVisible(id, false, false);
            if (!isActive) item->setCheckState(0, Qt::CheckState::Unchecked);
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildProbeAccel
            || type == GUI::TreeWidgetSolutionType::TWSolutionChildProbeSection)
        {
            // 探针
            Radioss::FITKProbeManager* manager = currentSolution->getProbeManager();
            if (manager == nullptr) return;

            Radioss::FITKAbstractProbe* data = manager->getDataByID(id);
            if (data == nullptr) return;

            bool isActive = data->isEnable();

            isActive ? data->setVisible(isCheck) : data->setVisible(false);
            isActive ? oper->setModelVisible(id, isCheck, false) : oper->setModelVisible(id, false, false);
            if (!isActive) item->setCheckState(0, Qt::CheckState::Unchecked);
        }
        else return;
    }
}