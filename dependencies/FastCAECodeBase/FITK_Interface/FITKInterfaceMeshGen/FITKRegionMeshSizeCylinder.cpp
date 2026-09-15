/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRegionMeshSizeCylinder.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"
#include <cmath>

namespace Interface
{
    FITKRegionMeshSizeCylinder::FITKRegionMeshSizeCylinder()
    {
        //三个边界类型
        for (int i = 0; i < 3; ++i)
        {
            this->insertBoundary(i, BoundaryType::BTPatch);
            this->insertFaceName(i, "boundaries");
        }
    }

    FITKAbstractRegionMeshSize::RegionType FITKRegionMeshSizeCylinder::getRegionType()
    {
        return FITKAbstractRegionMeshSize::RegionType::RegionCylinder;
    }

    void FITKRegionMeshSizeCylinder::setLocation(double* p1)
    {
        //p1 is the location of the cylinder
        if (p1 == nullptr) return;
        for (int i =0;i<3;++i)
           _location[i] = p1[i];

    }

    void FITKRegionMeshSizeCylinder::getLocation(double* p1)
    {
        if (p1 == nullptr) return;
        for (int i = 0; i < 3; ++i)
            p1[i] = _location[i];
    }

    void FITKRegionMeshSizeCylinder::setDirection(double* p2)
    {
        //p2 is the direction of the cylinder
        if (p2 == nullptr) return;
        for (int i = 0; i < 3; ++i)
            _direction[i] = p2[i];

    }

    void FITKRegionMeshSizeCylinder::getDirection(double* d)
    {
        if (d == nullptr) return;
        for (int i = 0; i < 3; ++i)
            d[i] = _direction[i];
    }

    void FITKRegionMeshSizeCylinder::setRadius(double r)
    {
        //r is the radius of the cylinder
        _radius = r;
    }

    double FITKRegionMeshSizeCylinder::getRadius() const
    {
        return _radius;
    }

    void FITKRegionMeshSizeCylinder::setLength(double length)
    {
        // length is the length of the cylinder
        _length = length;
    }

    double FITKRegionMeshSizeCylinder::getLength() const
    {
        return _length;
    }

    void FITKRegionMeshSizeCylinder::setRadialFraction(double fac)
    {
        _radiusFaction = fac;
    }

    double FITKRegionMeshSizeCylinder::getRadialFraction() const
    {
        return _radiusFaction;
    }

    void FITKRegionMeshSizeCylinder::getPlaneAxis(double* axis)
    {
        if (axis == nullptr) return;
        Core::FITKVec3D  axis3(_direction);
        Core::FITKVec3D  axis1(1, 0, 0);
        double ang = Core::AngleBetweenVec(axis3, axis1);
        //轴向与x 平行
        if (fabs(ang) < 1e-6 || fabs(ang - FITK_PI) < 1e-6)
            axis1.setCoor(0, 1, 0);
        //计算轴线，使其与轴线两两垂直
        Core::FITKVec3D axis2 = Core::CrossProduct(axis3, axis1);
        axis1 = Core::CrossProduct(axis2, axis3);
        //复制值
        for (int i = 0; i < 3; ++i)
            axis[i] = axis1[i];
        for (int i = 0; i < 3; ++i)
            axis[i+3] = axis2[i];
    }

}

