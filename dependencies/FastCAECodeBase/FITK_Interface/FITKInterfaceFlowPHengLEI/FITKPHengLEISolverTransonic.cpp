/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEISolverTransonic.h"
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
#include "FITKPHengLEICalculation.h"
#include "FITKPHengLEIViscousType.h"
#include "FITKPHengLEISolution.h"
#include "FITKPHengLEIInitialCondition.h"
#include "FITKPHengLEIModels.h"
#include "FITKPHengLEIEnergyModel.h"
#include "FITKPHengLEISpeciesModel.h"

namespace Interface
{

    FITKPHengLEITransonicSolver::FITKPHengLEITransonicSolver()
    {
        this->init();
    }

    FITKPHengLEIAbsSolver::PHengLEISolverType FITKPHengLEITransonicSolver::getSolverType()
    {
        //跨音速类型
        return FITKPHengLEIAbsSolver::PHengLEISolverType::PLTransonic;
    }

    void FITKPHengLEITransonicSolver::initBoundaryTypes()
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

    void FITKPHengLEITransonicSolver::initBasicInfo()
    {
		if (!_generalControl) return;

		Core::FITKParameter* addData = _generalControl->getGeneralControlAdditionalData();
		if (!addData)return;
		addData->createDoubleParameter("Interval Step Flow", 1000);
		addData->createDoubleParameter("Max Simu Step", 10000);
		addData->createDoubleParameter("Interval Step Plot", 1000);
		addData->createDoubleParameter("Interval Step Force", 100);
		addData->createDoubleParameter("Interval Step Res", 10);

		_generalControl->setGeneralControlAdditionalData(addData);
    }

    void FITKPHengLEITransonicSolver::initInflow()
    {
		if (!_inFlow) return;
		_inFlow->setInFlowType(new FITKPHengLEIInFlowTypeNondimensional(this->getSolverType()));

		Core::FITKParameter* addData = _inFlow->getInFlowAdditionalData();
		if (!addData) return;
		addData->createDoubleParameter("RefMachNumber", 0.73);
		addData->createDoubleParameter("Attacked", 2.79);
		addData->createDoubleParameter("Angle Slide", 0.00);

		_inFlow->setInFlowAdditionalData(addData);

    }

    void FITKPHengLEITransonicSolver::initGridParam()
    {
        //todo
		if (!_meshPara) return;
		Core::FITKParameter* addData = _meshPara->getAdditionalMeshData();
		if (!addData) return;
		addData->createDoubleParameter("Grid Scale Factor", 1.0);

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

		_meshPara->setAdditionalMeshData(addData);
		_meshPara->setAdditionalForceReference(addForceReference);
		_meshPara->setAdditionalReferencePoint(addReferencePoint);
    }

    void FITKPHengLEITransonicSolver::initViscous()
    {
		if (!_viscousModel) return;
		_viscousModel->setViscousType(new Turbulent1eq);
		if (!_viscousModel) return;
		Core::FITKParameter* addData = _viscousModel->getAdditionalVicousModelData();
		if (!addData) return;

		QStringList desType = { "RANS","DES","DDES","IDDES" };
		addData->createComboxParameter("DES Type", desType, 0);
    }

    void FITKPHengLEITransonicSolver::initDiscrete()
    {
		if (!_discretisation) return;
		_discretisation->setSpatialDiscretisation(new FITKPHengLEISpatialUnStruct(this->getSolverType()));

		Core::FITKParameter* addData = _discretisation->getTemporalAdditionalData();
		if (!addData) return;
		QStringList flowState = { "Steady State","Unsteady State" };
		addData->createComboxParameter("Flow State", flowState, 0);
		QStringList CFLMethod = { "Linear change","Exponential change" };
		addData->createComboxParameter("CFL Method", CFLMethod, 1);
		addData->createDoubleParameter("CFL Start", 3.0);
		addData->createDoubleParameter("CFL End", 30.0);
		addData->createIntParameter("CFL Vary Step", 500);
		addData->createDoubleParameter("ktmax", 1.0e10, 1.0e11);
		addData->createIntParameter("LU-SGS Sub-Iterations",3,3,1);
    }

    void FITKPHengLEITransonicSolver::initMultiGridParam()
    {
        //todo
		if (!_multiGRID) return;
		Core::FITKParameter* addData = _multiGRID->getAdditionalMultiGridData();
		addData->createDoubleParameter("Multi-Grid Level", 1);
		addData->createDoubleParameter("Flow Init Step",100);

		_multiGRID->setAdditionalMultiGridData(addData);
    }



	void FITKPHengLEITransonicSolver::initOutputSetting()
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

		Core::FITKParameter* vorticityData = _outputSetting->getVorticityData();
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
		turbulenceModelData->createBoolParameter("iblank", false)->setData(81);

	

		//Core::FITKParameter* nonEquilibriumData = _outputSetting->getNonEquilibriumData();
		//nonEquilibriumData->createBoolParameter("vibration temperature", false)->setData(33);
		//nonEquilibriumData->createBoolParameter(" electron temperature", false)->setData(34);
		//nonEquilibriumData->createBoolParameter("vibrational energy", false)->setData(35);
		//nonEquilibriumData->createBoolParameter("electric energy", false)->setData(36);

		//Core::FITKParameter* dimensionalData = _outputSetting->getDimensionalData();
		//dimensionalData->createBoolParameter("number density of electron", false)->setData(37);
		//dimensionalData->createBoolParameter("dimensioanl density", false)->setData(38);
		//dimensionalData->createBoolParameter("dimensioanl pressure", false)->setData(39);
		//dimensionalData->createBoolParameter("dimensioanl temperature", false)->setData(40);

		//Core::FITKParameter* dimensionlessData = _outputSetting->getDimensionlessData();
		//dimensionlessData->createBoolParameter("Knudsen number", false)->setData(1);
		//dimensionlessData->createBoolParameter("Damkohler number", false)->setData(1);
		//dimensionlessData->createBoolParameter("vibrational nonequilibrium number", false)->setData(1);


	}

	void FITKPHengLEITransonicSolver::initFlow()
	{
		if (_flow) delete _flow;
		_flow = nullptr;
		return;
	}
	void FITKPHengLEITransonicSolver::initCalculation()
	{
		if (!_calculation) return;
		Core::FITKParameter* addData = _calculation->getAdditionalConvergenceData();
		if (!addData) return;
		addData->createIntParameter("Max Intervals Step", 10000);

		Core::FITKParameter* addSavingIntervals = _calculation->getAdditionalSavingIntervalsData();
		if (!addSavingIntervals) return;
		addSavingIntervals->createIntParameter("Flow Step", 20);
		addSavingIntervals->createIntParameter("Plot Step", 20);
		addSavingIntervals->createIntParameter("Force Step", 20);
		addSavingIntervals->createIntParameter("Res Step", 20);

		Core::FITKParameter* addParallelData = _calculation->getAdditionalParallelData();
		if (!addParallelData) return;
		addParallelData->createIntParameter("Processes", 4, 4, 1);
	}
	void FITKPHengLEITransonicSolver::initInitialCondition()
	{
		if (_initialCondition) delete _initialCondition;
		_initialCondition = nullptr;
	}
	void FITKPHengLEITransonicSolver::initSolution()
	{
		if (_solution) delete _solution;
		_solution = nullptr;
	}
	void FITKPHengLEITransonicSolver::initModels()
	{
		// 添加ViscousModel参数
		FITKPHengLEIViscousModel* viscousModel = new FITKPHengLEIViscousModel;
		viscousModel->setViscousEnumType(FITKPHengLEIAbsSolver::Lamilar);
		_models->setViscousModel(viscousModel);

		_models->setEnergyModel(nullptr);
		_models->setSpeciesModel(nullptr);
	}
}


