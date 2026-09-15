/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "TreeEventOperator.h"
#include "GraphEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace EventOper
{
    void TreeEventOperator::updateTree()
    {
        
    }

    void TreeEventOperator::on_itemStateChanged(QTreeWidget* w, QTreeWidgetItem * item, bool state, int column)
    {
        Q_UNUSED(w);
        Q_UNUSED(item);
        Q_UNUSED(state);
        Q_UNUSED(column);
        //状态改变事件
    }

    void TreeEventOperator::on_itemSelectionChanged(QTreeWidget* w)
    {
        Q_UNUSED(w);
        //节点选择状态变化（高亮选择）事件
    }

    GraphEventOperator * TreeEventOperator::getGraphEventOperator()
    {
        return FITKOPERREPO->getOperatorT<GraphEventOperator>("GraphPreprocess");
    }
}  // namespace EventOper
