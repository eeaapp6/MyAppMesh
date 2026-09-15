/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelCylinder.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"


namespace Interface
{

    FITKAbsGeoModelCylinder::~FITKAbsGeoModelCylinder()
    {

    }

    void FITKAbsGeoModelCylinder::setLocation(double* p1)
    {
        //p1 is the location of the cylinder
        if (p1 == nullptr) return;
        FITKAbstractModel::CopyArray(p1, _location, 3);
    }

    void FITKAbsGeoModelCylinder::getLocation(double* p1)
    {
        if (p1 == nullptr) return;
        FITKAbstractModel::CopyArray(_location, p1, 3);
    }

    void FITKAbsGeoModelCylinder::setDirection(double* p2)
    {
        //p2 is the direction of the cylinder
        if (p2 == nullptr) return;
        FITKAbstractModel::CopyArray(p2, _direction, 3);
    }

    void FITKAbsGeoModelCylinder::getDirection(double* dir)
    {
        //p2 is the direction of the cylinder
        if (dir == nullptr) return;
        FITKAbstractModel::CopyArray(_direction, dir,3);
    }

    void FITKAbsGeoModelCylinder::setRadius(double r)
    {
        //r is the radius of the cylinder
        _radius = r;
    }

    double FITKAbsGeoModelCylinder::getRadius() const
    {
        return _radius;
    }

    void FITKAbsGeoModelCylinder::setLength(double length)
    {
        //length is the length of the cylinder
        _length = length;
    }

    double FITKAbsGeoModelCylinder::getLength() const
    {
        return _length;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCylinder::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTCylinder;
    }
    
}



