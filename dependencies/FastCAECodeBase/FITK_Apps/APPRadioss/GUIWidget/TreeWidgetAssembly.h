/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
* @file    TreeWidgetAssembly.h
* @brief   装配树形菜单类
* @author  lilongyuan(lilongyuan@diso.cn)
* @date    2025-09-10
*********************************************************************/
#ifndef __TREE_WIDGET_ASSEMBLY_H__
#define __TREE_WIDGET_ASSEMBLY_H__

#include "FITK_Component/FITKWidget/FITKTreeWidget.h" 
#include "GUIWidgetAPI.h"
#include "GUIWidget/GUITreeWidgetBase.h"

namespace Interface
{
    class FITKGeoCommandList;
    class FITKAbsGeoCommand;
}

namespace Radioss
{
    class FITKRadiossPartManager;
    class FITKRadiossPart;
    class FITKRadiossGeomPartManager;
}

class QTreeWidgetItem;

namespace GUI
{
    /**
     * @brief   装配树形菜单类
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-10
     */
    class GUIWIDGETAPI TreeWidgetAssembly : public GUITreeWidgetBase
    {
        Q_OBJECT
    public:
        /**
         * @brief     构造函数
         * @param[i]  parent 父对象
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-10
         */
        explicit TreeWidgetAssembly(QWidget* parent = nullptr);
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-10
         */
        virtual ~TreeWidgetAssembly() = default;
        /**
         * @brief   获取操作器
         * @return  Core::FITKTreeWidgetOperator*
         * @author  LiBaojun (libaojunqd@foxmail.com)
         * @date    2024-12-28
         */
        virtual Core::FITKTreeWidgetOperator* getOperator() override;
        /**
         * @brief   更新树形菜单
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-24
         */
        virtual void updateTreeWidget() override;
    protected:
        /**
         * @brief     鼠标释放事件
         * @param[i]  event 鼠标事件
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-25
         */
        void mouseReleaseEvent(QMouseEvent* event) override;
        /**
         * @brief     树节点状态改变事件
         * @param[i]  item
         * @param[i]  state
         * @param[i]  column
         * @return    void
         * @author    liuzhonghua (liuzhonghuaszch@163.com)
         * @date      2025-08-21
         */
        void on_itemStateChanged(QTreeWidgetItem* item, bool state, int column) override;
        /**
         * @brief     显示右键菜单
         * @param[i]  item 右键的item
         * @param[i]  menu 显示的menu
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-10
         */
        void on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu) override;
    private slots:
        /**
         * @brief   选色按钮点击事件
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-10
         */
        void on_buttonColor_clicked();
        /**
         * @brief     材料下拉框选项选取事件
         * @param[i]  index 当前选项索引值
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-10
         */
        void on_comboBoxMaterial_activated(int index);
        /**
         * @brief     属性下拉框选项选取事件
         * @param[i]  index 当前选项索引值
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-10
         */
        void on_comboBoxProperty_activated(int index);
    private:
        /**
         * @brief     更新树形菜单的几何节点
         * @param[i]  itemParent 几何根节点
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-10
         */
        void updateTreeGeometry(QTreeWidgetItem* itemParent);
        /**
         * @brief     更新树形菜单的部件节点
         * @param[i]  itemParent 部件根节点
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-10
         */
        void updateTreePart(QTreeWidgetItem* itemParent);
        /**
         * @brief     填充颜色
         * @param[i]  item 需要填充的节点
         * @param[i]  data 部件数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-10
         */
        void fillColor(QTreeWidgetItem* item, Radioss::FITKRadiossPart* data);
        /**
         * @brief     填充颜色
         * @param[i]  item 需要填充的节点
         * @param[i]  data 几何数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-10
         */
        void fillColor(QTreeWidgetItem* item, Interface::FITKAbsGeoCommand* data);
        /**
         * @brief     填充材料
         * @param[i]  item 需要填充的节点
         * @param[i]  data 部件数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-10
         */
        void fillMaterial(QTreeWidgetItem* item, Radioss::FITKRadiossPart* data);
        /**
         * @brief     填充属性
         * @param[i]  item 需要填充的节点
         * @param[i]  data 部件数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-10
         */
        void fillProperty(QTreeWidgetItem* item, Radioss::FITKRadiossPart* data);
    private:
        /**
        * @brief   获取几何管理器
        * @return  Radioss::FITKRadiossGeomPartManager* 几何管理器
        * @author  libaoijun
        * @date    2025-11-11
        */
        Radioss::FITKRadiossGeomPartManager* getManagerGeometry();
        /**
         * @brief   获取部件管理器
         * @return  Radioss::FITKRadiossPartManager* 部件管理器
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-12
         */
        Radioss::FITKRadiossPartManager* getManagerPart();
        /**
         * @brief    获取几何命令列表
         * @return   几何命令列表
         * @author   WuYizhen (wyz_hanhan@163.com)
         * @date     2025-11-21
         */
        Interface::FITKGeoCommandList* getCommandManagerGeo();
    };
}
#endif  // __TREE_WIDGET_ASSEMBLY_H__