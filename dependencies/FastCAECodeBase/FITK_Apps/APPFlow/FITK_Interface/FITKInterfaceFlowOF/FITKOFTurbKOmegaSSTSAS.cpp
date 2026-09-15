/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTurbKOmegaSSTSAS.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFTurbKOmegaSSTSAS::FITKOFTurbKOmegaSSTSAS()
    {
        //初始化
        this->init();
        this->setDataObjectName("k-Omega SST SAS");
    }

    FITKOFSolverTypeEnum::FITKOFTurbulenceType FITKOFTurbKOmegaSSTSAS::getTurbulenceType()
    {
        //获取湍流类型
        return FITKOFSolverTypeEnum::FITKOFTurbulenceType::RANS;
    }

    void FITKOFTurbKOmegaSSTSAS::initTurbParam()
    {
        //设置参数
        _turbParams->createDoubleParameter("C", 2.0);
        _turbParams->createDoubleParameter("alphaK1", 0.85);
        _turbParams->createDoubleParameter("alphaOmega2", 0.856);
        _turbParams->createDoubleParameter("gamma2", 0.4403);
        _turbParams->createDoubleParameter("alphaK2", 1.0);
        _turbParams->createDoubleParameter("alphaOmega1", 0.5);
        _turbParams->createDoubleParameter("gamma1", 0.5532);
        _turbParams->createDoubleParameter("sigmaPhi", 0.6666666);
        _turbParams->createBoolParameter("F3", false);
        _turbParams->createDoubleParameter("c1", 10.0);
        _turbParams->createDoubleParameter("b1", 1.0);
        _turbParams->createDoubleParameter("a1", 0.31);
        _turbParams->createDoubleParameter("Cs", 0.11);
        _turbParams->createDoubleParameter("beta1", 0.075);
        _turbParams->createDoubleParameter("beta2", 0.0828);
        _turbParams->createDoubleParameter("kappa", 0.41);
        _turbParams->createDoubleParameter("betaStar", 0.09);
        _turbParams->createDoubleParameter("zeta2", 3.51);
    }

    void FITKOFTurbKOmegaSSTSAS::initDelta()
    {

    }

    void FITKOFTurbKOmegaSSTSAS::initDiscretization()
    {

    }

    void FITKOFTurbKOmegaSSTSAS::initSolution()
    {

    }

    void FITKOFTurbKOmegaSSTSAS::initBoundary()
    {

    }

    void FITKOFTurbKOmegaSSTSAS::initInitialConditions()
    {
        //初始化Initial Conditions
    }

}



