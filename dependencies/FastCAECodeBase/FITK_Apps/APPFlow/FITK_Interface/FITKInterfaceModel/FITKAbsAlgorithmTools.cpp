/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsAlgorithmTools.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"
#include "FITKAbsInterfaceFactory.h"

namespace Interface
{
    // 变换计算。
    //@{
    FITKAbstractTransformTool* FITKAbstractTransformTool::createTool()
    {
        // 获取注册的工具生成器。
        FITKAbsAlgorithmToolsCreator* creator = Interface::FITKInterfaceAlgorithmFactory::getInstance()->getAlgToolsCreator();
        if (!creator)
        {
            return nullptr;
        }

        return creator->createTransformTool();
    }

    //void FITKAbstractTransformTool::rotationToMatrix3x3(double* iAxis, double iAngle, double oMat33[3][3])
    //{
    //    // 旋转变换转矩阵。
    //    const double m = sqrt(iAxis[0] * iAxis[0] + iAxis[1] * iAxis[1] + iAxis[2] * iAxis[2]);
    //    if (qFuzzyCompare(0, m))
    //    {
    //        return;
    //    }

    //    const double radian = iAngle / 180. * FITK_PI;
    //    const double cosVal = std::cos(radian);
    //    const double sinVal = std::sin(radian);
    //    const double dirX = iAxis[0] / m;
    //    const double dirY = iAxis[1] / m;
    //    const double dirZ = iAxis[2] / m;
    //    
    //    oMat33[0][0] = cosVal + dirX * dirX * (1. - cosVal);
    //    oMat33[0][1] = dirX * dirY * (1. - cosVal) - dirZ * sinVal;
    //    oMat33[0][2] = dirX * dirZ * (1. - cosVal) + dirY * sinVal;

    //    oMat33[1][0] = dirY * dirX * (1. - cosVal) + dirZ * sinVal;
    //    oMat33[1][1] = cosVal + dirY * dirY * (1. - cosVal);
    //    oMat33[1][2] = dirY * dirZ * (1. - cosVal) - dirX * sinVal;

    //    oMat33[2][0] = dirZ * dirX * (1. - cosVal) - dirY * sinVal;
    //    oMat33[2][1] = dirZ * dirY * (1. - cosVal) + dirX * sinVal;
    //    oMat33[2][2] = cosVal + dirZ * dirZ * (1 - cosVal);
    //}
    //@}

    FITKAbstractTransformTool* FITKAbsAlgorithmToolsCreator::createTransformTool()
    {
        //错误值
        return nullptr;
    }
}
