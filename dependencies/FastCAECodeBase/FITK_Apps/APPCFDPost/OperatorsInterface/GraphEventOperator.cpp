/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphEventOperator.h"

namespace EventOper 
{
    void GraphEventOperator::updateAll()
    {

    }

    void GraphEventOperator::updateSingle(int postDataID)
    {
        Q_UNUSED(postDataID);
    }

    void GraphEventOperator::updateLight(int dataID)
    {
        Q_UNUSED(dataID);
    }

    void GraphEventOperator::updateShow(int postDataID, bool isShow)
    {
        Q_UNUSED(postDataID);
        Q_UNUSED(isShow);
    }

    void GraphEventOperator::updateAllRotateVis(bool isVis)
    {
        Q_UNUSED(isVis);
    }

    void GraphEventOperator::reRender()
    {

    }
}
