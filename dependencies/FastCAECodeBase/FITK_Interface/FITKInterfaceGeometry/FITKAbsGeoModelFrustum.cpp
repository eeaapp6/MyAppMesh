/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelFrustum.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"


namespace Interface
{

    FITKAbsGeoModelFrustum::~FITKAbsGeoModelFrustum()
    {

    }

    void FITKAbsGeoModelFrustum::setLocation(double* p)
    {
        if (p == nullptr) return;
        FITKAbstractModel::CopyArray(p, _location, 3);
    }

    void FITKAbsGeoModelFrustum::getLocation(double* p)
    {
        if (p == nullptr) return;
        FITKAbstractModel::CopyArray(_location, p, 3);
    }

    void FITKAbsGeoModelFrustum::setBottomLength(double length)
    {
        _bottomLength = length;
    }

    double FITKAbsGeoModelFrustum::getBottomLength() const
    {
        return _bottomLength;
    }

    void FITKAbsGeoModelFrustum::setBottomWidth(double width)
    {
        _bottomWidth = width;
    }

    double FITKAbsGeoModelFrustum::getBottomWidth() const
    {
        return _bottomWidth;
    }

    void FITKAbsGeoModelFrustum::setTopLength(double length)
    {
        _topLength = length;
    }

    double FITKAbsGeoModelFrustum::getTopLength() const
    {
        return _topLength;
    }

    void FITKAbsGeoModelFrustum::setTopWidth(double width)
    {
        _topWidth = width;
    }

    double FITKAbsGeoModelFrustum::getTopWidth() const
    {
        return _topWidth;
    }

    void FITKAbsGeoModelFrustum::setLength(double length)
    {
        _length = length;
    }

    double FITKAbsGeoModelFrustum::getLength() const
    {
        return _length;
    }

    void FITKAbsGeoModelFrustum::setDirection1(double* dir)
    {
        if (dir == nullptr) return;
        FITKAbstractModel::CopyArray(dir, _direction1, 3);
    }

    void FITKAbsGeoModelFrustum::getDirection1(double* dir)
    {
        if (dir == nullptr) return;
        FITKAbstractModel::CopyArray(_direction1, dir, 3);
    }

    void FITKAbsGeoModelFrustum::setDirection2(double* dir)
    {
        if (dir == nullptr) return;
        FITKAbstractModel::CopyArray(dir, _direction2, 3);
    }

    void FITKAbsGeoModelFrustum::getDirection2(double* dir)
    {
        if (dir == nullptr) return;
        FITKAbstractModel::CopyArray(_direction2, dir, 3);
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelFrustum::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTFrustum;
    }
    
}
