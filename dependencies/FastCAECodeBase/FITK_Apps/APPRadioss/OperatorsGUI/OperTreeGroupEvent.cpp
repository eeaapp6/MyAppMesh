/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperTreeGroupEvent.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"
#include "GUIWidget/TreeWidgetGroup.h"

namespace GUIOper
{
    void OperTreeGroupEvent::updateTree()
    {
        // 获取主窗口
        GUI::MainWindow* mainWindow = GUI::MainWindow::GetMainWindowFromFramework();
        if (mainWindow == nullptr) return;

        // 获取控制面板
        GUI::ControlPanel* controlPanel = mainWindow->getControlPanel();
        if (controlPanel == nullptr) return;

        // 获取分组树
        GUI::TreeWidgetGroup* tree = controlPanel->getGroupTree();
        if (tree == nullptr) return;

        // 更新分组树
        tree->updateTreeWidget();
    }

    void OperTreeGroupEvent::on_itemClicked(QTreeWidget * w, QTreeWidgetItem * item, int col)
    {
        Q_UNUSED(col);

        //点击树节点事件处理
        EventOper::GraphEventOperator* oper = getGraphEventOperator();
        if (oper == nullptr) return;
        oper->clearAllHight();

        //点击树节点事件处理(高亮接口)
        if (item == nullptr || w == nullptr) return;

        // 初始化ID
        int id = item->data(1, 0).toInt();
        if (id > 0) oper->hightObject(id);
    }
}