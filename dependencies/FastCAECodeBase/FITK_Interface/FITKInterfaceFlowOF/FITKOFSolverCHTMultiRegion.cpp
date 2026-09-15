/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFSolverCHTMultiRegion.h"
#include "FITKOFDiscretization.h"
#include "FITKOFRadiationModel.h"
#include "FITKOFRadiationAbsorptionModel.h"
#include "FITKOFRadiationScatterModel.h"
#include "FITKOFRadiationSunDirection.h"
#include "FITKOFRadiationSunLoad.h"
#include "FITKOFThermoPhysicalEquationOfState.h"
#include "FITKOFThermoPhysicalTransport.h"
#include "FITKOFThermoPhysicalThermodynamics.h"

#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamRadioGroup.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBoolGroup.h"

#include "FITKOFSolution.h"
#include "FITKOFOperatingConditions.h"
#include "FITKOFSolutionSolver.h"
#include "FITKOFRunControl.h"
#include "FITKOFInitialConditions.h"
#include "FITKOFRadiation.h"
#include "FITKOFThermo.h"
#include "FITKOFBoundary.h"
#include "FITKOFBoundaryType.h"

#include "FITKOFEnum.hpp"

namespace Interface
{
    FITKOFSolverCHTMultiRigon::FITKOFSolverCHTMultiRigon()
    {
        this->setDataObjectName("CHT Multi Region");
        
         _enableTurb= true;
         this->init();
    }

    FITKOFSolverTypeEnum::FITKOFSolverType FITKOFSolverCHTMultiRigon::getSolverType()
    {
        return FITKOFSolverTypeEnum::CHT_MULTI_REGION;
    }

    QString FITKOFSolverCHTMultiRigon::getSolverCommand()
    {
        return "foamMultiRun";
    }

    void FITKOFSolverCHTMultiRigon::initRadiation()
    {
        if (_radiation == nullptr) _radiation = new FITKOFRadiation;
        //初始化辐射参数
        Core::FITKParameter * radiationPara = _radiation->getRadiationParam();
        if (!radiationPara) return;
        Core::FITKParamBoolGroup* enableRad = radiationPara->createBoolGroupParameter("Enable Radiation", true);
        Core::FITKParameter* childV = new Core::FITKParameter;
        childV->createIntParameter("Solution Frequency", 5);
        enableRad->setValueGroup(childV);
        //初始化辐射模型、吸收系数、散射系数
        _radiation->setRadiationModel(new FITKOFRadiationModelNone);
        _radiation->setAbsorptionModel(new FITKOFRadiationAbsorptionModelNone);
        _radiation->setScatterModel(new FITKOFRadiationScatterModelNone);
        //初始化太阳辐射源
        FITKOFRadiationSolarLoad * solarLoad = _radiation->getSolarLoad();
        if (!solarLoad) return;
        Core::FITKParameter * solarLoadPara = solarLoad->getRadiationSolarLoadParams();
        if (!solarLoadPara) return;
        solarLoadPara->createBoolParameter("Enable Solar load", false);
        solarLoadPara->createBoolParameter("Reflected Rays", true);

        solarLoad->setRadiationSunDirection(new FITKOFRadiationSunDirectionConstant);
        solarLoad->setRadiationSunLoad(new FITKOFRadiationSunLoadConstant);
    }

    void FITKOFSolverCHTMultiRigon::initThermo()
    {
        if (_thermo == nullptr) _thermo = new FITKOFThermo;
        //初始化流体区域热物理属性
        FITKOFThermoPhysicalProp * fluidThermoPhy = new FITKOFThermoPhysicalProp;
        _thermo->addThermoPropRegion(fluidThermoPhy);
        //初始化数据
        fluidThermoPhy->setRegionMeshType(FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid);
        Core::FITKParameter * specie = fluidThermoPhy->getSpeciePara();
        if (!specie) return;
        specie->createDoubleParameter("molWeight", 28.96);
        fluidThermoPhy->setEquationOfStatePara(new FITKOFThermoPhysicalEquationOfStatePerfectGas);
        fluidThermoPhy->setThermodynamicsPara(new FITKOFThermoPhysicalThermodynamicsConstant);
        fluidThermoPhy->setTransportPara(new FITKOFThermoPhysicalTransportConstant);
        //初始化固体区域热物理属性
        FITKOFThermoPhysicalProp * solidThermoPhy = new FITKOFThermoPhysicalProp;
        _thermo->addThermoPropRegion(solidThermoPhy);
        //初始化数据
        solidThermoPhy->setRegionMeshType(FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid);
        specie = solidThermoPhy->getSpeciePara();
        if (!specie) return;
        specie->createDoubleParameter("molWeight", 28.96);
        solidThermoPhy->setEquationOfStatePara(new FITKOFThermoPhysicalEquationOfStateConstantDensity);
        solidThermoPhy->setThermodynamicsPara(new FITKOFThermoPhysicalThermodynamicsEConstant);
        solidThermoPhy->setTransportPara(new FITKOFThermoPhysicalTransportConstantIsotropic);
    }

    void FITKOFSolverCHTMultiRigon::initTransportProp()
    {
        // 初始化材料属性
    }

    void FITKOFSolverCHTMultiRigon::initDiscretization()
    {
        // 初始化离散化数据
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

        //添加h
        Core::FITKParameter* variableh = new Core::FITKParameter;
        _discretization->insertConvection("h", variableh);
        convectionType = variableh->createRadioGroupParameter("Convection Type");
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
        vlimitedLinear = new Core::FITKParameter;
        vlimitedLinear->createDoubleParameter("limitedLinear", 1.0);
        convectionType->appendValue("limitedLinear", vlimitedLinear, QList<bool>() << true << true << true);
        convectionType->appendValue("LUST", nullptr, QList<bool>() << false << true << true);
        convectionType->appendValue("cubic", nullptr, QList<bool>() << false << true << true);
        convectionType->appendValue("vanLeer", nullptr, QList<bool>() << true << true << true);
        convectionType->appendValue("MUSCL", nullptr, QList<bool>() << true << true << true);
        convectionType->appendValue("QUICK", nullptr, QList<bool>() << true << true << true);
        vDEShybrid = new Core::FITKParameter;
        vDEShybrid->createDoubleParameter("DEShybrid", 10.0);
        vDEShybrid->createDoubleParameter("DEShybrid", 1.0);
        convectionType->appendValue("DEShybrid", vDEShybrid, QList<bool>() << false << true << true);
        //设置初始值
        convectionType->setCurrentValue("upwind");
    }

    void FITKOFSolverCHTMultiRigon::initSolution()
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
        //初始化h的参数 solver
        FITKOFAlgebraicEquationsPara* hPara = new FITKOFAlgebraicEquationsPara;
        hPara->setSolutionSolver(new FITKOFSolutionSolverPBiCGStabilized());
        hPara->setVariableName("h");
        _solution->appendSolutionSolver(hPara);
        //初始化p的参数 solver
        FITKOFAlgebraicEquationsPara* pPara = new FITKOFAlgebraicEquationsPara;
        pPara->setSolutionSolver(new FITKOFSolutionSolverPCG());
        pPara->setVariableName("rho");
        _solution->appendSolutionSolver(pPara);
        //初始化h(solid)的参数 solver
        FITKOFAlgebraicEquationsPara* hSolidPara = new FITKOFAlgebraicEquationsPara;
        hSolidPara->setSolutionSolver(new FITKOFSolutionSolverPCG());
        hSolidPara->setVariableName("e(solid)");
        _solution->appendSolutionSolver(hSolidPara);

        //设置求解器有关的参数
        Core::FITKParameter* pimple = new Core::FITKParameter;

        pimple->createIntParameter("Outer Correctors", 1);
        pimple->createIntParameter("Correctors", 1);
        pimple->createIntParameter("Non-Orthogonal Correctors", 1);
        pimple->createBoolParameter("Momentum Predictor", true);
        pimple->createBoolParameter("Frozen Flow", false);
        pimple->createIntParameter("Energy Sub-cycling", 10);
        pimple->createIntParameter("Non-Orthogonal Correctors (solid)", 1);
        _solution->setSpeciallyData("PIMPLE", pimple);
        //设置Residuals
        _solution->initAppendResidual("p_rgh", "PIMPLE");
        _solution->initAppendResidual("U", "PIMPLE");
        _solution->initAppendResidual("h", "PIMPLE");
        _solution->initAppendResidual("rho", "PIMPLE");
        //设置Relaxation
        Core::FITKParameter* relaxation = _solution->getRelaxation();
        if (!relaxation)return;
        relaxation->createDoubleParameter("h (solid)", 1);
        relaxation->createDoubleParameter("p_rgh", 1);
        relaxation->createDoubleParameter("U", 1);
        //设置Limits
        Core::FITKParameter* limits = _solution->getLimits();
        if (!limits) return;
        Core::FITKParamBoolGroup* pl = limits->createBoolGroupParameter("Pressure Limits", false);
        Core::FITKParameter* childV = new Core::FITKParameter;
        childV->createDoubleParameter("Pmin [Pa]", 0, 1e38, 0);
        childV->createDoubleParameter("Pmax [Pa]", 1e10, 1e38, 0);
        pl->setValueGroup(childV);
        Core::FITKParamBoolGroup* tl = limits->createBoolGroupParameter("Temperature Limits", false);
        childV = new Core::FITKParameter;
        childV->createDoubleParameter("Tmin [K]", 200);
        childV->createDoubleParameter("Tmax [K]", 2000);
        tl->setValueGroup(childV);
        Core::FITKParamBoolGroup* vd = limits->createBoolGroupParameter("Velocity Damping", false);
        childV = new Core::FITKParameter;
        childV->createDoubleParameter("Umax [m/s]", 100);
        vd->setValueGroup(childV);
    }

    void FITKOFSolverCHTMultiRigon::initBoundarys()
    {
        //初始化Wall边界信息
        FITKOFBoundary* boundaryWall = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall);
        boundaryWall->setFlowBoundary("p_rgh", new FITKOFBTyFixedFluxPressure);
        boundaryWall->setFlowBoundary("U", new FITKOFBTyNoSlip);
        boundaryWall->setThermalBoundary("T", new FITKOFBTyZeroGradient);
        _boundaryInfos.append(boundaryWall);
        //初始化MappedWell
        FITKOFBoundary* boundaryMappedWell = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BMappedWall);
        boundaryMappedWell->setFlowBoundary("p_rgh", new FITKOFBTyFixedFluxPressure);
        boundaryMappedWell->setFlowBoundary("U", new FITKOFBTyNoSlip);
        boundaryMappedWell->setThermalBoundary("T", new FITKOFBTyCoupledTemperatureAndRadiation);
        _boundaryInfos.append(boundaryMappedWell);
        //初始化Empty
        FITKOFBoundary* boundaryEmpty = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BEmpty);
        boundaryEmpty->setFlowBoundary("p_rgh", new FITKOFBTyEmpty);
        boundaryEmpty->setFlowBoundary("U", new FITKOFBTyEmpty);
        boundaryEmpty->setThermalBoundary("T", new FITKOFBTyEmpty);
        _boundaryInfos.append(boundaryEmpty);
        //初始化PressureInlet边界信息
        FITKOFBoundary* boundaryPInlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet);
        boundaryPInlet->setFlowBoundary("p_rgh", new FITKOFBTyPTotalPressure);
        boundaryPInlet->setFlowBoundary("U", new FITKOFBTyPressureIOVelocity);
        boundaryPInlet->setThermalBoundary("T", new FITKOFBTyTotalTemperature);
        _boundaryInfos.append(boundaryPInlet);
        //初始化PressureOutlet边界信息
        FITKOFBoundary* boundaryPOutlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet);
        boundaryPOutlet->setFlowBoundary("p_rgh", new FITKOFBTyPFixedValue);
        boundaryPOutlet->setFlowBoundary("U", new FITKOFBTyPressureIOVelocity);
        boundaryPOutlet->setThermalBoundary("T", new FITKOFBTyInletOutlet);
        _boundaryInfos.append(boundaryPOutlet);
        //初始化VelocityInlet边界信息
        FITKOFBoundary* boundaryVInlet = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet);
        boundaryVInlet->setFlowBoundary("p_rgh", new FITKOFBTyFixedFluxPressure);
        boundaryVInlet->setFlowBoundary("U", new FITKOFBTySurNorFixedValue);
        boundaryVInlet->setThermalBoundary("T", new FITKOFBTyTotalTemperature);
        _boundaryInfos.append(boundaryVInlet);
        //初始化Outflow
        FITKOFBoundary* boundaryOutflow = new FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow);
        boundaryOutflow->setFlowBoundary("p_rgh", new FITKOFBTyZeroGradient);
        boundaryOutflow->setFlowBoundary("U", new FITKOFBTyZeroGradient);
        boundaryOutflow->setThermalBoundary("T", new FITKOFBTyZeroGradient);
        _boundaryInfos.append(boundaryOutflow);
    }

    void FITKOFSolverCHTMultiRigon::initInitialConditions()
    {
        // 初始化初始条件数据
        if (!_initialCond) return;
        _initialCond->setInitPropRegions(true);
        //流体区域模板
        FITKOFInitPropRegions * fluidInitProp = new FITKOFInitPropRegions;
        fluidInitProp->setRegionMeshType(FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid);
        _initialCond->addInitPropRegion(fluidInitProp);
        Core::FITKParameter * fluidReg = fluidInitProp->getRegionPara();
        if (!fluidReg) return;
        fluidReg->createDoubleParameter("p_rgh", 0);
        fluidReg->createDoubleListParameter("U", QList<double>() << 0 << 0 << 0);
        fluidReg->createDoubleParameter("T", 0);
        //固体区域模板
        FITKOFInitPropRegions * solidInitProp = new FITKOFInitPropRegions;
        solidInitProp->setRegionMeshType(FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid);
        _initialCond->addInitPropRegion(solidInitProp);
        Core::FITKParameter * solidReg = solidInitProp->getRegionPara();
        if (!solidReg) return;
        solidReg->createDoubleParameter("T", 0);
    }

    void FITKOFSolverCHTMultiRigon::initRunControl()
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
        timeStepChild->createDoubleParameter("max Co", 0);
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

    void FITKOFSolverCHTMultiRigon::initOperatingConditions()
    {
        // 初始化操作条件
        if (!_operatingCond) _operatingCond = new FITKOFOperatingConditions;
        _operatingCond->initGravitationalAcceleration();
        _operatingCond->initReferencePressure();
    }
} // namespace Interface

