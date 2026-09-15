/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractGeometryMeshSizeGenerator.h"

namespace Interface
{
    FITKAbstractGeometryMeshSizeGenerator::~FITKAbstractGeometryMeshSizeGenerator()
    {
        //纯虚函数
    }

    FITKGeometryMeshSize* FITKAbstractGeometryMeshSizeGenerator::createGeometryMeshSize()
    {
        //默认值
        return new FITKGeometryMeshSize;
    }

}


