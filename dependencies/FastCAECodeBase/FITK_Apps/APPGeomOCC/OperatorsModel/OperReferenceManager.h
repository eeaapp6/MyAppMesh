/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   OperReferenceManager.h
 * @brief  参考基准操作器.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-24
 */
#ifndef OPERATORSREFERENCEMANAGER_H
#define OPERATORSREFERENCEMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /// 参考点操作器
    DefineGeometryOperator(actionReferencePoint, OperReferencePointManager);

    /// 参考线操作器
    DefineGeometryOperator(actionReferenceLine, OperReferenceLineManager);

    /// 参考面操作器
    DefineGeometryOperator(actionReferencePlane, OperReferencePlaneManager);

} // namespace ModelOper
#endif // !OPERATORSREFERENCEMANAGER_H
