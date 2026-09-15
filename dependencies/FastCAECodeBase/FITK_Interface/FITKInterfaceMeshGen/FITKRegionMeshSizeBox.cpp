/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRegionMeshSizeBox.h"


namespace Interface
{

    FITKRegionMeshSizeBox::FITKRegionMeshSizeBox()
    {
        //初始化6个边界类型
        for (int i=0;i<6; ++i)
        {
            this->insertBoundary(i, BoundaryType::BTPatch);
            this->insertFaceName(i, "boundaries");
        }
    }

    FITKAbstractRegionMeshSize::RegionType FITKRegionMeshSizeBox::getRegionType()
    {
        //返回box类型
        return FITKAbstractRegionMeshSize::RegionType::RegionBox;
    }

    void FITKRegionMeshSizeBox::setPoint1(double* p1)
    {
        //错误判断
        if (nullptr == p1) return;
        for (int i = 0; i<3; ++i)
            _point1[i] = p1[i];
    }

    void FITKRegionMeshSizeBox::setLength(double* length)
    {
        //错误判断
        if (length == nullptr) return;
        for (int i = 0; i < 3; ++i)
            _lengthParam[i] = length[i];
    }

    void FITKRegionMeshSizeBox::getPoint1(double* p1)
    {
        //错误判断
        if (nullptr == p1) return;
        for (int i = 0; i < 3; ++i)
            p1[i] = _point1[i];
    }

    void FITKRegionMeshSizeBox::getLength(double* length)
    {
        //错误判断
        if (length == nullptr) return;
        for (int i = 0; i < 3; ++i)
            length[i] = _lengthParam[i];
    }

}


