/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
* @file    TreeWidgetSolution.h
* @brief   解决方案树形菜单
* @author  lilongyuan(lilongyuan@diso.cn)
* @date    2025-09-15
*********************************************************************/
#ifndef __TREE_WIDGET_SOLUTION_H__
#define __TREE_WIDGET_SOLUTION_H__

#include "GUIWidget/GUITreeWidgetBase.h"
#include "GUIWidgetAPI.h"

namespace Core { class FITKAbstractNDataObject; }
namespace Radioss { class FITKRadiossSolution; }

namespace GUI
{
    /**
     * @brief   解决方案树形菜单
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-15
     */
    class GUIWIDGETAPI TreeWidgetSolution : public GUITreeWidgetBase
    {
        Q_OBJECT
    public:
        /**
         * @brief     构造函数
         * @param[i]  parent 父窗口
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-15
         */
        explicit TreeWidgetSolution(QWidget* parent = nullptr);
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-15
         */
        virtual ~TreeWidgetSolution() = default;
        /**
         * @brief   更新树形菜单（重写）
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-15
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
         * @brief     树节点状态改变事件
         * @param[i]  item 树节点
         * @param[i]  state 状态
         * @param[i]  column 列号
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-23
         */
        void on_itemStateChanged(QTreeWidgetItem* item, bool state, int column) override;
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
         * @brief     更新设置子节点
         * @param[i]  itemParent 父节点
         * @param[i]  solution 解决方案数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-17
         */
        void updateSetting(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief     更新接触子节点
         * @param[i]  itemParent 父节点
         * @param[i]  solution 解决方案数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-26
         */
        void updateContact(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief     更新初始条件子节点
         * @param[i]  itemParent 父节点
         * @param[i]  solution 解决方案数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-17
         */
        void updateInitialCondition(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief     更新载荷子节点
         * @param[i]  itemParent 父节点
         * @param[i]  solution 解决方案数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-17
         */
        void updateLoad(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief     更新约束子节点
         * @param[i]  itemParent 父节点
         * @param[i]  solution 解决方案数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-17
         */
        void updateConstraint(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief    更新边界子节点
         * @param[i] itemParent 
         * @param[i] solution 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-04
         */
        void updateBoundary(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief    更新连接子节点
         * @param[i] itemParent 
         * @param[i] solution 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        void updateConnection(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief     更新相互作用数据
         * @param[i]  itemParent 父节点
         * @param[i]  solution 解决方案数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-26
         */
        void updateTreeWidgetInteraction(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief     更新初始化场数据
         * @param[i]  itemParent 父节点
         * @param[i]  solution 解决方案数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-16
         */
        void updateTreeWidgetInitialField(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief     更新刚性墙数据
         * @param[i]  itemParent 父节点
         * @param[i]  solution 解决方案数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-16
         */
        void updateTreeWidgetRigidWall(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief     更新重力场数据
         * @param[i]  itemParent 父节点
         * @param[i]  solution 解决方案数据
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-16
         */
        void updateTreeWidgetGravity(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief    更新边界条件数据
         * @param[i] itemParent 
         * @param[i] solution 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-04
         */
        void updateTreeWidgetBCS(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief    更新连接数据
         * @param[i] itemParent 
         * @param[i] solution 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        void updateTreeWidgetConnection(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);

        /**
         * @brief    更新加速度计子节点
         * @param[i] itemParent
         * @param[i] solution
         * @return   void
         */
        void updateAccel(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);
        /**
         * @brief    更新截面力子节点
         * @param[i] itemParent
         * @param[i] solution
         * @return   void
         */
        void updateSection(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution);

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
#endif  // __TREE_WIDGET_SOLUTION_H__