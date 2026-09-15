/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractModel.h"

namespace Interface
{

    FITKAbstractModel::~FITKAbstractModel()
    {
        //构造函数，无实际意义
    }

    FITKModelEnum::AbsModelType FITKAbstractModel::getAbsModelType()
    {
         // 返回错误值
        return FITKModelEnum::AbsModelType::AMTNone;
    }

    Interface::FITKComponentManager* FITKAbstractModel::getComponentManager()
    {
        return nullptr;
    }

 
    bool FITKAbstractModel::writeToFile(const QString & file)
    {
         // 返回错误值
        return false;
    }

    Interface::FITKModelEnum::FITKModelDim FITKAbstractModel::getModelDim()
    {
        return FITKModelEnum::FMDNone;
    }

    bool FITKAbstractModel::getPointCoor(int pointID, double* coor, int modelIndex /*= 0*/)
    {
        return  false;
    }

    void FITKAbstractModel::transformPoint(double* iPos, double* oPos)
    {
        // 默认不进行处理。
        for (int i = 0; i < 3; i++)
        {
            oPos[i] = iPos[i];
        }
    }

    void FITKAbstractModel::transformDirection(double* iDir, double* oDir)
    {
        // 默认不进行处理。
        for (int i = 0; i < 3; i++)
        {
            oDir[i] = iDir[i];
        }
    }
    
    void FITKAbstractModel::CopyArray(double* from, double* to, int dim)
    {
        for (int i = 0; i < dim; ++i)
            to[i] = from[i];
    }

}