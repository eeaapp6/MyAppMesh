/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIData.h"
#include "FITKPHengLEIMeshProcess.h"
#include "FITKPHengLEISolverTransonic.h"
#include "FITKPHengLEIBoundary.h"
#include "FITKPHengLEIGeneral.h"
#include "FITKPHengLEIGeneralControl.h"
#include "FITKPHengLEIInFlow.h"
#include "FITKPHengLEIMeshPara.h"
#include "FITKPHengLEIViscousModel.h"
#include "FITKPHengLEIMultiGrid.h"
#include "FITKPHengLEIDiscretisation.h"
#include "FITKPHengLEIOutputSetting.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITKPHengLEIerDriver.h"
#include "FITKPHengLEIFlow.h"
#include "FITKPHengLEICalculation.h"
#include "FITKPHengLEIResidaulProcess.h"
#include "FITKPHengLEIModels.h"
#include "FITKPHengLEISolution.h"
#include "FITKPHengLEICouplingControl.h"

namespace Interface
{
    FITKPHengLEIData::FITKPHengLEIData()
    {
        _driver = new FITKPHengLEIerDriver;
		_meshProcess = new FITKPHengLEIMeshProcess;
		_general = new FITKPHengLEIGeneral;
		_residaulProcess = new FITKPHengLEIResidaulProcess;
		_models = new FITKPHengLEIModels;
		_couplingData = new FITKPHengLEICouplingControl;
    }

    FITKPHengLEIData::~FITKPHengLEIData()
    {
        //释放内存
        if (_driver) delete _driver;
		if (_meshProcess) delete _meshProcess;
        if (_solver) delete _solver;
		if (_general) delete _general;
		if (_generalControl) delete _generalControl;
		if (_inFlow) delete _inFlow;
		if (_meshPara) delete _meshPara;
		if (_multiGrid) delete _multiGrid;
		if (_viscousModel) delete _viscousModel;
		if (_boundaryManager) delete _boundaryManager;
		if (_discretisation) delete _discretisation;
		if (_flow) delete _flow;
		if (_calculation) delete _calculation;
		if (_residaulProcess) delete _residaulProcess;
		if(_models) delete _models;
		if (_couplingData) delete _couplingData;
    }

    FITKPHengLEIData* FITKPHengLEIData::GetPHengLEIDataFromFrame()
    {
        //从全局数据和获取
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (globalData == nullptr) return nullptr;
        return  globalData->getPhysicsData<FITKPHengLEIData>();
    }

	FITKPHengLEIMeshProcess* FITKPHengLEIData::getMeshProcess()
	{
		return _meshProcess;
	}
    FITKPHengLEIAbsSolver* FITKPHengLEIData::getSolver()
    {
        return _solver;
    }
	FITKPHengLEICouplingControl* FITKPHengLEIData::getCouplingData()
	{
		return _couplingData;
	}

    Interface::FITKPHengLEIBoundaryManager* FITKPHengLEIData::getBoundaryManager()
    {
        return _boundaryManager;
    }

	FITKPHengLEIGeneral * FITKPHengLEIData::getGeneral()
	{
		return _general;
	}

	FITKPHengLEIGeneralControl * FITKPHengLEIData::getGeneralControl()
	{
		return _generalControl;
	}

	FITKPHengLEIInFlow * FITKPHengLEIData::getInFlow()
	{
		return _inFlow;
	}

	FITKPHengLEIMeshPara * FITKPHengLEIData::getMeshPara()
	{
		return _meshPara;
	}

	FITKPHengLEIViscousModel * FITKPHengLEIData::getViscousModel()
	{

		return _viscousModel;
	}

	FITKPHengLEIMultiGrid * FITKPHengLEIData::getMultiGrid()
	{
		return _multiGrid;
	}

	FITKPHengLEIDiscretisation * FITKPHengLEIData::getDiscretisation()
	{
		return _discretisation;
	}

	FITKPHengLEIOutputSetting * FITKPHengLEIData::getOutputSetting()
	{
		return _outputSetting;
	}

	FITKPHengLEIerDriver * FITKPHengLEIData::getPHengLEIDirver()
	{
		return _driver;
	}

	FITKPHengLEIFlow * FITKPHengLEIData::getFlow()
	{
		return _flow;
	}
	FITKPHengLEICalculation * FITKPHengLEIData::getCalculation()
	{
		return _calculation;
	}
	FITKPHengLEIResidaulProcess * FITKPHengLEIData::getResidaulProcess()
	{
		return _residaulProcess;
	}
	void FITKPHengLEIData::setSolver(FITKPHengLEIAbsSolver * solver)
	{
		if (solver == _solver && _solver) delete _solver;
		_solver = solver;
	}

	void FITKPHengLEIData::setGeneral(FITKPHengLEIGeneral * general)
	{
		if (general != _general && _general) delete _general;
		_general = general;
	}

	void FITKPHengLEIData::setGeneralControl(FITKPHengLEIGeneralControl * generalControl)
	{
		if (generalControl != _generalControl && _generalControl) delete _generalControl;
		_generalControl = generalControl;
	}

	void FITKPHengLEIData::setInFlow(FITKPHengLEIInFlow * InFlow)
	{
		if (InFlow != _inFlow && _inFlow) delete _inFlow;
		_inFlow = InFlow;
	}

	void FITKPHengLEIData::setMeshPara(FITKPHengLEIMeshPara * MeshPara)
	{
		if (MeshPara != _meshPara && _meshPara) delete _meshPara;
		_meshPara = MeshPara;
	}

	void FITKPHengLEIData::setViscousModel(FITKPHengLEIViscousModel * viscousModel)
	{
		if (viscousModel != _viscousModel && _viscousModel) delete _viscousModel;
		_viscousModel = viscousModel;
	}

	void FITKPHengLEIData::setMultiGrid(FITKPHengLEIMultiGrid * multiGrid)
	{
		if (multiGrid != _multiGrid && _multiGrid) delete _multiGrid;
		_multiGrid = multiGrid;
	}
	void FITKPHengLEIData::setDiscretisation(FITKPHengLEIDiscretisation * discretisation)
	{
		if (_discretisation != discretisation && _discretisation) delete _discretisation;
		_discretisation = discretisation;
	}
	void FITKPHengLEIData::setBoundaryManager(FITKPHengLEIBoundaryManager * boundaryManager)
	{
		if (_boundaryManager != boundaryManager && _boundaryManager) delete _boundaryManager;
		_boundaryManager = boundaryManager;
	}
	void FITKPHengLEIData::setOutputSetting(FITKPHengLEIOutputSetting * outputSetting)
	{
		if (_outputSetting!=outputSetting && _outputSetting) delete _outputSetting;
		_outputSetting = outputSetting;
	}

	void FITKPHengLEIData::setMeshProcess(FITKPHengLEIMeshProcess* meshProcess)
	{
		if (meshProcess != _meshProcess && _meshProcess) delete _meshProcess;
		_meshProcess = meshProcess;
	}

	void FITKPHengLEIData::setFlow(FITKPHengLEIFlow* flow)
	{
		if (flow != _flow && _flow) delete _flow;
		_flow = flow;
	}
	void FITKPHengLEIData::setCalculation(FITKPHengLEICalculation * calculation)
	{
		if (_calculation != calculation && _calculation) delete _calculation;
		_calculation = calculation;
	}
	void FITKPHengLEIData::setResidaulProcess(FITKPHengLEIResidaulProcess * residaulProcess)
	{
		if (_residaulProcess != residaulProcess && _residaulProcess) delete _residaulProcess;
		_residaulProcess = residaulProcess;
	}

	FITKPHengLEIModels* FITKPHengLEIData::getModels()
	{
		return _models;
	}

	FITKPHengLEISolution * FITKPHengLEIData::getSolution()
	{
		return _solution;
	}

	Interface::FITKPHengLEIInitialCondition* FITKPHengLEIData::getInitialCondition()
	{
		return _initialCondition;
	}

	void FITKPHengLEIData::setModels(FITKPHengLEIModels* models)
	{
		if (_models != models && _models) delete _models;
		_models = models;
	}
	void FITKPHengLEIData::setSolution(FITKPHengLEISolution * solution)
	{
		if (_solution != solution && _solution) delete _solution;
		_solution = solution;
	}
	void FITKPHengLEIData::setCouplingData(FITKPHengLEICouplingControl * couplingData)
	{
		if (_couplingData != couplingData && _couplingData) delete _couplingData;
		_couplingData = couplingData;
	}

	void FITKPHengLEIData::setInitialCondition(FITKPHengLEIInitialCondition* initialCondition)
	{
		_initialCondition = initialCondition;
	}

}



