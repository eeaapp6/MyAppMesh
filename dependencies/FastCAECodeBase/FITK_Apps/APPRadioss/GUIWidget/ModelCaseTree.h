/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file ModelCaseTree.h
 * @brief 数据与TreeWidget绑定
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-12-27
 *
 */
#ifndef _MODEL_CASE_TREE_H___
#define _MODEL_CASE_TREE_H___
 
#include "GUIEnumType.h"
#include <QList>

class QTreeWidgetItem;
class QMenu;


namespace GUI
{
   /**
    * @brief 数据与TreeWidget绑定类
    * @author LiBaojun (libaojunqd@foxmail.com)
    * @date 2024-12-27
    */
    class ModelCaseTree
    {
    public:
       /**
        * @brief Create
        * @param parentItem 最上层父节点
        * @param caseData model数据
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-12-27
        */
        explicit ModelCaseTree(QTreeWidgetItem* parentItem/*,AbaqusData::FITKDataCase* caseData*/);

       /**
        * @brief Destory
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-12-27
        */
        virtual ~ModelCaseTree();

       /**
        * @brief 左键点击
        * @param item 点击的item
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-12-27
        */
        void itemClicked(QTreeWidgetItem* item);

       /**
        * @brief 右键点击
        * @param item 点击选中的items
        * @param menu 显示的菜单
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-12-27
        */
        void contextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu);

       /**
        * @brief 更新树的数据
        * @param type 类型
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-12-27
        */
        void updateTree(GUI::PartTreeItemType type);

    private:
       /**
        * @brief 初始化树
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-12-27
        */
        void initTree();

       /**
        * @brief 给节点添加一些信息
        * @param item 节点
        * @param dataObjID 节点对应的数据的id
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-12-27
        */
        void addTreeItemInfo(QTreeWidgetItem* item, const int dataObjID);


    private:
        /**
        * @brief 最上层的父节点
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-12-27
        */
        QTreeWidgetItem* _parentItem{};
        
        /**
        * @brief 模型数据
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-12-27
        */
        //AbaqusData::FITKDataCase* _caseData{};

        /**
        * @brief 模型数据
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-12-27
        */
        QTreeWidgetItem* _caseRoot{};
    };
}


#endif
