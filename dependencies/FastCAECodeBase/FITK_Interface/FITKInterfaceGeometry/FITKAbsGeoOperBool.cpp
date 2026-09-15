/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoOperBool.h"

namespace Interface
{
    FITKAbsGeoOperBool::~FITKAbsGeoOperBool()
    {
    }


    void FITKAbsGeoOperBool::setBoolOperType(GeoBoolOperType t)
    {
        _boolOperType = t;
    }

    FITKAbsGeoOperBool::GeoBoolOperType FITKAbsGeoOperBool::getBoolOperType()
    {
        return _boolOperType;
    }

    void FITKAbsGeoOperBool::setGeoShape1(FITKAbsGeoShapeAgent* s1)
    {
        _shape1 = s1;
    }


    void FITKAbsGeoOperBool::setGeoShape2(FITKAbsGeoShapeAgent* s2)
    {
        _shape2 = s2;
    }

    FITKAbsGeoShapeAgent* FITKAbsGeoOperBool::getGeoShape1()
    {
        return _shape1;
    }

    FITKAbsGeoShapeAgent* FITKAbsGeoOperBool::getGeoShape2()
    {
        return _shape2;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoOperBool::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTBool;
    }
}

