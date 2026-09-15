/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTurbKEpsilon.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFTurbKEpsilon::FITKOFTurbKEpsilon()
    {
        //初始化
        this->init();
        this->setDataObjectName("k-Epsilon");
    }

    FITKOFSolverTypeEnum::FITKOFTurbulenceType FITKOFTurbKEpsilon::getTurbulenceType()
    {
        //获取湍流类型
        return FITKOFSolverTypeEnum::FITKOFTurbulenceType::RANS;
    }

    void FITKOFTurbKEpsilon::initTurbParam()
    {
        //设置参数
        _turbParams->createDoubleParameter("Cmu", 0.09);//设置参数
        _turbParams->createDoubleParameter("C1", 1.44);//设置参数
        _turbParams->createDoubleParameter("sigmaEps", 1.3);//设置参数
        _turbParams->createDoubleParameter("C2", 1.92);//设置参数
    }

    void FITKOFTurbKEpsilon::initDelta()
    {

    }

    void FITKOFTurbKEpsilon::initDiscretization()
    {

    }

    void FITKOFTurbKEpsilon::initSolution()
    {

    }

    void FITKOFTurbKEpsilon::initBoundary()
    {

    }

    void FITKOFTurbKEpsilon::initInitialConditions()
    {
        //初始化Initial Conditions
    }

}



