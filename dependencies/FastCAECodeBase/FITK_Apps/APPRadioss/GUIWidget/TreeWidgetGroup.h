/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
* @file    TreeWidgetGroup.h
* @brief   分组树形菜单
* @author  lilongyuan(lilongyuan@diso.cn)
* @date    2025-09-17
*********************************************************************/
#ifndef __TREE_WIDGET_GROUP_H__
#define __TREE_WIDGET_GROUP_H__

#include "GUIWidgetAPI.h"
#include "GUIWidget/GUITreeWidgetBase.h"

namespace Interface
{
    class FITKModelSet;
    class FITKMeshSurface;
}

namespace GUI
{
    /**
     * @brief   分组树形菜单
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-17
     */
    class GUIWIDGETAPI TreeWidgetGroup : public GUITreeWidgetBase
    {
        Q_OBJECT
    public:
        /**
         * @brief     构造函数
         * @param[i]  parent 父对象
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-17
         */
        explicit TreeWidgetGroup(QWidget* parent = nullptr);
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-17
         */
        virtual ~TreeWidgetGroup() = default;
        /**
         * @brief   更新树形菜单
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-17
         */
        virtual void updateTreeWidget() override;
        /**
         * @brief   获取操作器
         * @return  Core::FITKTreeWidgetOperator* 操作器对象
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-17
         */
        virtual Core::FITKTreeWidgetOperator* getOperator() override;
    protected:
        /**
         * @brief     显示右键菜单
         * @param[i]  item 右键的item
         * @param[i]  menu 显示的menu
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-17
         */
        void on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu) override;
    private:
        /**
         * @brief     更新集合节点
         * @param[i]  itemParent 父节点
         * @param[i]  listSet
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-17
         */
        void updateSet(QTreeWidgetItem* itemParent, QList<Interface::FITKModelSet*> listSet);
        /**
         * @brief     更新表面节点
         * @param[i]  itemParent 父节点
         * @param[i]  listSurface
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-17
         */
        void updateSurface(QTreeWidgetItem* itemParent, QList<Interface::FITKMeshSurface*> listSurface);
     };
}
#endif  // __TREE_WIDGET_GROUP_H__