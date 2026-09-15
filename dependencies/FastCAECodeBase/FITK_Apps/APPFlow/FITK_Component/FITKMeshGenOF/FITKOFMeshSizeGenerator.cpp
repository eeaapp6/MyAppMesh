/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFMeshSizeGenerator.h"

namespace OF
{
    Interface::FITKGlobalMeshSizeInfo* FITKOFMeshSizeGenerator::generateGlobalMeshSizeInfo()
    {
        //默认使用父类函数
        return Interface::FITKAbstractMeshSizeInfoGenerator::generateGlobalMeshSizeInfo();
    }
    Interface::FITKAbstractRegionMeshSize* FITKOFMeshSizeGenerator::createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType t)
    {
        //默认使用父类函数
        return Interface::FITKAbstractMeshSizeInfoGenerator::createRegionMeshSize(t);
    }
}




