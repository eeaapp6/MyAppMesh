/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTurbKOmegaSSTIDDES.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFTurbKOmegaSSTIDDES::FITKOFTurbKOmegaSSTIDDES()
    {
        //初始化
        this->init();
        this->setDataObjectName("k-Omega SST IDDES");
    }

    FITKOFSolverTypeEnum::FITKOFTurbulenceType FITKOFTurbKOmegaSSTIDDES::getTurbulenceType()
    {
        //获取湍流类型
        return FITKOFSolverTypeEnum::FITKOFTurbulenceType::LES;
    }

    void FITKOFTurbKOmegaSSTIDDES::initTurbParam()
    {
        //设置参数
        _turbParams->createDoubleParameter("CDESkom", 0.82);
        _turbParams->createDoubleParameter("alphaK1", 0.85);
        _turbParams->createDoubleParameter("alphaOmega2", 0.856);
        _turbParams->createDoubleParameter("gamma2", 0.4403);
        _turbParams->createDoubleParameter("alphaK2", 1.0);
        _turbParams->createDoubleParameter("alphaOmega1", 0.5);
        _turbParams->createDoubleParameter("gamma1", 0.5532);
        _turbParams->createDoubleParameter("C1", 5.0);
        _turbParams->createBoolParameter("F3", false);
        _turbParams->createDoubleParameter("c1", 10.0);
        _turbParams->createDoubleParameter("b1", 1.0);
        _turbParams->createDoubleParameter("a1", 0.31);
        _turbParams->createDoubleParameter("Cd2", 3.0);
        _turbParams->createDoubleParameter("Cd1", 20.0);
        _turbParams->createDoubleParameter("Ct", 1.87);
        _turbParams->createDoubleParameter("CDESkeps", 0.6);
        _turbParams->createDoubleParameter("beta1", 0.075);
        _turbParams->createDoubleParameter("beta2", 0.0828);
        _turbParams->createDoubleParameter("kappa", 0.41);
        _turbParams->createDoubleParameter("betaStar", 0.09);
    }

    void FITKOFTurbKOmegaSSTIDDES::initDelta()
    {

    }

    void FITKOFTurbKOmegaSSTIDDES::initDiscretization()
    {

    }

    void FITKOFTurbKOmegaSSTIDDES::initSolution()
    {

    }

    void FITKOFTurbKOmegaSSTIDDES::initBoundary()
    {

    }

    void FITKOFTurbKOmegaSSTIDDES::initInitialConditions()
    {
        //初始化Initial Conditions
    }

}



