/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef OPERATORSPOINTMANAGER_H
#define OPERATORSPOINTMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    DefineGeometryOperator(actionPoint, OperCoordinatePointManager);
    DefineGeometryOperator(actionCurveEnd, OperCurveEndPointManager);
    DefineGeometryOperator(actionCurveRatio, OperCurveAndRatioPointManager);
    DefineGeometryOperator(actionCurveEndDistance, OperCurveEndAndDistancePointManager);
    DefineGeometryOperator(actionPointProjectionOnCurve, OperPointProjectionOnCurveManager);
    DefineGeometryOperator(actionTwoCurveIntersectPoint, OperIntersectionOfTwoCurveManager);
    DefineGeometryOperator(actionPointProjectionOnSurface, OperPointProjectionOnSurfaceManager);
    DefineGeometryOperator(actionCurveIntersectSurface, OperIntersectionOfCurveAndSurfaceManager);
    DefineGeometryOperator(actionInteractionPoint, OperInteractionPointManager);

} // namespace ModelOper
#endif // !OPERATORSPOINTMANAGER_H
