/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphInteractionOperator.h"

namespace EventOper
{
    void GraphInteractionOperator::setGraphStateByPickInfo(int dataObjId)
    {
        // 根据不同子类重写。
        Q_UNUSED(dataObjId);
    }

    void GraphInteractionOperator::setEnableGraphPickable(bool model, bool sketch, bool datum)
    {
        // 根据不同子类重写。
        Q_UNUSED(model);
        Q_UNUSED(sketch);
        Q_UNUSED(datum);
    }

    void GraphInteractionOperator::setCustomPickMode()
    {
        // 根据不同子类重写。
    }

    void GraphInteractionOperator::clearPickMode()
    {
        // 根据不同子类重写。
    }
}  // namespace EventOper
