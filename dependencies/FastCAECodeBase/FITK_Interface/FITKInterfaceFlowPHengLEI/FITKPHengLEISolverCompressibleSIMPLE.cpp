/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEISolverCompressibleSIMPLE.h"
#include "FITKPHengLEIGeneralControl.h"
#include "FITKPHengLEIInFlow.h"
#include "FITKPHengLEIMeshPara.h"
#include "FITKPHengLEIViscousModel.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Kernel/FITKEasyParam/FITKAbstractEasyParam.h"

#include "FITKPHengLEIInFlowTypeNondimensional.h"
#include "FITKPHengLEISpatialUnStruct.h"
#include "FITKPHengLEIDiscretisation.h"
#include "FITKPHengLEIBoundary.h"
#include "FITKPHengLEIBoundaryType.h"
#include "FITKPHengLEIBoundaryFarfieldType.h"
#include "FITKPHengLEIBoundarySolidSurfaceType.h"
#include "FITKPHengLEIBoundaryInFlowType.h"
#include "FITKPHengLEIMultiGrid.h"
#include "FITKPHengLEIOutputSetting.h"
#include "FITKPHengLEIFlow.h"
#include "FITKPHengLEIViscousType.h"
#include "FITKPHengLEICalculation.h"
#include "FITKPHengLEIInitialCondition.h"
#include "FITKPHengLEISolution.h"
#include "FITKPHengLEIModels.h"
#include "FITKPHengLEIEnergyModel.h"
#include "FITKPHengLEISpeciesModel.h"

namespace Interface
{

    FITKPHengLEISolverCompressibleSIMPLE::FITKPHengLEISolverCompressibleSIMPLE()
    {
		this->setDataObjectName("CompressibleSIMPLE");

        this->init();
    }

    FITKPHengLEIAbsSolver::PHengLEISolverType FITKPHengLEISolverCompressibleSIMPLE::getSolverType()
    {
        //CompressibleSIMPLE类型
        return  FITKPHengLEIAbsSolver::PHengLEISolverType::PLCompressibleSIMPLE;
    }

    void FITKPHengLEISolverCompressibleSIMPLE::initBoundaryTypes()
    {
		//FITKPHengLEIBoundary* boundaryFarfield = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBFarField);
		//boundaryFarfield->setBoundaryType(new FarfieldNone);
		//_boundarys->appendDataObj(boundaryFarfield);

		FITKPHengLEIBoundary* boundaryWall = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBWall);
		boundaryWall->setBoundaryType(new WallNone);
		_boundarys->appendDataObj(boundaryWall);

		FITKPHengLEIBoundary* boundaryInFLow = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBInFlow);
		boundaryInFLow->setBoundaryType(new InflowNone);
		_boundarys->appendDataObj(boundaryInFLow);

		FITKPHengLEIBoundary* boundaryOutFlow = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PhBOutFlow);
		boundaryOutFlow->setBoundaryType(new OutFLow);
		_boundarys->appendDataObj(boundaryOutFlow);


		//FITKPHengLEIBoundary* boundaryPressureInlet = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBPressureInlet);
		//boundaryPressureInlet->setBoundaryType(new PressureInlet);
		//_boundarys->appendDataObj(boundaryPressureInlet);

		//FITKPHengLEIBoundary* boundaryPressureOutlet = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBPressureOutlet);
		//boundaryPressureOutlet->setBoundaryType(new PressureOutlet);
		//_boundarys->appendDataObj(boundaryPressureOutlet);

		//FITKPHengLEIBoundary* boundaryMassFlowInlet = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBMassFlowInlet);
		//boundaryMassFlowInlet->setBoundaryType(new MassFlowInlet);
		//_boundarys->appendDataObj(boundaryMassFlowInlet);

		//FITKPHengLEIBoundary* boundaryMassFlowOutlet = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBMassFlowOutlet);
		//boundaryMassFlowOutlet->setBoundaryType(new MassFlowOutlet);
		//_boundarys->appendDataObj(boundaryMassFlowOutlet);

		//FITKPHengLEIBoundary* boundarySYMMETRY = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBSYMMETRY);
		//boundarySYMMETRY->setBoundaryType(new SYMMETRY);
		//_boundarys->appendDataObj(boundarySYMMETRY);
    }

    void FITKPHengLEISolverCompressibleSIMPLE::initBasicInfo()
    {
    }

    void FITKPHengLEISolverCompressibleSIMPLE::initInflow()
    {
		if (_flow) delete _flow;
		_flow = nullptr;
		return;
    }

    void FITKPHengLEISolverCompressibleSIMPLE::initGridParam()
    {
		if (_meshPara) delete _meshPara;
		_meshPara = nullptr;
		return;
    }

    void FITKPHengLEISolverCompressibleSIMPLE::initViscous()
    {

    }

    void FITKPHengLEISolverCompressibleSIMPLE::initDiscrete()
    {
		Core::FITKParameter* flowData = _discretisation->getFlowDiscretisation();
		if (!flowData) return;
		QStringList ConvectionMethodList = {"UPWIND","CDS","QUICK","SUDS"};
		flowData->createComboxParameter("Convection Method", ConvectionMethodList, 0);

		QStringList DiffusionMethodLsit = {"NON_ORTHOGONAL"};
		flowData->createComboxParameter("Diffusion Method", DiffusionMethodLsit, 0);

		QStringList TimeIntegrationMethodList = {"IMPLICIT_EULER","IMPLICIT_2ND_ORDER"};
		flowData->createComboxParameter("Time Integration Method", TimeIntegrationMethodList, 0);

		QStringList GradientMethodList = {"GAUSS","LSQ"};
		flowData->createComboxParameter("Gradient Method", GradientMethodList, 0);

		QStringList MathematicalLibraryTypeList = {"hypre","unap","yhamg"};
		flowData->createComboxParameter("Mathematical Library Type", MathematicalLibraryTypeList, 0);

		QStringList CouplingList = {"SIMPLE","SIMPLEC"};
		flowData->createComboxParameter("Pressure-Velocity Coupling", CouplingList, 1);

		flowData->createBoolParameter("Gradient Correction", false);

		QStringList DensityModelList = { "Constant","Incomp. IG","Ideal Gas"};
		flowData->createComboxParameter("Density Model", DensityModelList, 0);

		QStringList ViscosityModelList = {"Constant","Sutherland","Gas Mixing"};
		flowData->createComboxParameter("Viscosity Model", ViscosityModelList, 0);

		QStringList ThermalConductivityList = { "Constant","Mass-Mixed","Gas Mixing" };
		flowData->createComboxParameter("Thermal Conductivity", ThermalConductivityList, 0);

		QStringList SpecificHeatList = { "Constant","Mixing Law" };
		flowData->createComboxParameter("Specific Heat", SpecificHeatList, 0);

		QStringList MassDiffusionModelList = { "None","Const Dilute","Dilute Approx" };
		flowData->createComboxParameter("Mass Diffusion Model", MassDiffusionModelList, 0);
    }
	

    void FITKPHengLEISolverCompressibleSIMPLE::initMultiGridParam()
    {
		if (_multiGRID) delete _multiGRID;
		_multiGRID = nullptr;
		return;
    }

	void FITKPHengLEISolverCompressibleSIMPLE::initOutputSetting()
	{
		if (!_outputSetting) return;
		Core::FITKParameter* basicFlowPara = _outputSetting->getBasicFlowData();

		basicFlowPara->createBoolParameter("Velocity Component U", true)->setData(22);
		basicFlowPara->createBoolParameter("Velocity Component V", true)->setData(23);
		basicFlowPara->createBoolParameter("Velocity Component W", true)->setData(24);
		basicFlowPara->createBoolParameter("Pressure", true)->setData(25);
		basicFlowPara->createBoolParameter("Specific Heat Capacity", false)->setData(26);
		basicFlowPara->createBoolParameter("Temperature", false)->setData(27);
		basicFlowPara->createBoolParameter("Density", false)->setData(28);
		basicFlowPara->createBoolParameter("Viscosity", false)->setData(29);
		basicFlowPara->createBoolParameter("Total Energy", false)->setData(31);
		basicFlowPara->createBoolParameter("Internal Energy", false)->setData(32);
		basicFlowPara->createBoolParameter("Enthalpy", false)->setData(70);

		// 根据物种方程，获取物种分量的数量，再依次创建物种分量1，物种分量2，物种分量3，......
		if (!_models) return;
		FITKPHengLEISpeciesModel* speciesModel = _models->getSpeciesModel();
		if (!speciesModel) return;
		Core::FITKParameter* speciesPara = speciesModel->getAdditionalData();
		if (!speciesPara) return;
		bool OK{};
		int speciesCount = speciesPara->getIntParaByName("Solve Species Number", OK);
		if (!OK) return;
		for (int i = 1; i <= speciesCount; i++)
		{
			QString VariableName = QString("species_%1").arg(i);
			speciesPara->createBoolParameter(VariableName, false)->setData(6 + i);
		}
	}

	void FITKPHengLEISolverCompressibleSIMPLE::initFlow()
	{
		if (_flow) delete _flow;
		_flow = nullptr;
		return;
	}

	void FITKPHengLEISolverCompressibleSIMPLE::initCalculation()
	{
		if (!_calculation) return;
		Core::FITKParameter* addData = _calculation->getAdditionalConvergenceData();
		if (!addData) return;
		addData->createIntParameter("Max Intervals Step", 10);

		Core::FITKParameter* addSavingIntervals = _calculation->getAdditionalSavingIntervalsData();
		if (!addSavingIntervals) return;
		addSavingIntervals->createIntParameter("Flow Step", 1);
		addSavingIntervals->createIntParameter("Plot Step", 1);
		addSavingIntervals->createIntParameter("Force Step", 1);
		addSavingIntervals->createIntParameter("Res Step", 1);

		Core::FITKParameter* addParallelData = _calculation->getAdditionalParallelData();
		if (!addParallelData) return;
		addParallelData->createIntParameter("Processes", 4, 4, 1);

		Core::FITKParameter* addSolverControlData = _calculation->getAdditionalSolverControlData();
		if (!addSolverControlData) return;
		addSolverControlData->createDoubleParameter("Start Time", 0);
		addSolverControlData->createDoubleParameter("End Time", 2);
		addSolverControlData->createDoubleParameter("Dt", 0.01);
		addSolverControlData->createIntParameter("Min Sub Iter", 5);
		addSolverControlData->createIntParameter("Max Sub Iter", 50);
		addSolverControlData->createDoubleParameter("Tol Sub Iter", 0.00001);
		addSolverControlData->createBoolParameter("Enable Stable Unsteady Method", true);
	}

	void FITKPHengLEISolverCompressibleSIMPLE::initInitialCondition()
	{
		if (!_initialCondition) return;

		Core::FITKParameter* addData = _initialCondition->getInitialConditionAdditionalData();
		if (!addData)return;
		addData->createDoubleParameter("Density", 1.25);
		QList<double> VelocityValue{10,0,0};
		addData->createDoubleListParameter("Velocity",VelocityValue);
		addData->createDoubleParameter("Pressure", 0);
		addData->createDoubleParameter("Dynamic Viscosity", 1);
		addData->createDoubleParameter("Gas Constant", 296.928571);
		addData->createDoubleParameter("Reference Pressure", 0);
		addData->createDoubleParameter("Reference Pressure Location", 0);

	}

	void FITKPHengLEISolverCompressibleSIMPLE::initSolution()
	{
		QStringList SolutionMethod = { "GaussSeidel","GMRES", "Bicgstab","AMG","PCG","FlexGMRES","ILU","AMGDD","Hybrid"};
		QStringList PreconditionerMethod = { "None","ILU","AMG","Euclid","LduDiag","LduDIC","LduDILU" };

		if (!_solution) return;
		Core::FITKParameter* momentumEquationData = _solution->getAdditionalMomentumEquationData();
		momentumEquationData->createComboxParameter("Solver Method", SolutionMethod,2);
		momentumEquationData->createComboxParameter("Preconditioner", PreconditionerMethod,1);
		momentumEquationData->createIntParameter("Max Sweep", 30);
		momentumEquationData->createDoubleParameter("Iter Sol Tolerence", 1e-8);
		momentumEquationData->createDoubleParameter("Relax Coeff", 0.5);

		Core::FITKParameter* energyEquationData = _solution->getAdditionalEnergyEquationData();
		energyEquationData->createComboxParameter("Solver Method", SolutionMethod, 3);
		energyEquationData->createComboxParameter("Preconditioner", PreconditionerMethod, 1);
		energyEquationData->createIntParameter("Max Sweep", 30);
		energyEquationData->createDoubleParameter("Iter Sol Tolerence", 1e-8);
		energyEquationData->createDoubleParameter("Relax Coeff", 0.3);

		Core::FITKParameter* turbulenceEquationData = _solution->getAdditionalTurbulenceEquationData();
		turbulenceEquationData->createComboxParameter("Solver Method", SolutionMethod, 0);
		turbulenceEquationData->createComboxParameter("Preconditioner", PreconditionerMethod, 0);
		turbulenceEquationData->createIntParameter("Max Sweep", 30);
		turbulenceEquationData->createDoubleParameter("Iter Sol Tolerence", 1e-8);
		turbulenceEquationData->createDoubleParameter("Relax Coeff", 0.5);

		Core::FITKParameter* speciesEquationData = _solution->getAdditionalSpeciesEquationData();
		speciesEquationData->createComboxParameter("Solver Method", SolutionMethod, 0);
		speciesEquationData->createComboxParameter("Preconditioner", PreconditionerMethod, 0);
		speciesEquationData->createIntParameter("Max Sweep", 30);
		speciesEquationData->createDoubleParameter("Iter Sol Tolerence", 1e-8);
		speciesEquationData->createDoubleParameter("Relax Coeff", 0.5);

		Core::FITKParameter* relaxationData = _solution->getAdditionalRelaxationData();
		relaxationData->createDoubleParameter("Face Flux Relaxation", 0.5);
		relaxationData->createBoolParameter("Gravity Viscous Source", false);
		relaxationData->createBoolParameter("Body Force", false);
		relaxationData->createBoolParameter("Boussinesq Model", false);
		relaxationData->createDoubleParameter("Gravity X", 0.0);
		relaxationData->createDoubleParameter("Gravity Y", -9.81);
		relaxationData->createDoubleParameter("Gravity Z", 0.0);
		relaxationData->createDoubleParameter("Thermal Expansion", 3.44827e-3);

		Core::FITKParameter* pressurePossionEquationData = _solution->getAdditionalPressurePossionEquation();
		pressurePossionEquationData->createComboxParameter("Solver Method", SolutionMethod, 3);
		pressurePossionEquationData->createComboxParameter("Preconditioner", PreconditionerMethod, 1);
		pressurePossionEquationData->createIntParameter("Max Sweep", 30);
		pressurePossionEquationData->createDoubleParameter("Iter Sol Tolerence", 1e-8);
		pressurePossionEquationData->createDoubleParameter("Relax Coeff", 0.3);
	}

	void FITKPHengLEISolverCompressibleSIMPLE::initModels()
	{
		if (!_models) return;
		// 添加EnergyModel参数
		FITKPHengLEIEnergyModel* energyModel = new FITKPHengLEIEnergyModel;
		Core::FITKParameter* energyPara = energyModel->getAdditionalData();
		energyPara->createBoolParameter("Enable Energy Equations", true);
		energyPara->createDoubleParameter("T", 273.15);
		energyPara->createDoubleParameter("CPg", 1007.0);
		energyPara->createDoubleParameter("K", 0.0454);
		energyPara->createDoubleParameter("kappa", 1.4);
		energyPara->createDoubleParameter("refT", 273);
		_models->setEnergyModel(energyModel);

		// 添加SpeciesModel参数
		FITKPHengLEISpeciesModel* speciesModel = new FITKPHengLEISpeciesModel;
		Core::FITKParameter* speciesPara = speciesModel->getAdditionalData();
		speciesPara->createBoolParameter("Enable Solve Species", true);
		speciesPara->createIntParameter("Solve Species Number", 1);
		_models->setSpeciesModel(speciesModel);
		//TODO 添加SpeciesModel类型及初始值

		// 添加ViscousModel参数
		FITKPHengLEIViscousModel* viscousModel = new FITKPHengLEIViscousModel;
		viscousModel->setViscousEnumType(FITKPHengLEIAbsSolver::Lamilar);
		_models->setViscousModel(viscousModel);


	}

}



