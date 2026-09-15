/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperResultTreeEvent.h
 * @brief  结果树事件操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-16
 *********************************************************************/
#ifndef __OPER_RESULT_TREE_EVENT_H__
#define __OPER_RESULT_TREE_EVENT_H__

#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUIOper
{
    /**
     * @brief  结果树形菜单事件处理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-21
     */
    class  OperResultTreeEvent : public EventOper::TreeEventOperator
    {
    public:
        /**
         * @brief  Construct a new Operator Result Tree Event object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-21
         */
        explicit OperResultTreeEvent() = default;
        /**
         * @brief  Destroy the Operator Result Tree Event object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-21
         */
        virtual ~OperResultTreeEvent() = default;
        /**
         * @brief    更新树
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-21
         */
        virtual void updateTree() override;
        /**
         * @brief     树形节点点击事件
         * @param[i]  w              树形菜单窗口
         * @param[i]  item           点击的树形菜单节点
         * @param[i]  col            点击列数
         * @author    liuzhonghua (liuzhonghuaszch@163.com)
         * @date      2025-08-21
         */
        virtual void on_itemClicked(QTreeWidget* w, QTreeWidgetItem* item, int col = 0) override;
        /**
         * @brief     树形节点双击事件
         * @param[i]  w              树形菜单窗口
         * @param[i]  item           点击的树形菜单节点
         * @param[i]  col            点击列数
         * @author    liuzhonghua (liuzhonghuaszch@163.com)
         * @date      2025-08-21
         */
        virtual void on_itemDoubleClicked(QTreeWidget* w, QTreeWidgetItem* item, int col = 0) override;
        /**
         * @brief     树形节点状态改变事件
         * @param[i]  w              树形菜单窗口
         * @param[i]  item           点击的树形菜单节点
         * @param[i]  state          状态改变
         * @param[i]  col            点击列数
         * @author    liuzhonghua (liuzhonghuaszch@163.com)
         * @date      2025-08-21
         */
        virtual void on_itemStateChanged(QTreeWidget* w, QTreeWidgetItem * item, bool state, int column) override;

    protected:

        void resetViewport();
    };
    //注册事件操作器
    Register2FITKOperatorRepo(ResultTreeEvent, OperResultTreeEvent);
}
#endif  // __OPER_PROPERTY_TREE_EVENT_H__