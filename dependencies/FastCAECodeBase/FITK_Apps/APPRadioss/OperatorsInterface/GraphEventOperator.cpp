/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphEventOperator.h"

namespace EventOper
{
    void GraphEventOperator::updateAll(bool forceUpdate)
    {
        Q_UNUSED(forceUpdate);
    }

    void GraphEventOperator::updateGraph(int dataObjId, bool forceUpdate, bool isFitView)
    {
        // 根据不同子类重写。
        Q_UNUSED(dataObjId);
        Q_UNUSED(forceUpdate);
        Q_UNUSED(isFitView);
    }

    void GraphEventOperator::updateAllModel(bool forceUpate)
    {
        Q_UNUSED(forceUpate);
    }

    void GraphEventOperator::updateAllInstance(bool forceUpate)
    {
        Q_UNUSED(forceUpate);
    }

    Exchange::GraphVTKObject3D * GraphEventOperator::getModelGraphObjectByDataId(int dataObjId)
    {
        Q_UNUSED(dataObjId);
        return nullptr;
    }


    void GraphEventOperator::updateGraphRecursively(int dataObjId, int refLayer, bool recDown)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(refLayer);
        Q_UNUSED(recDown);
    }

    void GraphEventOperator::setModelVisible(int dataObjId, bool visibility, bool isFitView)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(visibility);
        Q_UNUSED(isFitView);
    }

    void GraphEventOperator::setPointVisible(bool visibility, bool isPart, bool isFitView)
    {
        Q_UNUSED(visibility);
        Q_UNUSED(isPart);
        Q_UNUSED(isFitView);
    }

    void GraphEventOperator::setLineVisible(bool visibility, bool isPart, bool isFitView)
    {
        Q_UNUSED(visibility);
        Q_UNUSED(isPart);
        Q_UNUSED(isFitView);
    }

    void GraphEventOperator::setAllModelVisible(bool visibility, bool isPart, bool isFitView)
    {
        Q_UNUSED(visibility);
        Q_UNUSED(isPart);
        Q_UNUSED(isFitView);
    }

    void GraphEventOperator::setAllInstanceVisible(bool visibility, bool isFitView)
    {
        Q_UNUSED(visibility);
        Q_UNUSED(isFitView);
    }

    void GraphEventOperator::hightObject(int dataObjId, QColor color)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(color);
    }

    void GraphEventOperator::hightSetObject(int dataObjId, int parentID, QColor color)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(parentID);
        Q_UNUSED(color);
    }

    void GraphEventOperator::disHightObject(int dataObjId, int parentID)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(parentID);
    }

    void GraphEventOperator::clearAllHight()
    {

    }

    void GraphEventOperator::render()
    {

    }

    void GraphEventOperator::fitView()
    {

    }

    void GraphEventOperator::updateColor(int dataObjId, QColor color, QVariant otherPar)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(otherPar);
        Q_UNUSED(color);
    }

    void GraphEventOperator::updateAllColor(int dataObjId, QColor color)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(color);
    }
    void GraphEventOperator::updatePointSize(int dataObjId, int pointSize)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(pointSize);
    }
    void GraphEventOperator::updateLineWidth(int dataObjId, int lineWidth)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(lineWidth);
    }
    void GraphEventOperator::updateSHPSize(int dataObjId, double size)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(size);
    }
    void GraphEventOperator::updateOpacity(int dataObjId, int element, double opacity)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(element);
        Q_UNUSED(opacity);
    }
    void GraphEventOperator::updateRepresentation(int dataObjId, int representation)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(representation);
    }
    void GraphEventOperator::changeField(int dataId, int type, QString fieldName, int compIndex)
    {
        Q_UNUSED(dataId);
        Q_UNUSED(type);
        Q_UNUSED(fieldName);
        Q_UNUSED(compIndex);
    }
    void GraphEventOperator::updateScalarBarVisibility(int selectedDataId)
    {
        Q_UNUSED(selectedDataId);
    }
    void GraphEventOperator::updateTextInfo(int dataId, QString info)
    {
        Q_UNUSED(dataId);
        Q_UNUSED(info);
    }

    void GraphEventOperator::setRangeColor(bool isOpenOutRanger, QColor minRangeColor, QColor maxRangeColor, double transparency, int postId)
    {
        Q_UNUSED(isOpenOutRanger);
        Q_UNUSED(minRangeColor);
        Q_UNUSED(maxRangeColor);
        Q_UNUSED(transparency);
    }
    void GraphEventOperator::getRangeColor(bool isOutRanger, int postId, QColor & minRangeColor, QColor & maxRangeColor)
    {
        Q_UNUSED(isOutRanger);
        Q_UNUSED(postId);
        Q_UNUSED(minRangeColor);
        Q_UNUSED(maxRangeColor);
    }
    void GraphEventOperator::setRange(int postId, double * range)
    {
        Q_UNUSED(postId);
        Q_UNUSED(range);
    }
    double* GraphEventOperator::getRange(int postId)
    {
        Q_UNUSED(postId);
        return nullptr;
    }
    void GraphEventOperator::setOutRangerColor(bool isOpenOutRanger, double* aboveRGBA, double* belowRGBA, int colorNum, int postId)
    {
        Q_UNUSED(isOpenOutRanger);
        Q_UNUSED(aboveRGBA);
        Q_UNUSED(belowRGBA);
        Q_UNUSED(colorNum);
    }
    void GraphEventOperator::setRangerColorSetFlag(bool isSet)
    {
        Q_UNUSED(isSet);
    }
    bool GraphEventOperator::getRangerColorSetFlag(int postId)
    {
        Q_UNUSED(postId);
        return false;
    }
    void GraphEventOperator::setRangerColorNum(bool isOpenOutRanger, int colorNum, int postId)
    {
        Q_UNUSED(isOpenOutRanger);
        Q_UNUSED(colorNum);
        Q_UNUSED(postId);
    }
    int GraphEventOperator::getRangerColorNum(int postId)
    {
        Q_UNUSED(postId);
        return 0;
    }
    void GraphEventOperator::finalize()
    {
        // 根据不同子类重写。
    }
}  // namespace EventOper
