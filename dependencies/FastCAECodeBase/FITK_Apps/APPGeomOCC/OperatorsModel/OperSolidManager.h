/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   OperSolidManager.h
 * @brief  体操作器.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-24
 */
#ifndef OPERATORSSOLIDMANAGER_H
#define OPERATORSSOLIDMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /// 闭合曲面形成体操作器
    DefineGeometryOperator(actionClosedSurfaceSolid, OperSolidClosedSurfaceManager);

    /// 拉伸形成体操作器
    DefineGeometryOperator(actionExtrudeSolid, OperSolidExtrudeManager);

    /// 旋转形成体操作器
    DefineGeometryOperator(actionRevolSolid, OperSolidRevolManager);

    /// 扫略形成体体操作器
    DefineGeometryOperator(actionSweepSolid, OperSolidSweepManager);

    /// 多截面扫略形成体操作器
    DefineGeometryOperator(actionMultiSectionSolid, OperSolidMultiSectionManager);

} // namespace ModelOper
#endif // !OPERATORSSOLIDMANAGER_H
