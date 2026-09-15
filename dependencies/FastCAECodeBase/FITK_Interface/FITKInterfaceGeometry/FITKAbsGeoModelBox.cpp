/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelBox.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{

    FITKAbsGeoModelBox::~FITKAbsGeoModelBox()
    {

    }

    void FITKAbsGeoModelBox::setPoint1(double* p1)
    {
        //第一点
        if(p1 == nullptr) return;
        FITKAbstractModel::CopyArray(p1, _point1, 3);
    }

    void FITKAbsGeoModelBox::setLength(double* p2)
    {
        if(p2 == nullptr) return;
        //第二点
        FITKAbstractModel::CopyArray(p2, _lengthParam, 3);
    }

     

    void FITKAbsGeoModelBox::getPoint1(double* p1)
    {
        //第一点
        if (p1 == nullptr) return;
        FITKAbstractModel::CopyArray( _point1, p1, 3);
    }

    void FITKAbsGeoModelBox::getLength(double* p2)
    {
        if (p2 == nullptr) return;
        //第二点
        FITKAbstractModel::CopyArray(_lengthParam, p2, 3);
    }

    void FITKAbsGeoModelBox::setDirection1(double* dir)
    {
        if (dir == nullptr) return;
        FITKAbstractModel::CopyArray(dir, _direction1, 3);
    }

    void FITKAbsGeoModelBox::setDirection2(double* dir)
    {
        if (dir == nullptr) return;
        FITKAbstractModel::CopyArray(dir, _direction2, 3);
    }

    void FITKAbsGeoModelBox::getDirection1(double* dir)
    {
        if (dir == nullptr) return;
        FITKAbstractModel::CopyArray(_direction1, dir, 3);
    }

    void FITKAbsGeoModelBox::getDirection2(double* dir)
    {
        if (dir == nullptr) return;
        FITKAbstractModel::CopyArray(_direction2, dir, 3);
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelBox::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTBox;
    }

}
