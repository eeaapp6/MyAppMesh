/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperCurveManager.h
 * @brief   曲线编辑操作器。（倒角/倒圆/移除倒角/移除倒圆/填补孔洞/去除凸包）

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-29
 *********************************************************************/

#ifndef OPERCURVEMANAGER_H
#define OPERCURVEMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    // 定义线段操作器类并注册。
    DefineGeometryOperator(actionSegment, OperTwoPointsSegmentManager);

    // 定义三点圆操作器类并注册。
    DefineGeometryOperator(actionThreePointsCircle, OperThreePointsCircleManager);

    // 定义三点椭圆操作器类并注册。
    DefineGeometryOperator(actionThreePointsEllipse, OperThreePointsEllipseManager);

    // 定义三点双曲线操作器类并注册。
    DefineGeometryOperator(actionThreePointsHyperbola, OperThreePointsHyperbolaManager);

    // 定义三点抛物线操作器类并注册。
    DefineGeometryOperator(actionThreePointsParabola, OperThreePointsParabolaManager);

    // 定义样条曲线操作器类并注册。
    DefineGeometryOperator(actionBSplineByThroughPoints, OperCurveBSplineManager);

    // 定义偏移曲线操作器类并注册。
    DefineGeometryOperator(actionOffsetCurve, OperOffsetCurveManager);

    // 定义线在面上的投影操作器类并注册。
    DefineGeometryOperator(actionCurveProjectionOnSurface, OperCurveProjSurfaceManager);

    // 定义面面交线操作器类并注册。
    DefineGeometryOperator(actionSurfaceIntersectSurface, OperSurfaceInterSurfaceManager);

    // 桥接曲线操作器类并注册。
    DefineGeometryOperator(actionBridgeCurve, OperBridgeCurveManager);

    // 从面提取边线操作器类并注册。
    DefineGeometryOperator(actionCurveFromSurface, OperCurveFromSurfaceManager);

} // namespace ModelOper

#endif // OPERCURVEMANAGER_H
