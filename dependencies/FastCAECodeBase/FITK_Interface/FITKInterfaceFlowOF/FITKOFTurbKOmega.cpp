/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTurbKOmega.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITKOFDiscretization.h"
#include "FITKOFSolution.h"
#include "FITKOFSolutionSolver.h"
#include "FITKOFInitialConditions.h"
#include "FITKOFBoundary.h"
#include "FITKOFBoundaryType.h"

namespace Interface
{
    FITKOFTurbKOmega::FITKOFTurbKOmega()
    {
        //初始化
        this->init();
        this->setDataObjectName("k-Omega");
    }

    FITKOFSolverTypeEnum::FITKOFTurbulenceType FITKOFTurbKOmega::getTurbulenceType()
    {
        //获取湍流类型
        return FITKOFSolverTypeEnum::FITKOFTurbulenceType::RANS;
    }

    void FITKOFTurbKOmega::initTurbParam()
    {
        //设置参数
        _turbParams->createDoubleParameter("alphak", 0.5);
        _turbParams->createDoubleParameter("Cmu", 0.09);
        _turbParams->createDoubleParameter("alphaOmega", 0.5);
        _turbParams->createDoubleParameter("beta", 0.072);
        _turbParams->createDoubleParameter("alpha", 0.52);
    }

    void FITKOFTurbKOmega::initDelta()
    {

    }

    void FITKOFTurbKOmega::initDiscretization()
    {
        if (!_discretization)return;
        _discretization->initAppendTurbulenceConvection("k");
        _discretization->initAppendTurbulenceConvection("omega");
    }

    void FITKOFTurbKOmega::initSolution()
    {
        if (!_solution) return;
        //初始化k的参数 solver
        FITKOFAlgebraicEquationsPara* kPara = new FITKOFAlgebraicEquationsPara;
        kPara->setSolutionSolver(new FITKOFSolutionSolverPBiCGStabilized());
        kPara->setVariableName("k");
        _solution->appendSolutionSolver(kPara);
        //初始化omega的参数 solver
        FITKOFAlgebraicEquationsPara* omegaPara = new FITKOFAlgebraicEquationsPara;
        omegaPara->setSolutionSolver(new FITKOFSolutionSolverPBiCGStabilized());
        omegaPara->setVariableName("omega");
        _solution->appendSolutionSolver(omegaPara);
        //设置Residuals
        _solution->initAppendResidual("k");
        _solution->initAppendResidual("omega");
        //设置Relaxation
        Core::FITKParameter* relaxation = _solution->getRelaxation();
        if (!relaxation)return;
        relaxation->createDoubleParameter("k", 1.0);
        relaxation->createDoubleParameter("omega", 1.0);

    }

    void FITKOFTurbKOmega::initBoundary()
    {
        //初始化Wall边界信息
        FITKOFBoundary* boundaryWall = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall);
        boundaryWall->setTurbulenceBoundary("k", new FITKOFBTyStandardWallFunction);
        boundaryWall->setTurbulenceBoundary("omega", new FITKOFBTyStandardWallFunction);
        boundaryWall->setTurbulenceBoundary("nut", new FITKOFBTyStandardWallFunction);
        _boundaryInfos.append(boundaryWall);
        //初始化PressureInlet边界信息
        FITKOFBoundary* boundaryPInlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet);
        boundaryPInlet->setTurbulenceBoundary("k", new FITKOFBTyTurbulentIntensityInlet);
        boundaryPInlet->setTurbulenceBoundary("omega", new FITKOFBTyTurbulentMixingLengthInlet);
        _boundaryInfos.append(boundaryPInlet);
        //初始化VelocityInlet边界信息
        FITKOFBoundary* boundaryVInlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet);
        boundaryVInlet->setTurbulenceBoundary("k", new FITKOFBTyTurbulentIntensityInlet);
        boundaryVInlet->setTurbulenceBoundary("omega", new FITKOFBTyTurbulentMixingLengthInlet);
        _boundaryInfos.append(boundaryVInlet);
        //初始化PressureOutlet边界信息
        FITKOFBoundary* boundaryPOutlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet);
        boundaryPOutlet->setTurbulenceBoundary("k", new FITKOFBTyZeroGradient);
        boundaryPOutlet->setTurbulenceBoundary("omega", new FITKOFBTyZeroGradient);
        _boundaryInfos.append(boundaryPOutlet);
    }

    void FITKOFTurbKOmega::initInitialConditions()
    {
        //初始化Initial Conditions
        if (!_initialCond) return;
        //初始化Basic
        Core::FITKParameter* basic = _initialCond->getBasicData();
        if (!basic)return;
        basic->createDoubleParameter("k", 1e-02);
        basic->createDoubleParameter("omega", 1);
        basic->createDoubleParameter("nut", 0.1);
    }

}



