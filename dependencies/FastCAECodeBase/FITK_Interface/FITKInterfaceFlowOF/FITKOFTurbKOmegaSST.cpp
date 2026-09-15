/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTurbKOmegaSST.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITKOFDiscretization.h"
#include "FITKOFSolution.h"
#include "FITKOFSolutionSolver.h"
#include "FITKOFInitialConditions.h"
#include "FITKOFBoundary.h"
#include "FITKOFBoundaryType.h"

namespace Interface
{
    FITKOFTurbKOmegaSST::FITKOFTurbKOmegaSST()
    {
        //初始化
        this->init();
        this->setDataObjectName("k-Omega SST");
    }

    FITKOFSolverTypeEnum::FITKOFTurbulenceType FITKOFTurbKOmegaSST::getTurbulenceType()
    {
        //获取湍流类型
        return FITKOFSolverTypeEnum::FITKOFTurbulenceType::RANS;
    }

    void FITKOFTurbKOmegaSST::initTurbParam()
    {
        //设置参数
        _turbParams->createDoubleParameter("alphaK1", 0.85);
        _turbParams->createDoubleParameter("alphaK2", 1.0);
        _turbParams->createDoubleParameter("alphaOmega1", 0.5);
        _turbParams->createDoubleParameter("alphaOmega2", 0.856);
        _turbParams->createDoubleParameter("beta1", 0.075);
        _turbParams->createDoubleParameter("beta2", 0.0828);
        _turbParams->createDoubleParameter("betaStar", 0.09);
        _turbParams->createDoubleParameter("gamma1", 0.5532);
        _turbParams->createDoubleParameter("gamma2", 0.4403);
        _turbParams->createDoubleParameter("a1", 0.31);
        _turbParams->createDoubleParameter("b1", 1.0);
        _turbParams->createDoubleParameter("c1", 10.0);
        _turbParams->createBoolParameter("F3", false);
    }

    void FITKOFTurbKOmegaSST::initDelta()
    {

    }

    void FITKOFTurbKOmegaSST::initDiscretization()
    {
        if (!_discretization)return;
        _discretization->initAppendTurbulenceConvection("k");
        _discretization->initAppendTurbulenceConvection("omega");
    }

    void FITKOFTurbKOmegaSST::initSolution()
    {
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
        relaxation->createDoubleParameter("k", 0.5);
        relaxation->createDoubleParameter("omega", 0.5);
    }

    void FITKOFTurbKOmegaSST::initBoundary()
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
        //初始化Outflow边界信息
        FITKOFBoundary* boundaryPOutflow = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow);
        boundaryPOutflow->setTurbulenceBoundary("k", new FITKOFBTyZeroGradient);
        boundaryPOutflow->setTurbulenceBoundary("omega", new FITKOFBTyZeroGradient);
        _boundaryInfos.append(boundaryPOutflow);
        //初始化Symmetry边界信息
        FITKOFBoundary* boundarySymmetry = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BSymmetry);
        boundarySymmetry->setTurbulenceBoundary("k", new FITKOFBTySymmetry);
        boundarySymmetry->setTurbulenceBoundary("omega", new FITKOFBTySymmetry);
        boundarySymmetry->setTurbulenceBoundary("nut", new FITKOFBTySymmetry);
        _boundaryInfos.append(boundarySymmetry);
    }

    void FITKOFTurbKOmegaSST::initInitialConditions()
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



