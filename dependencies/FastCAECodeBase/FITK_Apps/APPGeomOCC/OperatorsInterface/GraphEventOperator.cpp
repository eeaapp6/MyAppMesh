/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphEventOperator.h"

namespace EventOper
{
    void GraphEventOperator::initialize3D()
    {
        // 根据不同子类重写。
    }

    void GraphEventOperator::updateGraph(int dataObjId, bool forceUpdate)
    {
        // 根据不同子类重写。
        Q_UNUSED(dataObjId);
        Q_UNUSED(forceUpdate);
    }

    void GraphEventOperator::updateGraphs()
    {
        // 根据不同子类重写。
    }

    void GraphEventOperator::updateGraphRecursively(int dataObjId, int refLayer, bool recDown)
    {
        // 根据不同子类重写。
        Q_UNUSED(dataObjId);
        Q_UNUSED(refLayer);
        Q_UNUSED(recDown);
    }

    void GraphEventOperator::updateGraphVisibility(int dataObjId)
    {
        // 根据不同子类重写。
        Q_UNUSED(dataObjId);
    }

    void GraphEventOperator::preview(int dataObjId, PreviewType type, QColor color)
    {
        // 根据不同子类重写。
        Q_UNUSED(dataObjId);
        Q_UNUSED(type);
        Q_UNUSED(color);
    }

    void GraphEventOperator::clearPreview(int dataObjId)
    {
        // 根据不同子类重写。
        Q_UNUSED(dataObjId);
    }

    void GraphEventOperator::showGlobalComponent(int dataObjId)
    {
        // 根据不同子类重写。
        Q_UNUSED(dataObjId);
    }

    void GraphEventOperator::clearGlobalComponent()
    {
        // 根据不同子类重写。
    }

    void GraphEventOperator::setDatumsVisibility(bool visible)
    {
        // 根据不同子类重写。
        Q_UNUSED(visible);
    }

    void GraphEventOperator::setSketchTopMost(int dataObjId)
    {
        // 根据不同子类重写。
        Q_UNUSED(dataObjId);
    }

    void GraphEventOperator::clearSketchTopMost()
    {
        // 根据不同子类重写。
    }

    void GraphEventOperator::setShapeTransparency(int cmdId, int geomType, QList<int> geoIndice, double trans)
    {
        // 根据不同子类重写。
        Q_UNUSED(cmdId);
        Q_UNUSED(geomType);
        Q_UNUSED(geoIndice);
        Q_UNUSED(trans);
    }

    void GraphEventOperator::setShapeHidden(int cmdId, int geomType, QList<int> geoIndice)
    {
        // 根据不同子类重写。
        Q_UNUSED(cmdId);
        Q_UNUSED(geomType);
        Q_UNUSED(geoIndice);
    }

    void GraphEventOperator::clearShapeFaceTransparency()
    {
        // 根据不同子类重写。
    }

    void GraphEventOperator::highlight(int dataObjId, QVariant info, QColor color)
    {
        // 根据不同子类重写。
        Q_UNUSED(dataObjId);
        Q_UNUSED(info);
        Q_UNUSED(color);
    }

    void GraphEventOperator::clearHighlight()
    {
        // 根据不同子类重写。
    }

    void GraphEventOperator::reRender(bool fitView)
    {
        // 根据不同子类重写。
        Q_UNUSED(fitView);
    }
    bool GraphEventOperator::enableSketchMode(bool isOn, int virtualTopoID)
    {
        // 根据不同子类重写。
        return false;
    }

    bool GraphEventOperator::enableSketchMode(bool isOn, double* pos, double* nor, double* up)
    {
        // 根据不同子类重写。
        Q_UNUSED(isOn);
        Q_UNUSED(pos);
        Q_UNUSED(nor);
        Q_UNUSED(up);
        return false;
    }
}  // namespace EventOper
