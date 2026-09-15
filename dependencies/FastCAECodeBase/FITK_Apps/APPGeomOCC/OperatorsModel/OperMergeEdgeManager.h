/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperMergeEdgeManager.h
 * @brief   合并线操作器。

 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-08-26
 *********************************************************************/

#ifndef OperMergeEdgeManager_H
#define OperMergeEdgeManager_H

#include "OperCreateGeoManagerBase.h"
#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /**
     * @brief   合并线操作器。
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-26
     */
    DefineGeometryOperator(actionMergeLine, OperMergeEdgeManager);
} // namespace ModelOper

#endif // OperMergeEdgeManager_H
