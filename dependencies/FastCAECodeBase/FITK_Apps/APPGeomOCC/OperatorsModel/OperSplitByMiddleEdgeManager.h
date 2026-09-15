/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperSplitByMiddleEdgeManager.h
 * @brief   中点分割线操作器。

 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-09-04
 *********************************************************************/

#ifndef OPERSPLITBYMIDDLEEDGEMANAGER_H
#define OPERSPLITBYMIDDLEEDGEMANAGER_H

#include "OperCreateGeoManagerBase.h"
#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /**
     * @brief   中点分割线操作器。
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-26
     */
    DefineGeometryOperator(actionSplitByMiddlePositionLine, OperSplitByMiddleEdgeManager);
} // namespace ModelOper

#endif // OperSplitByMiddleEdgeManager_H
