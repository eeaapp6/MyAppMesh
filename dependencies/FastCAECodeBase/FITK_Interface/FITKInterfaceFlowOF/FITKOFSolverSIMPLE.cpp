/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFSolverSIMPLE.h"
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
    FITKOFSolverSIMPLE::FITKOFSolverSIMPLE()
    {
        //设置求解器名称
        this->setDataObjectName("SIMPLE");
        //设置启用湍流数据
        _enableTurb = true;
        //初始化
        this->init();
    }
    
    FITKOFSolverTypeEnum::FITKOFSolverType FITKOFSolverSIMPLE::getSolverType()
    {
        //获取求解器类型
        return FITKOFSolverTypeEnum::SIMPLE;
    }

    QString FITKOFSolverSIMPLE::getSolverCommand()
    {
        return "simpleFoam";
    }

    void FITKOFSolverSIMPLE::initTransportProp()
    {
        //初始化材料
        if (!_transportProp) _transportProp = new FITKOFTransportProp;
        FITKOFTransportPhase* phase1 = new FITKOFTransportPhase;
        phase1->setPhaseName("phase1");
        FITKOFTransModelNewtonian* model1 = new FITKOFTransModelNewtonian;
        phase1->setTransportModel(model1);
        _transportProp->appendPhase(phase1);
    }

    void FITKOFSolverSIMPLE::initDiscretization()
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
        timePara->appendValue("Steady State", nullptr, QList<bool>() << true);

        //添加U
        Core::FITKParameter* variableU = new Core::FITKParameter;
        _discretization->insertConvection("U", variableU);
        Core::FITKParamRadioGroup* convectionType = variableU->createRadioGroupParameter("Convection Type");
        //创建修饰convectionType的value
        decorated = new Core::FITKParameter;
        decorated->createBoolParameter("VectorSpecific", true);
        decorated->createBoolParameter("Bounded", true);
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

    void FITKOFSolverSIMPLE::initSolution()
    {
        //初始化solution
        if (!_solution)return;
        //初始化p的参数 solver
        FITKOFAlgebraicEquationsPara* pPara = new FITKOFAlgebraicEquationsPara;
        pPara->setSolutionSolver(new FITKOFSolutionSolverGAMG());
        pPara->setVariableName("p");
        _solution->appendSolutionSolver(pPara);
        //初始化U的参数 solver
        FITKOFAlgebraicEquationsPara* uPara = new FITKOFAlgebraicEquationsPara;
        uPara->setSolutionSolver(new FITKOFSolutionSolverPBiCGStabilized());
        uPara->setVariableName("U");
        _solution->appendSolutionSolver(uPara);
        //设置求解器有关的参数
        Core::FITKParameter* simple = new Core::FITKParameter;
        simple->createBoolParameter("Consistent", true);
        simple->createIntParameter("Non-Orthogonal Correctors", 1);
        _solution->setSpeciallyData("SIMPLE", simple);
        //设置Residuals
        _solution->initAppendResidual("p", "SIMPLE");
        _solution->initAppendResidual("U", "SIMPLE");
        //设置Relaxation
        Core::FITKParameter* relaxation = _solution->getRelaxation();
        if (!relaxation)return;
        relaxation->createDoubleParameter("p", 0.3);
        relaxation->createDoubleParameter("U", 0.7);
        //设置Limits
        Core::FITKParameter* limits = _solution->getLimits();
        if (!limits) return;
        Core::FITKParamBoolGroup* vG = limits->createBoolGroupParameter("Velocity Damping", false);
        Core::FITKParameter* childV = new Core::FITKParameter;
        childV->createDoubleParameter("Umax", 100);
        vG->setValueGroup(childV);
    }

    void FITKOFSolverSIMPLE::initBoundarys()
    {
        //初始化Wall边界信息
        FITKOFBoundary* boundaryWall = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall);
        boundaryWall->setFlowBoundary("p", new FITKOFBTyZeroGradient);
        boundaryWall->setFlowBoundary("U", new FITKOFBTyNoSlip);
        _boundaryInfos.append(boundaryWall);
        //初始化PressureInlet边界信息
        FITKOFBoundary* boundaryPInlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet);
        boundaryPInlet->setFlowBoundary("p", new FITKOFBTyTKETotalPressure);
        boundaryPInlet->setFlowBoundary("U", new FITKOFBTyPressureIOVelocity);
        _boundaryInfos.append(boundaryPInlet);
        //初始化VelocityInlet边界信息
        FITKOFBoundary* boundaryVInlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet);
        boundaryVInlet->setFlowBoundary("p", new FITKOFBTyZeroGradient);
        boundaryVInlet->setFlowBoundary("U", new FITKOFBTySurNorFixedValue);
        _boundaryInfos.append(boundaryVInlet);
        //初始化PressureOutlet边界信息
        FITKOFBoundary* boundaryPOutlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet);
        boundaryPOutlet->setFlowBoundary("p", new FITKOFBTyTKEFixedValue);
        boundaryPOutlet->setFlowBoundary("U", new FITKOFBTyPressureIOVelocity);
        _boundaryInfos.append(boundaryPOutlet);

    }

    void FITKOFSolverSIMPLE::initInitialConditions()
    {
        //初始化Initial Conditions数据
        if (!_initialCond)return;
        //初始化Basic
        Core::FITKParameter* basic = _initialCond->getBasicData();
        if (!basic)return;
        basic->createDoubleParameter("p", 0);
        basic->createDoubleListParameter("U", QList<double>() << 0 << 0 << 0);
    }

    void FITKOFSolverSIMPLE::initRunControl()
    {
        //初始化运行配置
        if (!_runControl) return;
        //初始化时间配置
        Core::FITKParameter* timeControl = _runControl->getTimeControl();
        if (!timeControl)return;
        timeControl->createIntParameter("Number of Iterations", 100, 99999999, 1);

        //初始化输出配置
        Core::FITKParameter* outputControl = _runControl->getOutputControl();
        if (!outputControl)return;
        outputControl->createIntParameter("Write Interval", 10);
        Core::FITKParamBoolGroup* cleanResult = outputControl->createBoolGroupParameter("Clean Old Result Files", true);
        Core::FITKParameter* cleanResultChild = new Core::FITKParameter;
        cleanResultChild->createIntParameter("Max No.of Files", 2);
        cleanResult->setValueGroup(cleanResultChild);
    }

    void FITKOFSolverSIMPLE::initOperatingConditions()
    {
        if (!_operatingCond) _operatingCond = new FITKOFOperatingConditions;
        _operatingCond->initReferencePressure();
    }
}



