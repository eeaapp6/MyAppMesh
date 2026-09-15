/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostRuler.h"

#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"

namespace Interface
{
    FITKCFDPostRuler::FITKCFDPostRuler()
    {
        
    }

    FITKCFDPostRuler::~FITKCFDPostRuler()
    {

    }

    FITKPostDataType FITKCFDPostRuler::getPostDataType()
    {
        return FITKPostDataType::Post_Ruler;
    }

    void FITKCFDPostRuler::setPoint(double * point1, double * point2)
    {
        _point1[0] = point1[0];
        _point1[1] = point1[1];
        _point1[2] = point1[2];
        _point2[0] = point2[0];
        _point2[1] = point2[1];
        _point2[2] = point2[2];
    }

    void FITKCFDPostRuler::getPoint(double * point1, double * point2)
    {
        point1[0] = _point1[0];
        point1[1] = _point1[1];
        point1[2] = _point1[2];
        point2[0] = _point2[0];
        point2[1] = _point2[1];
        point2[2] = _point2[2];
    }

    void FITKCFDPostRuler::setColor(QColor color)
    {
        _color = color;
    }

    QColor FITKCFDPostRuler::getColor()
    {
        return _color;
    }

    double FITKCFDPostRuler::getLength()
    {
        Core::FITKPoint pa(_point1);
        Core::FITKPoint pb(_point2);
        return Core::Distance(pa, pb);
    }
}