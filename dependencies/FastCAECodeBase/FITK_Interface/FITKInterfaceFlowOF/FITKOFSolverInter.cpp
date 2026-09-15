/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFSolverInter.h"
#include "FITKOFDiscretization.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamRadioGroup.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBoolGroup.h"
#include "FITKOFSolution.h"
#include "FITKOFSolutionSolver.h"
#include "FITKOFInitialConditions.h"
#include "FITKOFTransportProp.h"
#include "FITKOFTransportModel.h"
#include "FITKOFBoundary.h"
#include "FITKOFBoundaryType.h"
#include "FITKOFRunControl.h"
#include "FITKOFOperatingConditions.h"

namespace Interface
{
    FITKOFSolverInter::FITKOFSolverInter()
    {
        //设置求解器名称
        this->setDataObjectName("Inter");
        //设置启用湍流数据
        _enableTurb = true;
        //初始化
        this->init();
    }
    
    FITKOFSolverTypeEnum::FITKOFSolverType FITKOFSolverInter::getSolverType()
    {
        //获取求解器类型
        return FITKOFSolverTypeEnum::FITKOFSolverType::INTER;
    }

    QString FITKOFSolverInter::getSolverCommand()
    {
        return "interFoam";
    }

    void FITKOFSolverInter::initTransportProp()
    {
        //初始化材料
        if (!_transportProp) _transportProp = new FITKOFTransportProp;
        //phase
        FITKOFTransportPhase* phase1 = new FITKOFTransportPhase;
        phase1->setPhaseName("phase1");
        //初始化Phase1
        Core::FITKParameter* addPhase1 = new Core::FITKParameter;
        addPhase1->createDoubleParameter("rho", 1000);
        phase1->setPhaseAdditionalData(addPhase1);
        //初始化Phase1 - TransportModel
        FITKOFTransModelNewtonian* model1 = new FITKOFTransModelNewtonian;
        phase1->setTransportModel(model1);
        _transportProp->appendPhase(phase1);
        //初始化Phase2
        FITKOFTransportPhase* phase2 = new FITKOFTransportPhase;
        phase2->setPhaseName("phase2");
        Core::FITKParameter* addPhase2 = new Core::FITKParameter;
        addPhase2->createDoubleParameter("rho", 1.2);
        phase2->setPhaseAdditionalData(addPhase2);
        //初始化Phase2 - TransportModel
        FITKOFTransModelNewtonian* model2 = new FITKOFTransModelNewtonian;
        phase2->setTransportModel(model2);
        _transportProp->appendPhase(phase2);

        //addData
        Core::FITKParameter* addData = new Core::FITKParameter;
        addData->createDoubleParameter("sigma [N/m]", 0.07);
        _transportProp->setTransportAdditionalData(addData);
    }

    void FITKOFSolverInter::initDiscretization()
    {
        //初始化离散
        if (_discretization == nullptr) return;
        _discretization->initGradients();
        _discretization->initInterpolation();
        //Time
        Core::FITKParameter* timeOption = _discretization->getTimeOption();
        if (!timeOption)return;
        Core::FITKParamRadioGroup* timePara = timeOption->createRadioGroupParameter("Time Type");
        //创建修饰timetype的value
        Core::FITKParameter* decorated = new Core::FITKParameter;
        decorated->createBoolParameter("Bounded", false);
        timePara->setSubValue(decorated);
        //设置选择项
        timePara->appendValue("Implicit Euler", nullptr, QList<bool>() << true);
        timePara->appendValue("Backward", nullptr, QList<bool>() << true);
        Core::FITKParameter* crankNi = new Core::FITKParameter;
        crankNi->createDoubleParameter("Crank-Nicolson", 0.9);
        timePara->appendValue("Crank-Nicolson", crankNi, QList<bool>() << true);
        timePara->appendValue("Local Euler", nullptr, QList<bool>() << true);

        //添加U
        Core::FITKParameter* variableU = new Core::FITKParameter;
        _discretization->insertConvection("U", variableU);
        Core::FITKParamRadioGroup* convectionType = variableU->createRadioGroupParameter("Convection Type");
        //创建修饰convectionType的value
        decorated = new Core::FITKParameter;
        decorated->createBoolParameter("VectorSpecific", true);
        decorated->createBoolParameter("Bounded", false);
        decorated->createBoolParameter("SkewCorrected", false);
        convectionType->setSubValue(decorated);
        //设置选择项
        convectionType->appendValue("upwind", nullptr, QList<bool>() << false << true << true);
        convectionType->appendValue("linearUpwind", nullptr, QList<bool>() << true << true << true);
        convectionType->appendValue("linear", nullptr, QList<bool>() << false << true << true);
        Core::FITKParameter* vlimitedLinear = new Core::FITKParameter;
        vlimitedLinear->createDoubleParameter("limitedLinear", 1.0);
        convectionType->appendValue("limitedLinear", vlimitedLinear, QList<bool>() << true << true << true);
        convectionType->appendValue("LUST", nullptr, QList<bool>() << false << true << true);
        convectionType->appendValue("cubic", nullptr, QList<bool>() << false << true << true);
        convectionType->appendValue("vanLeer", nullptr, QList<bool>() << true << true << true);
        convectionType->appendValue("MUSCL", nullptr, QList<bool>() << true << true << true);
        convectionType->appendValue("QUICK", nullptr, QList<bool>() << true << true << true);
        Core::FITKParameter* vDEShybrid = new Core::FITKParameter;
        vDEShybrid->createDoubleParameter("DEShybrid", 10.0);
        vDEShybrid->createDoubleParameter("DEShybrid", 1.0);
        convectionType->appendValue("DEShybrid", vDEShybrid, QList<bool>() << false << true << true);
        //设置初始值
        convectionType->setCurrentValue("upwind");

    }

    void FITKOFSolverInter::initSolution()
    {
        //初始化solution
        if (!_solution)return;
        //初始化p_rgh的参数 solver
        FITKOFAlgebraicEquationsPara* p_rghPara = new FITKOFAlgebraicEquationsPara;
        p_rghPara->setSolutionSolver(new FITKOFSolutionSolverGAMG());
        p_rghPara->setVariableName("p_rgh");
        _solution->appendSolutionSolver(p_rghPara);
        //初始化U的参数 solver
        FITKOFAlgebraicEquationsPara* uPara = new FITKOFAlgebraicEquationsPara;
        uPara->setSolutionSolver(new FITKOFSolutionSolverPBiCGStabilized());
        uPara->setVariableName("U");
        _solution->appendSolutionSolver(uPara);
        //初始化Pcorr的参数 solver
        FITKOFAlgebraicEquationsPara* pcorrPara = new FITKOFAlgebraicEquationsPara;
        pcorrPara->setSolutionSolver(new FITKOFSolutionSolverGAMG());
        pcorrPara->setVariableName("pcorr");
        _solution->appendSolutionSolver(pcorrPara);
        //初始化alpha的参数 solver
        Core::FITKParameter* alpha = new Core::FITKParameter;
        alpha->createIntParameter("Alpha Sub-Cycles", 2);
        alpha->createIntParameter("Correctors", 2);
        alpha->createBoolParameter("MULES Corrector", false);
        alpha->createBoolParameter("Apply Previous Corrector", false);
        alpha->createIntParameter("Limiter Iterations", 3);
        alpha->createDoubleParameter("Calpha", 1);
        alpha->createDoubleParameter("ICalpha", 0);
        FITKOFAlgebraicEquationsPara* alphaPara = new FITKOFAlgebraicEquationsPara;
        alphaPara->setSolutionSolver(new FITKOFSolutionSolverSmoothSolver());
        alphaPara->setVariableName("alpha");
        alphaPara->setSolverAdditionalPara(alpha);
        _solution->appendSolutionSolver(alphaPara);
        
        //设置求解器有关的参数
        Core::FITKParameter* pimple = new Core::FITKParameter;
        
        pimple->createIntParameter("Outer Correctors", 1);
        pimple->createIntParameter("Correctors", 1);
        pimple->createIntParameter("Non-Orthogonal Correctors", 1);
        pimple->createBoolParameter("Momentum Predictor", true);
        pimple->createBoolParameter("Correct Phi", false);
        pimple->createBoolParameter("Mesh Outer Correctors", false);
        pimple->createBoolParameter("Check Mesh Co", false);
        _solution->setSpeciallyData("PIMPLE", pimple);
        //设置Residuals
        _solution->initAppendResidual("p_rgh", "PIMPLE");
        _solution->initAppendResidual("U", "PIMPLE");
        _solution->initAppendResidual("pcorr", "PIMPLE");
        //设置Relaxation
        Core::FITKParameter* relaxation = _solution->getRelaxation();
        if (!relaxation)return;
        relaxation->createDoubleParameter("p_rgh", 1);
        relaxation->createDoubleParameter("U", 1);
        //设置Limits
        Core::FITKParameter* limits = _solution->getLimits();
        if (!limits) return;
        Core::FITKParamBoolGroup* vG = limits->createBoolGroupParameter("Velocity Damping", false);
        Core::FITKParameter* childV = new Core::FITKParameter;
        childV->createDoubleParameter("Umax", 100);
        vG->setValueGroup(childV);
    }

    void FITKOFSolverInter::initBoundarys()
    {
        //初始化Wall边界信息
        FITKOFBoundary* boundaryWall = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall);
        boundaryWall->setFlowBoundary("p_rgh", new FITKOFBTyFixedFluxPressure);
        boundaryWall->setFlowBoundary("U", new FITKOFBTyNoSlip);
        boundaryWall->setPhasesBoundary("alpha.phase1", new FITKOFBTyZeroGradient);
        _boundaryInfos.append(boundaryWall);
        //初始化PressureInlet边界信息
        FITKOFBoundary* boundaryPInlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet);
        boundaryPInlet->setFlowBoundary("p_rgh", new FITKOFBTyPTotalPressure);
        boundaryPInlet->setFlowBoundary("U", new FITKOFBTyPressureIOVelocity);
        boundaryPInlet->setPhasesBoundary("alpha.phase1", new FITKOFBTyInletOutlet);
        _boundaryInfos.append(boundaryPInlet);
        //初始化VelocityInlet边界信息
        FITKOFBoundary* boundaryVInlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet);
        boundaryVInlet->setFlowBoundary("p_rgh", new FITKOFBTyFixedFluxPressure);
        boundaryVInlet->setFlowBoundary("U", new FITKOFBTySurNorFixedValue);
        boundaryVInlet->setPhasesBoundary("alpha.phase1", new FITKOFBTyFixedValue);
        _boundaryInfos.append(boundaryVInlet);
        //初始化PressureOutlet边界信息
        FITKOFBoundary* boundaryPOutlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet);
        boundaryPOutlet->setFlowBoundary("p_rgh", new FITKOFBTyPFixedValue);
        boundaryPOutlet->setFlowBoundary("U", new FITKOFBTyPressureIOVelocity);
        boundaryPOutlet->setPhasesBoundary("alpha.phase1", new FITKOFBTyZeroGradient);
        _boundaryInfos.append(boundaryPOutlet);
        //初始化Outflow
        FITKOFBoundary* boundaryOutflow = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow);
        boundaryOutflow->setFlowBoundary("p_rgh", new FITKOFBTyZeroGradient);
        boundaryOutflow->setFlowBoundary("U", new FITKOFBTyZeroGradient);
        boundaryOutflow->setPhasesBoundary("alpha.phase1", new FITKOFBTyZeroGradient);
        _boundaryInfos.append(boundaryOutflow);
        //初始化Symmetry
        FITKOFBoundary* boundarySymmetry = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BSymmetry);
        boundarySymmetry->setFlowBoundary("p_rgh", new FITKOFBTySymmetry);
        boundarySymmetry->setFlowBoundary("U", new FITKOFBTySymmetry);
        boundarySymmetry->setPhasesBoundary("alpha.phase1", new FITKOFBTySymmetry);
        _boundaryInfos.append(boundarySymmetry);
        //初始化Wedge
        FITKOFBoundary* boundaryWedge = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWedge);
        boundaryWedge->setFlowBoundary("p_rgh", new FITKOFBTyWedge);
        boundaryWedge->setFlowBoundary("U", new FITKOFBTyWedge);
        boundaryWedge->setPhasesBoundary("alpha.phase1", new FITKOFBTyWedge);
        _boundaryInfos.append(boundaryWedge);
    }

    void FITKOFSolverInter::initInitialConditions()
    {
        //初始化Initial Conditions数据
        if (!_initialCond)return;
        //初始化Basic
        Core::FITKParameter* basic = _initialCond->getBasicData();
        if (!basic)return;
        basic->createDoubleParameter("p_rgh", 0);
        basic->createDoubleListParameter("U", QList<double>() << 0 << 0 << 0);
        basic->createDoubleParameter("alpha.phase1", 0);
    }

    void FITKOFSolverInter::initRunControl()
    {
        //初始化运行配置
        if (!_runControl) return;
        //初始化时间配置
        Core::FITKParameter* timeControl = _runControl->getTimeControl();
        if (!timeControl)return;
        timeControl->createDoubleParameter("Simulation Time [s]", 1);
        timeControl->createDoubleParameter("Intial Time [s]", 1);
        Core::FITKParamBoolGroup* timeStep = timeControl->createBoolGroupParameter("Adjust Time Stepping", false);
        Core::FITKParameter* timeStepChild = new Core::FITKParameter;
        timeStepChild->createDoubleParameter("max Delta Time", 1);
        timeStepChild->createDoubleParameter("max Co", 2);
        timeStepChild->createDoubleParameter("max Alpha Co", 0.5);
        timeStep->setValueGroup(timeStepChild);
        //初始化输出配置
        Core::FITKParameter* outputControl = _runControl->getOutputControl();
        if (!outputControl)return;
        outputControl->createDoubleParameter("Write Interval", 1);
        Core::FITKParamBoolGroup* cleanResult = outputControl->createBoolGroupParameter("Clean Old Result Files", true);
        Core::FITKParameter* cleanResultChild = new Core::FITKParameter;
        cleanResultChild->createIntParameter("Max No.of Files", 2);
        cleanResult->setValueGroup(cleanResultChild);
    }

    void FITKOFSolverInter::initOperatingConditions()
    {
        if (!_operatingCond) _operatingCond = new FITKOFOperatingConditions;
        _operatingCond->initGravitationalAcceleration();
        _operatingCond->initReferencePressure();
    }
}



