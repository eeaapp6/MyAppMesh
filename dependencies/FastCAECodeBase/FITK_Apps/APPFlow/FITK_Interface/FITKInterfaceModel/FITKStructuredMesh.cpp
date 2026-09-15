/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKStructuredMesh.h"


namespace Interface
{
    void FITKStructuredMesh::setIJKDim(const int i, const int j, const int k)
    {
        //复制参数
        _ijkDim[0] = i;
        _ijkDim[1] = j;
        _ijkDim[2] = k;
    }

    void FITKStructuredMesh::getIJKDim(int *ijk)
    {
        //复制参数
        if (ijk == nullptr)return;
        for (int i = 0; i < 3; ++i)
            ijk[i] = _ijkDim[i];
    }

    FITKModelEnum::AbsModelType FITKStructuredMesh::getAbsModelType()
    {
        return FITKModelEnum::AMTStructuredMesh;
    }


    void FITKStructuredMesh::update()
    {

    }

    FITKModelEnum::FITKMeshDim FITKStructuredMesh::getMeshDim()
    {
        //值为1的数量
        int oneNum = 0;
        for (int i = 0; i < 3; ++i)
        {
            if (_ijkDim[i] == 1) oneNum++;
        }
        switch (oneNum)
        {
        //没有非1的值是三维
        case 0: return FITKModelEnum::FMDimD3;
        //1个1，是二维
        case 1: return FITKModelEnum::FMDimD2;
        //2个1 是一维
        case 2: return FITKModelEnum::FMDimD1;
        //3个1 只有一个点 0维
        case 3: return FITKModelEnum::FMDimD0;
        default: break;
        }
        return FITKModelEnum::FMDimNone;
    }

}

