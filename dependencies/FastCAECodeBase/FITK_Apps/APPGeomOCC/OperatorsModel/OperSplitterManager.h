/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   OperSplitterManager.h
 * @brief  分割操作器.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-09-10
 */
#ifndef OPERATORSSPLITTERMANAGER_H
#define OPERATORSSPLITTERMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /// 线分割
    DefineGeometryOperator(actionCurveSplitter, OperCurveSplitterManager);

    /// 面分割
    DefineGeometryOperator(actionSurfaceSplitter, OperSurfaceSplitterManager);

    /// 体分割
    DefineGeometryOperator(actionSolidSplitter, OperSolidSplitterManager);
} // namespace ModelOper
#endif // OPERATORSSPLITTERMANAGER_H
