/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphToolOperator.h"

namespace EventOper
{
    void GraphToolOperator::measure(QList<int> items, MeasureToolType type, QColor color)
    {
        // 根据不同子类重写。
        Q_UNUSED(items);
        Q_UNUSED(type);
        Q_UNUSED(color);
    }

    void GraphToolOperator::clearMeasure()
    {
        // 根据不同子类重写。
    }

    void GraphToolOperator::initPickTool(bool calculateCenter, PickToolType type, QColor color)
    {
        // 根据不同子类重写。
        Q_UNUSED(calculateCenter);
        Q_UNUSED(type);
        Q_UNUSED(color);
    }

    void GraphToolOperator::clearPickTool()
    {
        // 根据不同子类重写。
    }

    Exchange::FITKOCCGraphObject3D* GraphToolOperator::getPickToolObject(PickToolType type)
    {
        // 根据不同子类重写。
        Q_UNUSED(type);
        return nullptr;
    }

    void GraphToolOperator::setPickDirectionAnchorTopoId(int mainId, int subId)
    {
        // 根据不同子类重写。
        Q_UNUSED(mainId);
        Q_UNUSED(subId);
    }

    void GraphToolOperator::setPickToolObjectVisible(PickToolType type, bool visible)
    {
        // 根据不同子类重写。
        Q_UNUSED(type);
        Q_UNUSED(visible);
    }

    void GraphToolOperator::setPickDirectionUserDirection(QList<double> dir)
    {
        // 根据不同子类重写。
        Q_UNUSED(dir);
    }

    bool GraphToolOperator::getPickedDirection(QList<double> & dir)
    {
        // 根据不同子类重写。
        Q_UNUSED(dir);
        return false;
    }

    bool GraphToolOperator::getPickedProjPosition(QList<double> & pos)
    {
        // 根据不同子类重写。
        Q_UNUSED(pos);
        return false;
    }

    void GraphToolOperator::setPickProjPosTargetId(int targetCmdId, int targetTopoId)
    {
        // 根据不同子类重写。
        Q_UNUSED(targetCmdId);
        Q_UNUSED(targetTopoId);
    }
}  // namespace GraphToolOper
