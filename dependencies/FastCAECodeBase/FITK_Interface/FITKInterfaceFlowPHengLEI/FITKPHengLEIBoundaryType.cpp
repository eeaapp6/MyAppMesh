/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIBoundaryType.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITKPHengLEIAbsSolver.h"

#include <QList>

namespace Interface
{
    FITKPHengLEIBoundaryType::FITKPHengLEIBoundaryType()
	{
        //初始化
		_boundaryTypeParams = new Core::FITKParameter;
    }

    FITKPHengLEIBoundaryType::~FITKPHengLEIBoundaryType()
    {
        //销毁数据
        if (_boundaryTypeParams) delete _boundaryTypeParams;
    }

    Core::FITKParameter* FITKPHengLEIBoundaryType::getBoundaryTypeParams()
    {
        return _boundaryTypeParams;
    }


    void FITKPHengLEIBoundaryType::init()
    {
        //初始化
        this->initBoundaryTypeParam();
    }

    void FITKPHengLEIBoundaryType::initBoundaryTypeParam()
    {

    }


	PressureInlet::PressureInlet()
	{
		//初始化
		this->init();
		this->setDataObjectName("PressureInlet");
	}

	void PressureInlet::initBoundaryTypeParam()
	{
		//设置参数
		//   int bcType = 52; 不由用户界面指定，在后端默认配置该参数
		_boundaryTypeParams->createDoubleParameter("Total Pressure", 118309.784);
		_boundaryTypeParams->createDoubleParameter("Total Temperature", 302.4);

		QList<double> directionInlet = { 1,0,0 };
		_boundaryTypeParams->createDoubleListParameter("Direction Inlet",directionInlet);
	}

	PressureOutlet::PressureOutlet()
	{
		//初始化
		this->init();
		this->setDataObjectName("PressureOutlet");
	}

	void PressureOutlet::initBoundaryTypeParam()
	{
		//   int bcType = 62; 不由用户界面指定，在后端默认配置该参数
		_boundaryTypeParams->createDoubleParameter("Static Pressure",115056.0);

	}

	MassFlowInlet::MassFlowInlet()
	{
		//初始化
		this->init();
		this->setDataObjectName("MassFlowInlet");
	}

	void MassFlowInlet::initBoundaryTypeParam()
	{
		//   int bcType = 53; 不由用户界面指定，在后端默认配置该参数
		_boundaryTypeParams->createDoubleParameter("Mass Flow", 0.70666);
		_boundaryTypeParams->createDoubleParameter("Total Temperature", 216.6666667);

		QList<double> directionInlet = { 1,0,0 };
		_boundaryTypeParams->createDoubleListParameter("Direction Inlet", directionInlet);
	}

	MassFlowOutlet::MassFlowOutlet()
	{
		//初始化
		this->init();
		this->setDataObjectName("MassFlowOutlet");
	}

	void MassFlowOutlet::initBoundaryTypeParam()
	{
		//   int bcType = 63; 不由用户界面指定，在后端默认配置该参数
		_boundaryTypeParams->createDoubleParameter("Mass Flow", 0.500);
	}
	SYMMETRY::SYMMETRY()
	{
		//初始化
		this->init();
		this->setDataObjectName("SYMMETRY");
	}
	void SYMMETRY::initBoundaryTypeParam()
	{
	}
	OutFLow::OutFLow()
	{
		//初始化
		this->init();
		this->setDataObjectName("OutFlow");
	}
	void OutFLow::initBoundaryTypeParam()
	{
		_boundaryTypeParams->createDoubleParameter("Initial P", 41.45637);
	}
}



