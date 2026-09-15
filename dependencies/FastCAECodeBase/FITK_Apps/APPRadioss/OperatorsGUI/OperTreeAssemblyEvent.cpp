/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperTreeAssemblyEvent.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"
#include "GUIWidget/GUIEnumType.h"
#include "GUIWidget/TreeWidgetAssembly.h"

namespace GUIOper
{
    void OperTreeAssemblyEvent::updateTree()
    {
        // 获取主窗口
        GUI::MainWindow* mainWindow = GUI::MainWindow::GetMainWindowFromFramework();
        if (mainWindow == nullptr) return;
        // 获取控件面板
        GUI::ControlPanel* controlPanel = mainWindow->getControlPanel();
        if (controlPanel == nullptr) return;
        // 获取装配树
        GUI::TreeWidgetAssembly* assemblyTreeWidget = controlPanel->getAssemblyTree();
        if (assemblyTreeWidget == nullptr) return;
        // 更新装配树
        assemblyTreeWidget->updateTreeWidget();
    }

    void OperTreeAssemblyEvent::on_itemStateChanged(QTreeWidget* w, QTreeWidgetItem* item, bool state, int column)
    {
        Q_UNUSED(w);
        Q_UNUSED(item);
        Q_UNUSED(state);
        Q_UNUSED(column);
        // 检查参数
        if (item == nullptr) return;

        // 获取节点类型
        int type = item->type();
        // 获取数据ID
        int id = item->data(1, 0).toInt();
        // 获取数据对象可见状态
        bool isCheck = item->checkState(0) == Qt::CheckState::Checked ? true : false;

        // 根据节点类型更新数据对象可见状态
        if (type == GUI::TreeWidgetAssemblyType::TWAssemblyGeometryChild)
        {
            // 获取几何管理器
            Interface::FITKGeoCommandList* managerGeometry = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
            if (managerGeometry == nullptr) return;
            // 获取数据对象
            Interface::FITKAbsGeoCommand* data = managerGeometry->getDataByID(id);
            if (data == nullptr) return;
            // 设置数据对象是否可见
            data->enable(isCheck);

        }
        else if (type == GUI::TreeWidgetAssemblyType::TWAssemblyPartChild)
        {
            // 获取算例数据
            Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
            if (dataCase == nullptr) return;
            // 获取网格模型
            Radioss::FITKRadiossMeshModel* mesh = dataCase->getMeshModel();
            if (mesh == nullptr) return;
            // 获取部件管理器
            Radioss::FITKRadiossPartManager* managerPart = mesh->getPartsManager();
            if (managerPart == nullptr) return;
            // 获取数据对象
            Radioss::FITKRadiossPart* data = managerPart->getDataByID(id);
            if (data == nullptr) return;
            // 设置数据对象是否可见
            data->enable(isCheck);
        }

        // 获取可视对象数据操作器
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;

        // 设置模型可见状态
        oper->setModelVisible(id, isCheck, false);
    }
}