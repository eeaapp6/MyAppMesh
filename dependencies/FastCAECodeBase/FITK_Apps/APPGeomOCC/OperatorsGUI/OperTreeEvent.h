/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperTreeEvent.h
 * @brief   前处理可视化工具操作器。
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-10-24
 *********************************************************************/

#ifndef __OPERTREEEVENT_H__
#define __OPERTREEEVENT_H__

#include "OperatorsInterface/TreeEventOperator.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

 // 前置声明
namespace GUI
{
    class TreeWidget;
}

namespace GUIOper
{
    /**
     * @brief   前处理可视化工具操作器。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-24
     */
    class OperTreeEvent : public EventOper::TreeEventOperator
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        explicit OperTreeEvent() = default;

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        ~OperTreeEvent() = default;

        /**
         * @brief   刷新模型树。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        void updateGeometryTree() override;

        /**
         * @brief   树节点点击事件。[重写]
         * @param   item：点击到的节点
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        void mouseClicked(QTreeWidgetItem* item) override;

        /**
         * @brief   树节点双击事件。[重写]
         * @param   item：双击到的节点
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        void mouseDoubleClicked(QTreeWidgetItem* item) override;

    private:
        /**
         * @brief   获取模型树。
         * @return  模型树。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        GUI::TreeWidget* getGeometryTreeWidget();

    };

    Register2FITKOperatorRepo(TreeEvent, OperTreeEvent);

}  // namespace GUIOper

#endif // __OPERTREEEVENT_H__
