/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKTreeWidget.h
 * @brief 树形菜单窗口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-10-08
 * 
 */
#ifndef _FITK_TREE_WIDGET_H___
#define _FITK_TREE_WIDGET_H___
 
#include "FITKWidgetAPI.h"
#include <QTreeWidget>
#include "FITK_Kernel/FITKCore/FITKAbstractGUIObject.h"

class QMenu;
class QAction;
class QTimer;

namespace Core
{
    class FITKAbstractOperator;
    class FITKTreeWidgetOperator;
}

namespace Comp
{
    /**
     * @brief 树形菜单窗口
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-10-08
     */
    class FITKWidgetAPI FITKTreeWidget : public QTreeWidget, public Core::FITKAbstractGUIObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKTreeWidget object
         * @param[i]  parent         父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        explicit FITKTreeWidget(QWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKTreeWidget object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        virtual ~FITKTreeWidget();
        /**
         * @brief    根据类型获取对应的item
         * @param[i] parent                           父界面
         * @param[i] type                             类型
         * @return   QTreeWidgetItem*                 TreeItem
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-15
         */
        static QTreeWidgetItem* getTreeWidgetItemByType(QTreeWidget* parent, int type);
        /**
         * @brief    创建树形菜单根节点项
         * @param[i]  parent         父对象
         * @param[i] text 显示文本
         * @param[i] type 类型标记
         * @param[i] icon 图标
         * @param[i] state 状态 : -1不使用 0不选择 1选择
         * @return   QTreeWidgetItem *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-22
         */
        static QTreeWidgetItem* CreateTreeRootItem(QTreeWidget* parent, const QString & text,
            int type = 0, const QString& icon = "", int state = -1);
        /**
         * @brief 创建树形菜单项
         * @param[i]  parent         父对象
         * @param[i]  text           显示文本
         * @param[i]  type           类型标记
         * @param[i]  icon           图标
         * @param[i]  state 状态 : -1不使用 0不选择 1选择
         * @return QTreeWidgetItem* 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        static QTreeWidgetItem* CreateTreeItem(QTreeWidgetItem* parent, const QString & text,
            int type = 0, const QString& icon = "", int state = -1);
        /**
         * @brief 获取操作器
         * @param[i]  operName       操作器名称
         * @return Core::FITKAbstractOperator* 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        static Core::FITKAbstractOperator* GetOperator(const QString& operName);

        template <class T>
        /**
         * @brief 获取操作器，并进行强制类型转换
         * @param[i]  operName       操作器名称
         * @return T* 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        T* getOperatorT(const QString & operName)
        {
            return dynamic_cast<T*> (this->GetOperator(operName));
        }
        /**
         * @brief 获取操作器
         * @return Core::FITKTreeWidgetOperator*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-28
         */
        virtual Core::FITKTreeWidgetOperator* getOperator();
        /**
         * @brief 设置双击间隔
         * @param[i]  msec           间隔时间，单位：毫秒
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-01-12
         */
        void setDoubleClickInterval(int msec);
        /**
         * @brief 获取双击间隔
         * @return int 双击间隔，单位：毫秒
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-01-12
         */
        int getDoubleClickInterval() const;

    protected:
        /**
         * @brief 树形菜单节点点击事件，子类重写实现
         * @param[i]  item           点击对象
         * @param[i]  column         列数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        virtual void on_itemClicked(QTreeWidgetItem *item, int column);

        
        /**
         * @brief 树形菜单节点双击事件，子类重写实现
         * @param[i]  item           点击对象
         * @param[i]  column         列数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        virtual void on_itemDoubleClicked(QTreeWidgetItem *item, int column);
        /**
         * @brief    树形菜单节点状态改变事件，子类重写实现
         * @param[i] item 点击对象
         * @param[i] state 改变之后的状态
         * @param[i] column 列数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-22
         */
        virtual void on_itemStateChanged(QTreeWidgetItem *item, bool state, int column);
        /**
         * @brief 树形菜单节点右键菜单事件，子类重写实现
         * @param[i]  item           点击对象
         * @param[i]  menu           右键菜单
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-26
         */ 
        virtual void on_itemContextMenu(const QList<QTreeWidgetItem*>& item, QMenu* menu);
    private slots:
        /**
         * @brief 树形菜单节点点击槽函数。调用on_itemClicked
         * @param[i]  item           点击对象
         * @param[i]  column         列数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        void slot_itemClicked(QTreeWidgetItem *item, int column);

        /**
         * @brief 处理树形菜单节点点击事件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-01-12
         */
        void slot_processItemClickEvent();
        /**
         * @brief 树形菜单节点双击槽函数。调用on_itemDoubleClicked
         * @param[i]  item           点击对象
         * @param[i]  column         列数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        void slot_itemDoubleClicked(QTreeWidgetItem *item, int column);
        /**
         * @brief    树形菜单节点状态改变槽函数。调用on_itemStateChanged
         * @param[i] item 点击对象
         * @param[i] column 列数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-22
         */
        void slot_itemStateChanged(QTreeWidgetItem *item, int column);
        /**
         * @brief 树形菜单节点右键菜单槽函数。调用on_itemContextMenu
         * @param[i]  p           点击位置
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-26
         */
        void slot_customContextMenuRequested(const QPoint& p);
        /**
         * @brief 树形菜单节点右键菜单按钮被点击
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-26
         */
        void slot_treeActionTriggered();


        private:
            QTimer* m_clickTimer{};  // 定时器，单机双击使用
            QTreeWidgetItem* m_currentItem{}; // 当前点击的项，单击时临时记录
            int m_currentColumn{0};   // 当前点击的列，单击时临时记录
            int m_doubleClickInterval{200}; // 双击间隔, 毫秒
    };

}

#endif
