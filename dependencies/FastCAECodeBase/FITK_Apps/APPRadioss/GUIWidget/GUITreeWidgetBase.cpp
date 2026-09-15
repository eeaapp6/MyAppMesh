/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUITreeWidgetBase.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include <QMenu>

namespace GUI
{
    GUITreeWidgetBase::GUITreeWidgetBase(QWidget* parent) : Comp::FITKTreeWidget(parent)
    {
    }

    QTreeWidgetItem* GUITreeWidgetBase::createRootItem(QTreeWidget* parentWidget, QString name, int type)
    {
        // 创建根节点
        return new QTreeWidgetItem(parentWidget, QStringList(name), type);
    }

    QTreeWidgetItem* GUITreeWidgetBase::createChildItem(QTreeWidgetItem* parentItem, QString name, int type)
    {
        // 创建子节点
        return new QTreeWidgetItem(parentItem, QStringList(name), type);
    }

    void GUITreeWidgetBase::setObjectID(QTreeWidgetItem* item, int id)
    {
        // 检查数据是否有效
        if (item == nullptr) return;

        // 存储数据ID
        item->setData(1, 0, QVariant::fromValue(id));
    }

    int GUITreeWidgetBase::getObjectID(QTreeWidgetItem* item)
    {
        // 检查数据是否有效
        if (item == nullptr) return -1;

        // 获取数据ID
        return item->data(1, 0).toInt();
    }

    void GUITreeWidgetBase::setItemChecked(QTreeWidgetItem* item, int column, bool checked)
    {
        // 检查数据是否有效
        if (item == nullptr) return;

        // 设置点击状态
        item->setCheckState(column, checked ? Qt::Checked : Qt::Unchecked);
    }

    void GUITreeWidgetBase::createMenuItem(QMenu* menu, QString itemName, QString itemObjectName)
    {
        // 检查数据是否有效
        if (menu == nullptr) return;

        // 创建菜单项
        QAction* action = menu->addAction(itemName);
        // 设置对象名称
        action->setObjectName(itemObjectName);
    }

    void GUITreeWidgetBase::show(int id, bool isShow)
    {
        // 获取可视化对象操作器
        //EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        //if (oper == nullptr) return;

        //oper->setModelVisible(id, isShow, false);
    }

    void GUITreeWidgetBase::highlight(int id)
    {
        // 获取可视化对象操作器
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;

        // 清除所有高亮
        oper->clearAllHight();

        // 设置高亮
        oper->hightObject(id);
    }

    void GUITreeWidgetBase::highlight(QList<int> ids)
    {
        // 获取可视化对象操作器
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;
        
        // 清除所有高亮
        oper->clearAllHight();

        // 逐个设置高亮
        for (int id : ids) oper->hightObject(id);
    }

    void GUITreeWidgetBase::clearAllHighlight()
    {
        // 获取可视化对象操作器
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;

        // 清除所有高亮
        oper->clearAllHight();
    }

    Radioss::FITKRadiossCase* GUITreeWidgetBase::getDataCase()
    {
        // 获取Radioss算例数据
        return FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    }
}