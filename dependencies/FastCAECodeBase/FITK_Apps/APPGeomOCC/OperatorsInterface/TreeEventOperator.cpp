/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "TreeEventOperator.h"

namespace EventOper
{
    void TreeEventOperator::updateGeometryTree()
    {
        // 根据不同子类重写。
    }

    void TreeEventOperator::mouseClicked(QTreeWidgetItem* item)
    {
        // 根据不同子类重写。
        Q_UNUSED(item);
    }

    void TreeEventOperator::mouseDoubleClicked(QTreeWidgetItem* item)
    {
        // 根据不同子类重写。
        Q_UNUSED(item);
    }
}  // namespace GraphToolOper
