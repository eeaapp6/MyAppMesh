/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ResultTreeWidget.h
 * @brief  结果树窗口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-15
 *********************************************************************/
#ifndef __RESULT_TREE_WIDGET_H__
#define __RESULT_TREE_WIDGET_H__

#include "FITK_Component/FITKWidget/FITKTreeWidget.h" 
#include "GUIWidgetAPI.h"
#include "GUIWidget/GUITreeWidgetBase.h"

class QTreeWidgetItem;

namespace GUI
{
    /**
     * @brief   结果树形菜单
     * @author  liuzhonghua (liuzhonghuaszch@163.com)
     * @date    2025-09-15
     */
    class GUIWIDGETAPI ResultTreeWidget : public GUITreeWidgetBase//Comp::FITKTreeWidget, 
    {
        Q_OBJECT
    public:
        /**
         * @brief     构造函数
         * @param[i]  parent 父对象
         * @author    liuzhonghua (liuzhonghuaszch@163.com)
         * @date      2025-09-15
         */
        explicit ResultTreeWidget(QWidget* parent = nullptr);
        /**
         * @brief   析构函数
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-09-15
         */
        virtual ~ResultTreeWidget() = default;
        /**
         * @brief   获取操作器
         * @return  Core::FITKTreeWidgetOperator*
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-09-15
         */
        virtual Core::FITKTreeWidgetOperator* getOperator() override;
        /**
         * @brief   更新界面
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-09-15
         */
        virtual void updateTreeWidget() override;
        /**
         * @brief    显示右键菜单
         * @param[i] items 
         * @param[i] menu 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-10-21
         */
        virtual void on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu) override;
     };
}
#endif  // __PART_TREE_WIDGET_H__