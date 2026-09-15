/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshSizeGeneratorGmshExec.h"

namespace Gmsh
{
    Interface::FITKGlobalMeshSizeInfo* FITKMeshSizeGeneratorGmshExec::generateGlobalMeshSizeInfo()
    {
        //默认使用父类函数
        return Interface::FITKAbstractMeshSizeInfoGenerator::generateGlobalMeshSizeInfo();
    }
    Interface::FITKAbstractRegionMeshSize* FITKMeshSizeGeneratorGmshExec::createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType t)
    {
        //默认使用父类函数
        return Interface::FITKAbstractMeshSizeInfoGenerator::createRegionMeshSize(t);
    }
}




