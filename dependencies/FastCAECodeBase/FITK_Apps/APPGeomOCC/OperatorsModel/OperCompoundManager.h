/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   OperCompoundManager.h
 * @brief  复合体操作器.
 * @author fulipeng (fulipengqd@yeah.net)
 * @date   2024-09-07
 */
#ifndef OPERCOMPOUNDMANAGER_H
#define OPERCOMPOUNDMANAGER_H

#include "OperCreateGeoManagerBase.h"
#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /// 添加复合体操作对象操作器
    DefineGeometryOperator(actionCompoundAppendShape, OperAppendCompoundManager);

    /// 移除复合体操作对象操作器
    DefineGeometryOperator(actionCompoundRemoveShape, OperRemoveCompoundManager);

} // namespace ModelOper
#endif // !OPERATORSSOLIDMANAGER_H
