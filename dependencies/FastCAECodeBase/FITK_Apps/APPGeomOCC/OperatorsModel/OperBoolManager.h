/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   OperBoolManager.h
 * @brief  布尔操作器.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-31
 */
#ifndef OPERATORSBOOLMANAGER_H
#define OPERATORSBOOLMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /// 布尔求交
    DefineGeometryOperator(actionIntersect, OperBoolIntersectManager);

    /// 布尔求并
    DefineGeometryOperator(actionUnite, OperBoolUniteManager);

    /// 布尔求差
    DefineGeometryOperator(actionSubtract, OperBoolSubtractManager);
} // namespace ModelOper
#endif // OPERATORSBOOLMANAGER_H
