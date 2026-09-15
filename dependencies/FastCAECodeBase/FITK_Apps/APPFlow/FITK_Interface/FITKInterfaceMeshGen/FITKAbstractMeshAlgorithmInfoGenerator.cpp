/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractMeshAlgorithmInfoGenerator.h"
#include "FITKGlobalMeshGenerateAlgorithmInfo.h"


namespace Interface
{
    FITKAbstractMeshAlgorithmInfoGenerator::~FITKAbstractMeshAlgorithmInfoGenerator()
    {
        //纯虚函数
    }

    FITKGlobalMeshGenerateAlgorithmInfo* FITKAbstractMeshAlgorithmInfoGenerator::generateGlobalMeshAlgorithmInfo()
    {
        //默认值
        return new FITKGlobalMeshGenerateAlgorithmInfo;
    }

}


