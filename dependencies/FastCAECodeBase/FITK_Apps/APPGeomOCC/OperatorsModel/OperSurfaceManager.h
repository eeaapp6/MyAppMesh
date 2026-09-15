/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   OperSurfaceManager.h
 * @brief  曲面操作器.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-24
 */
#ifndef OPERATORSSURFACEMANAGER_H
#define OPERATORSSURFACEMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /// 闭合曲线形成面操作器
    DefineGeometryOperator(actionClosedWireSurface, OperSurfaceClosedWireManager);

    /// 偏移面操作器
    DefineGeometryOperator(actionOffsetSurface, OperSurfaceOffsetManager);

    /// 拉伸形成曲面操作器
    DefineGeometryOperator(actionExtrudeSurface, OperSurfaceExtrudeManager);

    /// 旋转形成曲面操作器
    DefineGeometryOperator(actionRevolSurface, OperSurfaceRevolManager);

    /// 扫略形成曲面面操作器
    DefineGeometryOperator(actionSweepSurface, OperSurfaceSweepManager);

    /// 多截面扫略形成曲面操作器
    DefineGeometryOperator(actionMultiSectionSurface, OperSurfaceMultiSectionManager);

    /// 从体上提取面操作器
    DefineGeometryOperator(actionSurfaceFromSolid, OperSurfaceFromSolidManager);

    /// 桥接面操作器
    DefineGeometryOperator(actionBridgeSurface, OperBridgeSurfaceManager);

} // namespace ModelOper
#endif // !OPERATORSSURFACEMANAGER_H
