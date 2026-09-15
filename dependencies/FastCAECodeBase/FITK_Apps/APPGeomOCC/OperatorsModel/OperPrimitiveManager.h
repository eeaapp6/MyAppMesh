/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   OperPrimitiveManager.h
 * @brief  体素操作器.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-31
 */
#ifndef OPERATORSPRIMITIVEMANAGER_H
#define OPERATORSPRIMITIVEMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /// 方盒子
    DefineGeometryOperator(actionBox, OperPrimitiveBoxManager);

    /// 球
    DefineGeometryOperator(actionSphere, OperPrimitiveSphereManager);

    /// 圆柱
    DefineGeometryOperator(actionCylinder, OperPrimitiveCylinderManager);

} // namespace ModelOper
#endif // !OPERATORSPRIMITIVEMANAGER_H
