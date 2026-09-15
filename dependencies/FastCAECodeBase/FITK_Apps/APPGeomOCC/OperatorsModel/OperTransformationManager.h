/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   OperTransformationManager.h
 * @brief  变换操作器.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-09-10
 */
#ifndef OPERATORSTRANSFORMATIONMANAGER_H
#define OPERATORSTRANSFORMATIONMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /// 平移变换
    DefineGeometryOperator(actionTransform, OperTransformManager);

    /// 旋转变换
    DefineGeometryOperator(actionRotate, OperRotateManager);

    /// 缩放变换
    DefineGeometryOperator(actionScale, OperScaleManager);

    /// 镜像变换
    DefineGeometryOperator(actionMirror, OperMirrorManager);

    /// 线性（矩阵）阵列变换
    DefineGeometryOperator(actionRectangularPattern, OperRectangularPatternManager);

    /// 环形阵列变换
    DefineGeometryOperator(actionCircularPattern, OperCircularPatternManager);
} // namespace ModelOper
#endif // OPERATORSTRANSFORMATIONMANAGER_H
