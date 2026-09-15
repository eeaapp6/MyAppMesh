/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    OperTreeAssemblyEvent.h
 * @brief   装配树形菜单事件处理器
 * @author  lilongyuan(lilongyuan@diso.cn)
 * @date    2025-09-25
 *********************************************************************/
#ifndef __OPER_TERR_ASSEMBLY_EVENT_H__
#define __OPER_TERR_ASSEMBLY_EVENT_H__

#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUIOper
{
    /**
     * @brief   Assembly树形菜单事件处理器
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-25
     */
    class  OperTreeAssemblyEvent : public EventOper::TreeEventOperator
    {
    public:
        /**
         * @brief   构造函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-25
         */
        explicit OperTreeAssemblyEvent() = default;
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-25
         */
        virtual ~OperTreeAssemblyEvent() = default;
        /**
         * @brief   更新树（重写）
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-25
         */
        virtual void updateTree() override;
        /**
         * @brief     树形节点状态改变事件
         * @param[i]  w 树形菜单窗口
         * @param[i]  item 点击的树形菜单节点
         * @param[i]  state 状态改变
         * @param[i]  col 点击列数
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-25
         */
        virtual void on_itemStateChanged(QTreeWidget* w, QTreeWidgetItem* item, bool state, int column) override;
    };
    //注册事件操作器
    Register2FITKOperatorRepo(TreeAssemblyEvent, OperTreeAssemblyEvent);
}
#endif  // __OPER_TERR_ASSEMBLY_EVENT_H__