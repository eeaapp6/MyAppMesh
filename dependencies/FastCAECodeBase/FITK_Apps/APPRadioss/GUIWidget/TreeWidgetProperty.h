/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
* @file    TreeWidgetProperty.h
* @brief   属性树形菜单
* @author  lilongyuan(lilongyuan@diso.cn)
* @date    2025-09-16
*********************************************************************/
#ifndef __TREE_WIDGET_PROPERTY_H__
#define __TREE_WIDGET_PROPERTY_H__

#include "GUIWidgetAPI.h"
#include "GUIWidget/GUITreeWidgetBase.h"

namespace Core { class FITKAbstractNDataObject; }
class QTreeWidgetItem;

namespace GUI
{
    /**
     * @brief   属性树形菜单
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-16
     */
    class GUIWIDGETAPI TreeWidgetProperty : public GUITreeWidgetBase
    {
        Q_OBJECT
    public:
        /**
         * @brief     构造函数
         * @param[i]  parent 父对象
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-16
         */
        explicit TreeWidgetProperty(QWidget* parent = nullptr);
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-16
         */
        virtual ~TreeWidgetProperty() = default;
        /**
         * @brief   更新树形菜单
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-16
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
         * @date      2025-09-16
         */
        virtual void on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu) override;
    private:
        /**
         * @brief     更新材料节点
         * @param[i]  itemParent 父节点
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-16
         */
        void updateTreeWidgetMaterial(QTreeWidgetItem* itemParent);
        /**
         * @brief     更新属性节点
         * @param[i]  itemParent 父节点
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-16
         */
        void updateTreeWidgetProperty(QTreeWidgetItem* itemParent);
        /**
         * @brief     更新曲线节点
         * @param[i]  itemParent 父节点
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-16
         */
        void updateTreeWidgetCurve(QTreeWidgetItem* itemParent);
        /**
         * @brief    更新状态方程
         * @param[i] itemParent 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-04
         */
        void updateTreeWidgetEquationOfState(QTreeWidgetItem* itemParent);
        /**
        * @brief    更新失效
        * @param[i] itemParent
        * @return   void
        * @author   liuzhonghua (liuzhonghuaszch@163.com)
        * @date     2025-11-04
        */
        void updateTreeWidgetFailureModel(QTreeWidgetItem* itemParent);
        /**
         * @brief     获取rad文件关键字
         * @param[i]  data Radioss数据对象
         * @return    QString 关键字
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-16
         */
        QString getRadiossKeyWord(Core::FITKAbstractNDataObject* data);
    };
}
#endif  // __TREE_WIDGET_PROPERTY_H__