/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTurbKKLOmega.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFTurbKKLOmega::FITKOFTurbKKLOmega()
    {
        //初始化
        this->init();
        this->setDataObjectName("k-kL-Omega");
    }

    FITKOFSolverTypeEnum::FITKOFTurbulenceType FITKOFTurbKKLOmega::getTurbulenceType()
    {
        //获取湍流类型
        return FITKOFSolverTypeEnum::FITKOFTurbulenceType::RANS;
    }

    void FITKOFTurbKKLOmega::initTurbParam()
    {
        //设置参数
        _turbParams->createDoubleParameter("Ats", 200.0);
        _turbParams->createDoubleParameter("CtauL", 4360.0);
        _turbParams->createDoubleParameter("Sigmaw", 1.17);
        _turbParams->createDoubleParameter("CalphaTheta", 0.035);
        _turbParams->createDoubleParameter("CbpCrit", 1.2);
        _turbParams->createDoubleParameter("Abp", 0.6);
        _turbParams->createDoubleParameter("A0", 4.04);
        _turbParams->createDoubleParameter("As", 2.12);
        _turbParams->createDoubleParameter("CtsCrit", 1000.0);
        _turbParams->createDoubleParameter("Av", 6.75);
        _turbParams->createDoubleParameter("Cw1", 0.44);
        _turbParams->createDoubleParameter("Clambda", 2.495);
        _turbParams->createDoubleParameter("Sigmak", 1.0);
        _turbParams->createDoubleParameter("CnatCrit", 1250.0);
        _turbParams->createDoubleParameter("Cw3", 0.3);
        _turbParams->createDoubleParameter("Anat", 200.0);
        _turbParams->createDoubleParameter("Cw2", 0.92);
        _turbParams->createDoubleParameter("C11", 3.4E-6);
        _turbParams->createDoubleParameter("Css", 1.5);
        _turbParams->createDoubleParameter("C12", 1.0E-10);
        _turbParams->createDoubleParameter("Cint", 0.75);
        _turbParams->createDoubleParameter("CR", 0.12);
        _turbParams->createDoubleParameter("Prtheta", 0.85);
        _turbParams->createDoubleParameter("CmuStd", 0.09);
        _turbParams->createDoubleParameter("CrNat", 0.02);
        _turbParams->createDoubleParameter("Cnc", 0.1);
        _turbParams->createDoubleParameter("CwR", 1.5);
    }

    void FITKOFTurbKKLOmega::initDelta()
    {

    }

    void FITKOFTurbKKLOmega::initDiscretization()
    {

    }

    void FITKOFTurbKKLOmega::initSolution()
    {

    }

    void FITKOFTurbKKLOmega::initBoundary()
    {

    }

    void FITKOFTurbKKLOmega::initInitialConditions()
    {
        //初始化Initial Conditions
    }

}



