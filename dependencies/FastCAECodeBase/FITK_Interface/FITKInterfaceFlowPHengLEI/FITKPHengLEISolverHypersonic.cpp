/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEISolverHypersonic.h"
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
#include "FITKPHengLEISolution.h"
#include "FITKPHengLEIInitialCondition.h"
#include "FITKPHengLEIModels.h"
#include "FITKPHengLEIEnergyModel.h"
#include "FITKPHengLEISpeciesModel.h"

namespace Interface
{

    FITKPHengLEIHypersonicSolver::FITKPHengLEIHypersonicSolver()
    {
		this->setDataObjectName("Hypersonic");

        this->init();
    }

    FITKPHengLEIAbsSolver::PHengLEISolverType FITKPHengLEIHypersonicSolver::getSolverType()
    {
        //亚音速类型
        return  FITKPHengLEIAbsSolver::PHengLEISolverType::PLHypersonic;
    }

    void FITKPHengLEIHypersonicSolver::initBoundaryTypes()
    {
		FITKPHengLEIBoundary* boundaryFarfield = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBFarField);
		boundaryFarfield->setBoundaryType(new FarfieldNone);
		_boundarys->appendDataObj(boundaryFarfield);

		FITKPHengLEIBoundary* boundaryWall = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBWall);
		boundaryWall->setBoundaryType(new WallNone);
		_boundarys->appendDataObj(boundaryWall);

		FITKPHengLEIBoundary* boundaryInFLow = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBInFlow);
		boundaryInFLow->setBoundaryType(new InflowNone);
		_boundarys->appendDataObj(boundaryInFLow);

		FITKPHengLEIBoundary* boundaryPressureInlet = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBPressureInlet);
		boundaryPressureInlet->setBoundaryType(new PressureInlet);
		_boundarys->appendDataObj(boundaryPressureInlet);

		FITKPHengLEIBoundary* boundaryPressureOutlet = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBPressureOutlet);
		boundaryPressureOutlet->setBoundaryType(new PressureOutlet);
		_boundarys->appendDataObj(boundaryPressureOutlet);

		FITKPHengLEIBoundary* boundaryMassFlowInlet = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBMassFlowInlet);
		boundaryMassFlowInlet->setBoundaryType(new MassFlowInlet);
		_boundarys->appendDataObj(boundaryMassFlowInlet);

		FITKPHengLEIBoundary* boundaryMassFlowOutlet = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBMassFlowOutlet);
		boundaryMassFlowOutlet->setBoundaryType(new MassFlowOutlet);
		_boundarys->appendDataObj(boundaryMassFlowOutlet);

		FITKPHengLEIBoundary* boundarySYMMETRY = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PHBSYMMETRY);
		boundarySYMMETRY->setBoundaryType(new SYMMETRY);
		_boundarys->appendDataObj(boundarySYMMETRY);

		FITKPHengLEIBoundary* boundaryOutFlow = new FITKPHengLEIBoundary(FITKPHengLEIBoundary::PhBOutFlow);
		boundaryOutFlow->setBoundaryType(new OutFLow);
		_boundarys->appendDataObj(boundaryOutFlow);
    }

    void FITKPHengLEIHypersonicSolver::initBasicInfo()
    {
		if (!_generalControl) return;

		Core::FITKParameter* addData = _generalControl->getGeneralControlAdditionalData();
		if (!addData)return;
		addData->createDoubleParameter("maxSimuStep", 20000);
		addData->createDoubleParameter("intervalStepFlow", 5000);
		addData->createDoubleParameter("intervalStepPlot", 5000);
		addData->createDoubleParameter("intervalStepForce", 500);
		addData->createDoubleParameter("intervalStepRes", 100);

		_generalControl->setGeneralControlAdditionalData(addData);
    }

    void FITKPHengLEIHypersonicSolver::initInflow()
    {
		if (!_inFlow) return;
		_inFlow->setInFlowType(new FITKPHengLEIInFlowTypeNondimensional(this->getSolverType()));

		Core::FITKParameter* addData = _inFlow->getInFlowAdditionalData();
		if (!addData) return;
		addData->createDoubleParameter("RefMachNumber", 7.00);
		addData->createDoubleParameter("Attacked", 0.00);
		addData->createDoubleParameter("Angle Slide", 0.00);

		// Todo:wallTemperature 参数通过checkbox控件实现是否启动该参数
		addData->createDoubleParameter("Wall Temperature", -1.0);

		_inFlow->setInFlowAdditionalData(addData);

    }

    void FITKPHengLEIHypersonicSolver::initGridParam()
    {
		if (!_meshPara) return;
		Core::FITKParameter* addData = _meshPara->getAdditionalMeshData();
		if (!addData) return;
		addData->createDoubleParameter("Grid Scale Factor", 1.0);

		//addData->createDoubleParameter("Knudsen Length", 1.0);

		Core::FITKParameter* addForceReference = _meshPara->getAdditionalForceReference();
		if (!addForceReference) return;
		addForceReference->createDoubleParameter("Length", 1.0);
		addForceReference->createDoubleParameter("Span Wise", 1.0);
		addForceReference->createDoubleParameter("Area", 1.0);

		Core::FITKParameter* addReferencePoint = _meshPara->getAdditionalReferencePoint();
		if (!addReferencePoint) return;
		addReferencePoint->createDoubleParameter("TorqueRefx", 0.0);
		addReferencePoint->createDoubleParameter("TOrqueRefy", 0.0);
		addReferencePoint->createDoubleParameter("TorqueRefz", 0.0);
    }

    void FITKPHengLEIHypersonicSolver::initViscous()
    {
		if (!_viscousModel) return;
		_viscousModel->setViscousType(new Turbulent1eq);
		Core::FITKParameter* addData = _viscousModel->getAdditionalVicousModelData();
		if (!addData) return;

		QStringList desType = { "RANS","DES","DDES","IDDES" };
		addData->createComboxParameter("DES Type",desType, 0);
    }

    void FITKPHengLEIHypersonicSolver::initDiscrete()
    {
		if (!_discretisation) return;
		_discretisation->setSpatialDiscretisation(new FITKPHengLEISpatialUnStruct(this->getSolverType()));

		Core::FITKParameter* addData = _discretisation->getTemporalAdditionalData();
		if (!addData) return;

		QStringList flowState = {"Steady State","Unsteady State"};
		addData->createComboxParameter("Flow State", flowState, 0);
		QStringList CFLMethod = {"Linear change","Exponential change"};
		addData->createComboxParameter("CFL Method", CFLMethod, 1);
		addData->createDoubleParameter("CFL Start", 1.0);
		addData->createDoubleParameter("CFL End", 5.0);
		addData->createIntParameter("CFL Vary Step", 500);
		addData->createDoubleParameter("ktmax", 1.0e9,1.0e20);
		addData->createIntParameter("LU-SGS Sub-Iterations", 1, 3, 1);

    }

    void FITKPHengLEIHypersonicSolver::initMultiGridParam()
    {
		if (_multiGRID) delete _multiGRID;
		_multiGRID = nullptr;
		return;
    }

	void FITKPHengLEIHypersonicSolver::initOutputSetting()
	{
		if (!_outputSetting) return;
		Core::FITKParameter* basicFlowPara = _outputSetting->getBasicFlowData();

		basicFlowPara->createBoolParameter("Density", true)->setData(0);
		basicFlowPara->createBoolParameter("U", true)->setData(1);
		basicFlowPara->createBoolParameter("V", true)->setData(2);
		basicFlowPara->createBoolParameter("W", true)->setData(3);
		basicFlowPara->createBoolParameter("Pressure", true)->setData(4);
		basicFlowPara->createBoolParameter("Temperature", true)->setData(5);
		basicFlowPara->createBoolParameter("Mach", true)->setData(6);

		Core::FITKParameter* viscosityData = _outputSetting->getViscosityData();
		viscosityData->createBoolParameter("Laminar", false)->setData(7);
		viscosityData->createBoolParameter("Turbulent", false)->setData(8);

		Core:: FITKParameter* vorticityData = _outputSetting->getVorticityData();
		vorticityData->createBoolParameter("Vorticity_X", false)->setData(9);
		vorticityData->createBoolParameter("Vorticity_Y", false)->setData(10);
		vorticityData->createBoolParameter("Vorticity_Z", false)->setData(11);
		vorticityData->createBoolParameter("Vorticity_Magnitude", false)->setData(12);

		Core::FITKParameter* derivedFlowData = _outputSetting->getDerivedFlowData();
		derivedFlowData->createBoolParameter("Strain Rate", false)->setData(13);
		derivedFlowData->createBoolParameter("Q_criteria", false)->setData(14);
		derivedFlowData->createBoolParameter("Cp", true)->setData(15);
		derivedFlowData->createBoolParameter("Time Step", false)->setData(16);
		derivedFlowData->createBoolParameter("Volume", false)->setData(17);

		Core::FITKParameter* turbulenceModelData = _outputSetting->getTurbulenceModelData();
		turbulenceModelData->createBoolParameter("TKE", false)->setData(18);
		turbulenceModelData->createBoolParameter("Dissipation Rate", false)->setData(19);
		turbulenceModelData->createBoolParameter("SSTF1", false)->setData(20);
		turbulenceModelData->createBoolParameter("SSTF2", false)->setData(21);

		Core::FITKParameter* nonEquilibriumData = _outputSetting->getNonEquilibriumData();
		nonEquilibriumData->createBoolParameter("vibration temperature", false)->setData(33);
		nonEquilibriumData->createBoolParameter(" electron temperature", false)->setData(34);
		nonEquilibriumData->createBoolParameter("vibrational energy", false)->setData(35);
		nonEquilibriumData->createBoolParameter("electric energy", false)->setData(36);

		Core::FITKParameter* dimensionalData = _outputSetting->getDimensionalData();
		dimensionalData->createBoolParameter("number density of electron", false)->setData(37);
		dimensionalData->createBoolParameter("dimensioanl density", false)->setData(38);
		dimensionalData->createBoolParameter("dimensioanl pressure", false)->setData(39);
		dimensionalData->createBoolParameter("dimensioanl temperature", false)->setData(40);

		//Core::FITKParameter* dimensionlessData = _outputSetting->getDimensionlessData();
		//dimensionlessData->createBoolParameter("Knudsen number", false)->setData(1);
		//dimensionlessData->createBoolParameter("Damkohler number", false)->setData(1);
		//dimensionlessData->createBoolParameter("vibrational nonequilibrium number", false)->setData(1);


	}

	void FITKPHengLEIHypersonicSolver::initFlow()
	{
		if (_flow) delete _flow;
		_flow = nullptr;
		return;
	}

	void FITKPHengLEIHypersonicSolver::initCalculation()
	{
		if (!_calculation) return;
		Core::FITKParameter* addData = _calculation->getAdditionalConvergenceData();
		if (!addData) return;
		addData->createIntParameter("Max Intervals Step", 15000);

		Core::FITKParameter* addSavingIntervals = _calculation->getAdditionalSavingIntervalsData();
		if (!addSavingIntervals) return;
		addSavingIntervals->createIntParameter("Flow Step", 2000);
		addSavingIntervals->createIntParameter("Plot Step", 2000);
		addSavingIntervals->createIntParameter("Force Step", 50);
		addSavingIntervals->createIntParameter("Res Step", 20);

		Core::FITKParameter* addParallelData = _calculation->getAdditionalParallelData();
		if (!addParallelData) return;
		addParallelData->createIntParameter("Processes", 4,4,1);
	}

	void FITKPHengLEIHypersonicSolver::initInitialCondition()
	{
		if (_initialCondition) delete _initialCondition;
		_initialCondition = nullptr;
		return;
	}

	void FITKPHengLEIHypersonicSolver::initSolution()
	{
		if (_solution) delete _solution;
		_solution = nullptr;
		return;
	}

	void FITKPHengLEIHypersonicSolver::initModels()
	{
		// 添加ViscousModel参数
		FITKPHengLEIViscousModel* viscousModel = new FITKPHengLEIViscousModel;
		viscousModel->setViscousEnumType(FITKPHengLEIAbsSolver::Lamilar);
		_models->setViscousModel(viscousModel);

		_models->setEnergyModel(nullptr);
		_models->setSpeciesModel(nullptr);
	}

}



