/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKAbsGeoModelNearestPointLine.cpp
 * @brief   Nearest Point Line Command Class Implementation
 * @date    2025-03-27
 * @author  libaojun
 *********************************************************************/

#include "FITKAbsGeoModelNearestPointLine.h"
 

namespace Interface
{
    FITKAbsGeoModelNearestPointLine::~FITKAbsGeoModelNearestPointLine() = default;

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelNearestPointLine::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTNearestPointLine;
    }

    void FITKAbsGeoModelNearestPointLine::setPoint(const double* point)
    {
        _point[0] = point[0];
        _point[1] = point[1];
        _point[2] = point[2];
    }

    void FITKAbsGeoModelNearestPointLine::getPoint(double* point) const
    {
        point[0] = _point[0];
        point[1] = _point[1];
        point[2] = _point[2];
    }

    void FITKAbsGeoModelNearestPointLine::setShape(const VirtualShape& shape)
    {
        _shape = shape;
    }

    VirtualShape FITKAbsGeoModelNearestPointLine::getShape() const
    {
        return _shape;
    }

    void FITKAbsGeoModelNearestPointLine::setShapeType(ShapeType type)
    {
        _shapeType = type;
    }

    FITKAbsGeoModelNearestPointLine::ShapeType FITKAbsGeoModelNearestPointLine::getShapeType() const
    {
        return _shapeType;
    }

    void FITKAbsGeoModelNearestPointLine::setNearestPoint(const double* nearestPoint)
    {
        _nearestPoint[0] = nearestPoint[0];
        _nearestPoint[1] = nearestPoint[1];
        _nearestPoint[2] = nearestPoint[2];
    }

    void FITKAbsGeoModelNearestPointLine::getNearestPoint(double* nearestPoint) const
    {
        nearestPoint[0] = _nearestPoint[0];
        nearestPoint[1] = _nearestPoint[1];
        nearestPoint[2] = _nearestPoint[2];
    }

}