/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelCone.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"


namespace Interface
{

    FITKAbsGeoModelCone::~FITKAbsGeoModelCone()
    {

    }

    void FITKAbsGeoModelCone::setLocation(double* p1)
    {
        if (p1 == nullptr) return;
        FITKAbstractModel::CopyArray(p1, _location, 3);
    }

    void FITKAbsGeoModelCone::getLocation(double* p1)
    {
        if (p1 == nullptr) return;
        FITKAbstractModel::CopyArray(_location, p1, 3);
    }

    void FITKAbsGeoModelCone::setDirection(double* dir)
    {
        if (dir == nullptr) return;
        FITKAbstractModel::CopyArray(dir, _direction, 3);
    }

    void FITKAbsGeoModelCone::getDirection(double* dir)
    {
        if (dir == nullptr) return;
        FITKAbstractModel::CopyArray(_direction, dir, 3);
    }

    void FITKAbsGeoModelCone::setBottomRadius(double r)
    {
        _bottomRadius = r;
    }

    double FITKAbsGeoModelCone::getBottomRadius() const
    {
        return _bottomRadius;
    }

    void FITKAbsGeoModelCone::setTopRadius(double r)
    {
        _topRadius = r;
    }

    double FITKAbsGeoModelCone::getTopRadius() const
    {
        return _topRadius;
    }

    void FITKAbsGeoModelCone::setLength(double length)
    {
        _length = length;
    }

    double FITKAbsGeoModelCone::getLength() const
    {
        return _length;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCone::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTCone;
    }
    
}
