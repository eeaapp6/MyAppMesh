/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   TreeEventOperator.h
 * @brief  树形菜单操作器接口类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-08-20
 *********************************************************************/
#ifndef __TREEEVENTOPERATOR_H__
#define __TREEEVENTOPERATOR_H__

#include "OperatorsInterfaceAPI.h"
#include "FITK_Kernel/FITKCore/FITKTreeWidgetOperator.h"

namespace EventOper
{
    class GraphEventOperator;
    /**
     * @brief  树形菜单操作器，抽象类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-20
     */
    class OperatorsInterfaceAPI TreeEventOperator : public Core::FITKTreeWidgetOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-20
         */
        explicit TreeEventOperator() = default;
        /**
         * @brief    析构函数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-20
         */
        virtual ~TreeEventOperator() = default;
        /**
         * @brief    更新树
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-20
         */
        virtual void updateTree();
        /**
         * @brief     树形节点状态改变事件
         * @param[i]  w              树形菜单窗口
         * @param[i]  item           点击的树形菜单节点
         * @param[i]  state          状态改变
         * @param[i]  col            点击列数
         * @author    liuzhonghua (liuzhonghuaszch@163.com)
         * @date      2025-08-20
         */
        virtual void on_itemStateChanged(QTreeWidget* w, QTreeWidgetItem * item, bool state, int column);
        /**
         * @brief     节点选择状态变化（高亮选择）
         * @param[i]  w              树形菜单窗口
         * @author    liuzhonghua (liuzhonghuaszch@163.com)
         * @date      2025-08-20
         */
        virtual void on_itemSelectionChanged(QTreeWidget* w);
        
    protected:
        /**
         * @brief    获取三维可视对象数据操作器
         * @return   GraphEventOperator *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-21
         */
        GraphEventOperator* getGraphEventOperator();
    };
}   // namespace EventOper

#endif // __GRAPHEVENTOPERATOR_H__