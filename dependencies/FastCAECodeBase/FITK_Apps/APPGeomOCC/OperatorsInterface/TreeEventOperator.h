/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    TreeEventOperator.h
 * @brief   模型树事件操作器。
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-10-24
 *********************************************************************/

#ifndef __TREEEVENTOPERATOR_H__
#define __TREEEVENTOPERATOR_H__

#include "FITK_Kernel/FITKCore/FITKAbstractOperator.h"

#include "OperatorsInterfaceAPI.h"

#include <QColor>
#include <QVariant>

// 前置声明。
class QTreeWidgetItem;

namespace EventOper
{
    /**
     * @brief   模型树事件操作器类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-24
     */
    class OperatorsInterfaceAPI TreeEventOperator : public Core::FITKAbstractOperator
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        explicit TreeEventOperator() = default;

        /**
         * @brief   析构函数。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        virtual ~TreeEventOperator() = default;      

        /**
         * @brief   刷新模型树。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        virtual void updateGeometryTree();

        /**
         * @brief   树节点点击事件。[虚函数]
         * @param   item：点击到的节点
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        virtual void mouseClicked(QTreeWidgetItem* item);

        /**
         * @brief   树节点双击事件。[虚函数]
         * @param   item：双击到的节点
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        virtual void mouseDoubleClicked(QTreeWidgetItem* item);

    };
}   // namespace EventOper

#endif // __TREEEVENTOPERATOR_H__