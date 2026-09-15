/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __TREEWIDGET_H__
#define __TREEWIDGET_H__

#include "GUIFrameAPI.h"
#include <QTreeWidget>
#include <QContextMenuEvent>
#include <QMenu>

// 前置声明
namespace Interface
{
    class FITKAbsGeoCommand;
}

namespace GUI
{
    class MainWindow;
    class GUIFRAMEAPI TreeWidget : public QTreeWidget
    {
        Q_OBJECT

    public:
        /**
         * @brief   树节点类型。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        enum TreeItemType
        {
            NoneItemType = -1,

            // 几何节点。
            GeometryRootItem = 0,
            GeometryItem,

            // 几何分组节点。
            ComponentRootItem,
            ComponentItem
        };

        /**
         * @brief  构造函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-11
         */
        TreeWidget(MainWindow* parent = nullptr);
        /**
         * @brief  析构函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-06-11
         */
        ~TreeWidget() = default;
        /**
         * @brief  刷新树.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-30
         */
        void updateTree();
        /**
         * @brief  移除节点.
         * @param  item 节点
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-30
         */
        void removeItem(QTreeWidgetItem* item);
        /**
         * @brief  移除节点的子节点.
         * @param  item 节点
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-30
         */
        void removeChildItem(QTreeWidgetItem* item);

        /**
         * @brief   递归更新命令（模型）节点。
         * @param   item：命令（模型）节点
         * @param   geoCommnad：命令（模型）节点对应的命令
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        void updateCommandItem(QTreeWidgetItem* item, Interface::FITKAbsGeoCommand* geoCommnad);

    protected:
        /**
         * @brief  添加右键菜单.
         * @param  text 文本
         * @param  objName 名称
         * @param  id id
         * @return QAction
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-30
         */
        QAction* addContextMenuAction(const QString &text, const QString &objName, int id);
        /**
         * @brief  右键菜单事件.
         * @param  event 事件
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-30
         */
        void contextMenuEvent(QContextMenuEvent *event) override;

    private:
        /**
         * @brief   是否在建模操作状态。（建模或草绘状态下）
         * @return  是否
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        bool isModeling();

    private slots:
        /**
         * @brief   槽函数 - 树节点点击。
         * @param   index：点击到的数据[常量][引用]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        void slot_itemClicked(const QModelIndex & index);

        /**
         * @brief   槽函数 - 树节点双击。
         * @param   index：点击到的数据[常量][引用]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        void slot_itemDoubleClicked(const QModelIndex & index);

    private:
        /**
         * @brief  主窗口.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-30
         */
        MainWindow* m_MainWindow{};
        /**
         * @brief  几何根节点.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-30
         */
        QTreeWidgetItem* m_GeoRootItem{};
        /**
         * @brief  几何分组根节点.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-30
         */
        QTreeWidgetItem* m_GeoGroupRootItem{};
        /**
         * @brief  右键菜单.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-30
         */
        QMenu* m_ContextMenu{};

    };
}
#endif // !__TREEWIDGET_H__