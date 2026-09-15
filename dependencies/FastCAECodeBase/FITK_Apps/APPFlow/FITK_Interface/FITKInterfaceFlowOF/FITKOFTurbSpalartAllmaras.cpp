/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTurbSpalartAllmaras.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFTurbSpalartAllmaras::FITKOFTurbSpalartAllmaras()
    {
        //初始化
        this->init();
        this->setDataObjectName("Spalart-Allmaras");
    }

    FITKOFSolverTypeEnum::FITKOFTurbulenceType FITKOFTurbSpalartAllmaras::getTurbulenceType()
    {
        //获取湍流类型
        return FITKOFSolverTypeEnum::FITKOFTurbulenceType::RANS;
    }

    void FITKOFTurbSpalartAllmaras::initTurbParam()
    {
        //设置参数
        _turbParams->createDoubleParameter("Cb2", 0.622);
        _turbParams->createDoubleParameter("Cb1", 0.1355);
        _turbParams->createDoubleParameter("kappa", 0.41);
        _turbParams->createDoubleParameter("sigmaNut", 0.66666);
        _turbParams->createDoubleParameter("Cw3", 2.0);
        _turbParams->createDoubleParameter("Cv2", 5.0);
        _turbParams->createDoubleParameter("Cw2", 0.3);
        _turbParams->createDoubleParameter("Cv1", 7.1);
    }

    void FITKOFTurbSpalartAllmaras::initDelta()
    {

    }

    void FITKOFTurbSpalartAllmaras::initDiscretization()
    {

    }

    void FITKOFTurbSpalartAllmaras::initSolution()
    {

    }

    void FITKOFTurbSpalartAllmaras::initBoundary()
    {

    }

    void FITKOFTurbSpalartAllmaras::initInitialConditions()
    {
        //初始化Initial Conditions
    }

}



