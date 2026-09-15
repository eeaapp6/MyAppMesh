/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIBoundaryInFlowType.h"

Interface::InflowOne::InflowOne()
{
	//初始化
	this->init();
	this->setDataObjectName("InFlow1");
}

void Interface::InflowOne::initBoundaryTypeParam()
{
	//设置参数
	//   int bcType = 5; 不由用户界面指定，在后端默认配置该参数
	//   int inflowParaType = 0; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("RefMachNumber", 0.1);
	_boundaryTypeParams->createDoubleParameter("Attacked", 0);
	_boundaryTypeParams->createDoubleParameter("AngleSlide", 0);
	_boundaryTypeParams->createDoubleParameter("RefReNumber", 2.0e5);
	_boundaryTypeParams->createDoubleParameter("RefDismensionalTemperature", 293);
}

Interface::InflowTwo::InflowTwo()
{
	//初始化
	this->init();
	this->setDataObjectName("InFlow2");
}

void Interface::InflowTwo::initBoundaryTypeParam()
{
	//设置参数
	//   int bcType = 5; 不由用户界面指定，在后端默认配置该参数
	//   int inflowParaType = 1; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("RefMachNumber", 0.1);
	_boundaryTypeParams->createDoubleParameter("Attacked", 0);
	_boundaryTypeParams->createDoubleParameter("AngleSlide", 0);
	_boundaryTypeParams->createDoubleParameter("Height", 0.001);
}

Interface::InflowThree::InflowThree()
{
	//初始化
	this->init();
	this->setDataObjectName("InFlow3");
}

void Interface::InflowThree::initBoundaryTypeParam()
{
	//设置参数
	//   int bcType = 5; 不由用户界面指定，在后端默认配置该参数
	//   int inflowParaType = 4; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("RefDimensionalVelocity", 1000.0);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalDensity", 1.0e3);
	_boundaryTypeParams->createDoubleParameter("Attacked", 0);
	_boundaryTypeParams->createDoubleParameter("AngleSlide", 0);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalTempature", 293);
}

Interface::InflowFour::InflowFour()
{
	//初始化
	this->init();
	this->setDataObjectName("InFlow4");
}

void Interface::InflowFour::initBoundaryTypeParam()
{
	//设置参数
//   int bcType = 5; 不由用户界面指定，在后端默认配置该参数
//   int inflowParaType = 5; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("RefDimensionalVelocity", 1000.0);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalPressure", 1.0e5);
	_boundaryTypeParams->createDoubleParameter("Attacked", 0);
	_boundaryTypeParams->createDoubleParameter("AngleSlide", 0);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalTempature", 293);
}

Interface::InflowFive::InflowFive()
{
	//初始化
	this->init();
	this->setDataObjectName("InFlow5");
}

void Interface::InflowFive::initBoundaryTypeParam()
{
	//设置参数
//   int bcType = 5; 不由用户界面指定，在后端默认配置该参数
//   int inflowParaType = 6; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("RefMachNumber", 0.1);
	_boundaryTypeParams->createDoubleParameter("Attacked", 0.0);
	_boundaryTypeParams->createDoubleParameter("AngleSlide", 0.00);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalTempature", 293);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalPressure", 8886.06);
}

Interface::InflowSix::InflowSix()
{
	//初始化
	this->init();
	this->setDataObjectName("InFlow6");
}

void Interface::InflowSix::initBoundaryTypeParam()
{
	//设置参数
	//   int bcType = 5; 不由用户界面指定，在后端默认配置该参数
	//   int inflowParaType = 9; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("PrimDensity", 41.45637);
	_boundaryTypeParams->createDoubleParameter("PrimU", 0.0);
	_boundaryTypeParams->createDoubleParameter("PrimV", 0.543918);
	_boundaryTypeParams->createDoubleParameter("PrimW", 0.0);
	_boundaryTypeParams->createDoubleParameter("PrimPressure", 8.76743560);
}

Interface::InflowNone::InflowNone()
{
	//初始化
	this->init();
	this->setDataObjectName("InFlowNone");
}

void Interface::InflowNone::initBoundaryTypeParam()
{
}

Interface::InflowSeven::InflowSeven()
{
	//初始化
	this->init();
	this->setDataObjectName("InFlow7");
}

void Interface::InflowSeven::initBoundaryTypeParam()
{
	_boundaryTypeParams->createDoubleParameter("boundaryTypeParams", 41.45637);
}
