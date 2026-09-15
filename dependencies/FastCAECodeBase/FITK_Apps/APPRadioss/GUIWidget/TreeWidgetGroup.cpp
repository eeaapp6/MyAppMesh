/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "TreeWidgetGroup.h"
#include "GUIEnumType.h"
#include "FITK_Kernel/FITKCore/FITKTreeWidgetOperator.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include <QHeaderView>

namespace GUI
{
    TreeWidgetGroup::TreeWidgetGroup(QWidget* parent) : GUITreeWidgetBase(parent)
    {
        // 隐藏树形菜单头
        this->header()->hide();
        // 设置选择模式：单选
        this->setSelectionMode(QAbstractItemView::SingleSelection);
        // 更新树形菜单
        updateTreeWidget();
    }

    void TreeWidgetGroup::updateTreeWidget()
    {
        // 锁定信号（防止刷新时触发信号）
        this->blockSignals(true);
        // 清空树
        this->clear();

        // 创建根节点
        QTreeWidgetItem* rootSet = createRootItem(this, tr("Set"), GUI::TreeWidgetGroupType::TWGroupSet);
        if (rootSet == nullptr) return;
        QTreeWidgetItem* rootSurface = createRootItem(this, tr("Surface"), GUI::TreeWidgetGroupType::TWGroupSurface);
        if (rootSurface == nullptr) return;

        // 获取网格模型数据
        Radioss::FITKRadiossCase* dataCase = getDataCase();
        if (dataCase == nullptr) return;
        Radioss::FITKRadiossMeshModel* mesh = dataCase->getMeshModel();
        if (mesh == nullptr) return;

        // 获取部件管理器
        Radioss::FITKRadiossPartManager* managerPart = mesh->getPartsManager();
        if (managerPart == nullptr) return;

        // 获取集合管理器
        Interface::FITKComponentManager* managerComponent = mesh->getComponentManager();
        if (managerComponent == nullptr) return;

        // 更新子节点
        updateSet(rootSet, managerComponent->getAllSet(false));
        updateSurface(rootSurface, managerComponent->getAllMeshSurface());

        // 展开全部节点
        this->expandAll();
        // 解锁信号（刷新后允许触发信号）
        this->blockSignals(false);
    }

    Core::FITKTreeWidgetOperator* TreeWidgetGroup::getOperator()
    {
        return getOperatorT<Core::FITKTreeWidgetOperator>("TreeGroupEvent");
    }

    void TreeWidgetGroup::on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu)
    {
        // 检查输入参数
        if (items.size() != 1 || menu == nullptr) return;

        // 获取当前项
        QTreeWidgetItem* item = items.at(0);
        if (item == nullptr) return;

        // 根据节点类型初始化右键菜单
        int type = item->type();

        // 根据节点类型生成菜单
        if (type == GUI::TreeWidgetGroupType::TWGroupSet)
        {
            createMenuItem(menu, tr("Create Set By Nodes"), "actionGroupTreeCreateSetByNodes");
            createMenuItem(menu, tr("Create Set By Element"), "actionGroupTreeCreateSetByElement");
        }
        else if (type == GUI::TreeWidgetGroupType::TWGroupSurface)
        {
            createMenuItem(menu, tr("Create Surface By Nodes"), "actionGroupTreeCreateSurfaceByNodes");
            createMenuItem(menu, tr("Create Surface By Element"), "actionGroupTreeCreateSurfaceByElement");
        }
        else if (type == GUI::TreeWidgetGroupType::TWGroupSetChild || type == GUI::TreeWidgetGroupType::TWGroupSurfaceChild)
        {
            createMenuItem(menu, tr("Rename"), "actionGroupTreeRename");
            createMenuItem(menu, tr("Delete"), "actionGroupTreeDelete");
        }
        else return;
    }
    
    void TreeWidgetGroup::updateSet(QTreeWidgetItem* itemParent, QList<Interface::FITKModelSet*> listSet)
    {
        // 检查输入参数
        if (itemParent == nullptr || listSet.size() == 0) return;

        // 遍历集合列表
        for (Interface::FITKModelSet* data : listSet)
        {
            // 当集合无效或为内部集合时，跳过
            if (data == nullptr || data->isInternal()) continue;

            // 创建集合子节点
            QTreeWidgetItem* itemAssemblySet = createChildItem(itemParent, data->getDataObjectName(), GUI::TreeWidgetGroupType::TWGroupSetChild);
            if (itemAssemblySet != nullptr) setObjectID(itemAssemblySet, data->getDataObjectID());
        }
    }
    
    void TreeWidgetGroup::updateSurface(QTreeWidgetItem* itemParent, QList<Interface::FITKMeshSurface*> listSurface)
    {
        // 检查输入参数
        if (itemParent == nullptr || listSurface.size() == 0) return;

        // 遍历表面列表
        for (Interface::FITKMeshSurface* data : listSurface)
        {
            // 仅当表面无效时，跳过
            if (data == nullptr) continue;

            // 创建表面子节点
            QTreeWidgetItem* itemAssemblySurface = createChildItem(itemParent, data->getDataObjectName(), GUI::TreeWidgetGroupType::TWGroupSurfaceChild);
            if (itemAssemblySurface != nullptr) setObjectID(itemAssemblySurface, data->getDataObjectID());
        }
    }
}