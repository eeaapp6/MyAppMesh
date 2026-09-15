/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTurbSmagorinsky.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFTurbSmagorinsky::FITKOFTurbSmagorinsky()
    {
        //初始化
        this->init();
        this->setDataObjectName("Smagorinsky");
    }

    FITKOFSolverTypeEnum::FITKOFTurbulenceType FITKOFTurbSmagorinsky::getTurbulenceType()
    {
        //获取湍流类型
        return FITKOFSolverTypeEnum::FITKOFTurbulenceType::LES;
    }

    void FITKOFTurbSmagorinsky::initTurbParam()
    {
        //设置参数
        _turbParams->createDoubleParameter("Ck", 0.094);
        _turbParams->createDoubleParameter("Ce", 1.048);
    }

    void FITKOFTurbSmagorinsky::initDelta()
    {

    }

    void FITKOFTurbSmagorinsky::initDiscretization()
    {

    }

    void FITKOFTurbSmagorinsky::initSolution()
    {

    }

    void FITKOFTurbSmagorinsky::initBoundary()
    {

    }

    void FITKOFTurbSmagorinsky::initInitialConditions()
    {
        //初始化Initial Conditions
    }

}



