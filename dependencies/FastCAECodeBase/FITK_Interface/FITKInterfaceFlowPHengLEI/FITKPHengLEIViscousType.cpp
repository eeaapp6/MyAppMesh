/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIViscousType.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITKPHengLEIAbsSolver.h"

namespace Interface
{
	FITKPHengLEIViscousType::FITKPHengLEIViscousType()
	{
		//初始化
		_viscousTypeParams = new Core::FITKParameter;
	}

	FITKPHengLEIViscousType::~FITKPHengLEIViscousType()
	{
		//销毁数据
		if (_viscousTypeParams) delete _viscousTypeParams;
	}

	Core::FITKParameter* FITKPHengLEIViscousType::getViscousTypeParams()
	{
		//获取粘性模型参数
		return _viscousTypeParams;
	}


	void FITKPHengLEIViscousType::init()
	{
		//初始化
		this->initViscousTypeParam();
	}

	void FITKPHengLEIViscousType::initViscousTypeParam()
	{
		//初始化粘性类型参数
	}
}

namespace Interface
{
	Turbulent1eq::Turbulent1eq()
    {
        //初始化
        this->init();
        this->setDataObjectName("Turbulent1eq");
    }


    void Turbulent1eq::initViscousTypeParam()
    {
		QStringList turbulentModel = {"1eq-sa"};
		_viscousTypeParams->createComboxParameter("1eq turbulent model", turbulentModel, 0);
    }
}

namespace Interface
{
	Turbulent2eq::Turbulent2eq()
	{
		//初始化
		this->init();
		this->setDataObjectName("Turbulent2eq");
	}


	void Turbulent2eq::initViscousTypeParam()
	{
		QStringList turbulentModel = { "2eq-kw-menter-sst","2eq-kw-menter-bsl","2eq-kw-wilcox-1988","2eq-kw-wilcox-1998","2eq-kw-kok-tnt","2eq-kw-wilcox-2006"};
		_viscousTypeParams->createComboxParameter("2eq turbulent model", turbulentModel, 0);
	}
}

namespace Interface
{
	KEpsilon::KEpsilon()
	{
		//初始化
		this->init();
		this->setDataObjectName("KEpsilon");
	}


	void KEpsilon::initViscousTypeParam()
	{
		_viscousTypeParams->createDoubleParameter("Inital Kinetic", 0.0348844);
		_viscousTypeParams->createDoubleParameter("Inital Epsilon", 10.89987);
	}
}

namespace Interface
{
	SpalartAllmaras::SpalartAllmaras()
	{
		//初始化
		this->init();
		this->setDataObjectName("SpalartAllmaras");
	}


	void SpalartAllmaras::initViscousTypeParam()
	{
		_viscousTypeParams->createDoubleParameter("Inital Kinetic", 0.0348844);
		_viscousTypeParams->createDoubleParameter("Inital Epsilon", 10.89987);
	}
}


