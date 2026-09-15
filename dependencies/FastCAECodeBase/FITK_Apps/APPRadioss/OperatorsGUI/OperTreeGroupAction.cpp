/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperTreeGroupAction.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include <QTreeWidgetItem>

namespace GUIOper
{
    OperTreeGroupAction::~OperTreeGroupAction()
    {
        // 重置选中节点的数据ID
        _selectedItemDataID = -1;
    }

    bool OperTreeGroupAction::execGUI()
    {
        // 检查触发器是否为空，如果为空则返回false
        if (_emitter == nullptr) return false;

        // 获取触发器的对象名称
        QString name = _emitter->objectName();
        // 如果名称为空，则返回false
        if (name.isEmpty()) return false;


        // 获取全局数据中的Radioss案例数据
        Radioss::FITKRadiossCase* dataCase = getRadiossCase();
        // 如果数据案例为空，则返回false
        if (dataCase == nullptr) return false;

        // 获取案例数据中的网格模型
        Radioss::FITKRadiossMeshModel* dataMesh = dataCase->getMeshModel();
        // 如果网格模型为空，则返回false
        if (dataMesh == nullptr) return false;

        // 获取网格模型中的组件管理器
        Interface::FITKComponentManager* managerComponent = dataMesh->getComponentManager();
        // 如果组件管理器为空，则返回false
        if (managerComponent == nullptr) return false;

        // 根据不同的对象名称调用相应的操作函数
        if (name == "actionGroupTreeCreateSetByNodes") callOperator("actionSetNodesCreate");
        else if (name == "actionGroupTreeCreateSetByElement") callOperator("actionSetElementCreate");
        else if (name == "actionGroupTreeCreateSurfaceByNodes") callOperator("actionSurfNodeCreate");
        else if (name == "actionGroupTreeCreateSurfaceByElement") callOperator("actionSurfElementCreate");
        else if (name == "actionGroupTreeRename") renameData<Interface::FITKComponentManager*>(this, managerComponent, _selectedItemDataID);
        else if (name == "actionGroupTreeDelete") deleteData(managerComponent, _selectedItemDataID);
        else return false;

        // 清除高亮显示
        clearAllHighLight();

        return true;
    }

    void OperTreeGroupAction::preArgs()
    {
        // 初始化选中节点的数据ID
        _selectedItemDataID = -1;

        // 获取选中项列表参数
        QVariant selectedItems;
        this->argValue("SelectedItems", selectedItems);
        if (selectedItems.isNull()) return;

        // 将参数转换为TreeWidgetItem列表指针并检查有效性
        QList<QTreeWidgetItem*>* listSelectedItems = static_cast<QList<QTreeWidgetItem*>*>(selectedItems.value<void*>());
        if (listSelectedItems == nullptr || listSelectedItems->isEmpty()) return;

        // 获取第一个选中的项的ID
        _selectedItemDataID = listSelectedItems->first()->data(1, 0).toInt();
    }
}
