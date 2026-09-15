/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   TreeWidget.h
 * @brief  树界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-04
 *********************************************************************/
#ifndef _TreeWidget_H
#define _TreeWidget_H

#include "GUIWidgetAPI.h"
#include <QTreeWidget>

namespace Interface {
    class FITKCFDPost3DManager;
    class PostGraphObjectManager;
}

namespace GUI
{
    /**
     * @brief  树界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-04
     */
    class GUIWidgetAPI TreeWidget : public QTreeWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent  父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        TreeWidget(QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        ~TreeWidget();
        /**
         * @brief    更新树
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        void updateTree();
        /**
         * @brief    获取当前数据对象id
         * @return   int            id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        int getCurrentDataID();
    private slots:
        ;
        /**
         * @brief    树节点点击事件
         * @param[i] item    节点对象
         * @param[i] row     索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-04
         */
        void slotItemClecked(QTreeWidgetItem* item, int index);
        /**
         * @brief    鼠标右键点击事件
         * @param[i] point 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        void onModelCustomContextMenu(QPoint point);
        /**
         * @brief    按钮点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        void slotAcitonClicked();
    private:
        /**
         * @brief    添加action
         * @param[i] menu           Qmenu对象
         * @param[i] actions        action对象
         * @param[i] objectName     action名称
         * @param[i] isEnable       action是否可用
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        void addMenuActions(QMenu& menu, QString actions, QString objectName, bool isEnable = true);
    private:
        /**
         * @brief  根节点
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-04
         */
        QTreeWidgetItem* _rootItem = nullptr;
        /**
         * @brief  当前节点数据对象id
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        int _currentObjID = -1;
        /**
         * @brief  后处理数据管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        Interface::FITKCFDPost3DManager* _postDataManager = nullptr;
        /**
         * @brief  后处理可视化数据管理
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-08
         */
        Interface::PostGraphObjectManager* _postGraphManager = nullptr;
        /**
         * @brief  树节点存储
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-08
         */
        QHash<int, QTreeWidgetItem*> _itemHash = {};
    };
}

#endif