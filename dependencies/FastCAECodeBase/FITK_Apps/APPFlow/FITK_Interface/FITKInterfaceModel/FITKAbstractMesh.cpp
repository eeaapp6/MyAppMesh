/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractMesh.h"

namespace Interface
{

    FITKAbstractMesh::~FITKAbstractMesh()
    {

    }

    FITKModelEnum::AbsModelType FITKAbstractMesh::getAbsModelType()
    {
         // 返回错误值，子类重写
        return FITKModelEnum::AbsModelType::AMTMesh;
    }

    void FITKAbstractMesh::update()
    {
        //todo
    }

    FITKModelEnum::FITKMeshDim FITKAbstractMesh::getMeshDim()
    {
        //错误值
        return FITKModelEnum::FITKMeshDim::FMDimNone;
    }

    unsigned int FITKAbstractMesh::getMeshDimBit() 
    {
       return 0;
    }

}