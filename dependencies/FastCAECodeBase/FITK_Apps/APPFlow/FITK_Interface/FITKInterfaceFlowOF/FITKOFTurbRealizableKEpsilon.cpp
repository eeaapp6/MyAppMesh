/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTurbRealizableKEpsilon.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFTurbRealizableKEpsilon::FITKOFTurbRealizableKEpsilon()
    {
        //初始化
        this->init();
        this->setDataObjectName("Realizable k-Epsilon");
    }

    FITKOFSolverTypeEnum::FITKOFTurbulenceType FITKOFTurbRealizableKEpsilon::getTurbulenceType()
    {
        //获取湍流类型
        return FITKOFSolverTypeEnum::FITKOFTurbulenceType::RANS;
    }

    void FITKOFTurbRealizableKEpsilon::initTurbParam()
    {
        //设置参数
        _turbParams->createDoubleParameter("Cmu", 0.09);
        _turbParams->createDoubleParameter("sigmak", 1.0);
        _turbParams->createDoubleParameter("sigmaEps", 1.2);
        _turbParams->createDoubleParameter("A0", 4.0);
        _turbParams->createDoubleParameter("C2", 1.9);
    }

    void FITKOFTurbRealizableKEpsilon::initDelta()
    {

    }

    void FITKOFTurbRealizableKEpsilon::initDiscretization()
    {

    }

    void FITKOFTurbRealizableKEpsilon::initSolution()
    {

    }

    void FITKOFTurbRealizableKEpsilon::initInitialConditions()
    {
        //初始化Initial Conditions
    }

    void FITKOFTurbRealizableKEpsilon::initBoundary()
    {

    }

}



