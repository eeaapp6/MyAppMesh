/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEISolverSubsonic.h"
#include "FITKPHengLEIBoundary.h"
#include "FITKPHengLEIGeneralControl.h"
#include "FITKPHengLEIInFlow.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITKPHengLEIMeshPara.h"
#include "FITKPHengLEIViscousModel.h"
#include "FITKPHengLEIMultiGrid.h"
#include "FITKPHengLEIDiscretisation.h"
#include "FITKPHengLEIOutputSetting.h"
#include "FITKPHengLEIFlow.h"
#include "FITKPHengLEICalculation.h"
#include "FITKPHengLEIModels.h"
#include "FITKPHengLEISolution.h"
#include "FITKPHengLEIInitialCondition.h"


namespace Interface
{

    FITKPHengLEIAbsSolver::FITKPHengLEIAbsSolver()
    {
        //创建边界管理器实例
        _boundarys = new FITKPHengLEIBoundaryManager;
		// 创建通用控制参数实例
		_generalControl = new FITKPHengLEIGeneralControl;
		// 创建来流参数实例
		_inFlow = new FITKPHengLEIInFlow;
		// 创建网格相关参数实例
		_meshPara = new FITKPHengLEIMeshPara;
		// 创建粘性模型参数实例
		_viscousModel = new FITKPHengLEIViscousModel;
		// 创建多重网格参数实例
		_multiGRID = new FITKPHengLEIMultiGrid;
		// 创建离散参数实例
		_discretisation = new FITKPHengLEIDiscretisation;
		// 创建计算输出参数实例
		_outputSetting = new FITKPHengLEIOutputSetting;
		// 创建来流参数实例
		_flow = new FITKPHengLEIFlow;
		// 创建求解计算参数实例 
		_calculation = new FITKPHengLEICalculation;
		// 创建Model参数实例
		_models = new FITKPHengLEIModels;
		// 创建Solution参数实例
		_solution = new FITKPHengLEISolution;
		// 创建InitialCondition参数实例
		_initialCondition = new FITKPHengLEIInitialCondition;

    }

    FITKPHengLEIAbsSolver::~FITKPHengLEIAbsSolver()
    {
		if (_boundarys)
		{
			delete _boundarys;
			_boundarys = nullptr;
		}

		if (_generalControl)
		{
			delete _generalControl;
			_generalControl = nullptr;
		}

		if (_inFlow)
		{
			delete _inFlow;
			_inFlow = nullptr;
		}

		if (_meshPara)
		{
			delete _meshPara;
			_meshPara = nullptr;
		}

		if (_viscousModel)
		{
			delete _viscousModel;
			_viscousModel = nullptr;
		}

		if (_multiGRID)
		{
			delete _multiGRID;
			_multiGRID = nullptr;
		}

		if (_discretisation)
		{
			delete _discretisation;
			_discretisation = nullptr;
		}

		if (_outputSetting)
		{
			delete _outputSetting;
			_outputSetting = nullptr;
		}

		if (_flow)
		{
			delete _flow;
			_flow = nullptr;
		}

		if (_calculation)
		{
			delete _calculation;
			_calculation = nullptr;
		}

		if (_initialCondition)
		{
			delete _initialCondition;
			_initialCondition = nullptr;
		}

		if (_solution)
		{
			delete _solution;
			_solution = nullptr;
		}

		if (_models)
		{
			delete _models;
			_models = nullptr;
		}
    }

    FITKPHengLEIAbsSolver::PHengLEISolverType FITKPHengLEIAbsSolver::getSolverType()
    {
        //抽象类返回当前求解器类型
        return FITKPHengLEIAbsSolver::PHengLEISolverType::PLSolverNone;
    }

    void FITKPHengLEIAbsSolver::init()
    {
        //执行子类重写的函数，对数据对象初始化 
        this->initBoundaryTypes();
        this->initBasicInfo();
        this->initInflow();
        this->initGridParam();
        this->initViscous();
        this->initDiscrete();
        this->initMultiGridParam();
		this->initOutputSetting();
		this->initFlow();
		this->initCalculation();
		this->initInitialCondition();
		this->initSolution();
		this->initModels();
    }

	FITKPHengLEIGeneralControl * FITKPHengLEIAbsSolver::getGeneralControl()
	{
		return _generalControl;
	}

	FITKPHengLEIInFlow * FITKPHengLEIAbsSolver::getInFlow()
	{
		return _inFlow;
	}

	FITKPHengLEIMeshPara * FITKPHengLEIAbsSolver::getMeshPara()
	{
		return _meshPara;
	}

	FITKPHengLEIViscousModel * FITKPHengLEIAbsSolver::getViscousModel()
	{
		return _viscousModel;
	}

	FITKPHengLEIMultiGrid * FITKPHengLEIAbsSolver::getMultiGrid()
	{
		return _multiGRID;
	}

	FITKPHengLEIDiscretisation * FITKPHengLEIAbsSolver::getDiscretisation()
	{
		return _discretisation;
	}

	FITKPHengLEIBoundary* FITKPHengLEIAbsSolver::getBoundary(FITKPHengLEIBoundary::PHengLEIBoundaryType bType)
	{
		return _boundarys->getBoundary(bType);
	}

	FITKPHengLEIOutputSetting * FITKPHengLEIAbsSolver::getOutputSetting()
	{
		return _outputSetting;
	}
	FITKPHengLEIFlow* FITKPHengLEIAbsSolver::getFlow()
	{
		return _flow;
	}

	FITKPHengLEICalculation * FITKPHengLEIAbsSolver::getCalculation()
	{
		return _calculation;
	}

	FITKPHengLEISolution * FITKPHengLEIAbsSolver::getSolution()
	{
		return _solution;
	}


	Interface::FITKPHengLEIInitialCondition* FITKPHengLEIAbsSolver::getInitialCondition()
	{
		return _initialCondition;
	}

	FITKPHengLEIModels* FITKPHengLEIAbsSolver::getModels()
	{
		return _models;
	}

	void FITKPHengLEIAbsSolver::initBoundaryTypes()
    {

    }

    void FITKPHengLEIAbsSolver::initInflow()
    {

    }

    void FITKPHengLEIAbsSolver::initGridParam()
    {

    }

    void FITKPHengLEIAbsSolver::initViscous()
    {

    }

    void FITKPHengLEIAbsSolver::initDiscrete()
    {

    }

    void FITKPHengLEIAbsSolver::initMultiGridParam()
    {

    }

	void FITKPHengLEIAbsSolver::initOutputSetting()
	{

	}

	void FITKPHengLEIAbsSolver::initFlow()
	{

	}

}


