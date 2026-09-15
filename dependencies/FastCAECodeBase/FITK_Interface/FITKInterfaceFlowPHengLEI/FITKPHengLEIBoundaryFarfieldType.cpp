/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIBoundaryFarfieldType.h"


Interface::FarfieldOne::FarfieldOne()
{
	//初始化
	this->init();
	this->setDataObjectName("Farfield1");
}

void Interface::FarfieldOne::initBoundaryTypeParam()
{
	//设置参数
	//   int bcType = 4; 不由用户界面指定，在后端默认配置该参数
	//   int inflowParaType = 0; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("RefMachNumber", 0.1);
	_boundaryTypeParams->createDoubleParameter("Attacked", 0);
	_boundaryTypeParams->createDoubleParameter("AngleSlide", 0);
	_boundaryTypeParams->createDoubleParameter("RefReNumber", 2.0e5);
	_boundaryTypeParams->createDoubleParameter("RefDismensionalTemperature", 293);
}

Interface::FarfieldTwo::FarfieldTwo()
{
	//初始化
	this->init();
	this->setDataObjectName("Farfield2");
}

void Interface::FarfieldTwo::initBoundaryTypeParam()
{
	//设置参数
	//   int bcType = 4; 不由用户界面指定，在后端默认配置该参数
	//   int inflowParaType = 1; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("RefMachNumber", 0.1);
	_boundaryTypeParams->createDoubleParameter("Attacked", 0);
	_boundaryTypeParams->createDoubleParameter("AngleSlide", 0);
	_boundaryTypeParams->createDoubleParameter("Height", 10.0);
}

Interface::FarfieldThree::FarfieldThree()
{
	//初始化
	this->init();
	this->setDataObjectName("Farfield3");
}

void Interface::FarfieldThree::initBoundaryTypeParam()
{
	//设置参数
	//   int bcType = 4; 不由用户界面指定，在后端默认配置该参数
	//   int inflowParaType = 4; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("RefDimensionalVelocity", 1000.0);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalDensity", 1.0e3);
	_boundaryTypeParams->createDoubleParameter("Attacked", 0);
	_boundaryTypeParams->createDoubleParameter("AngleSlide", 0);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalTempature", 293);
}

Interface::FarfieldFour::FarfieldFour()
{
	//初始化
	this->init();
	this->setDataObjectName("Farfield4");
}

void Interface::FarfieldFour::initBoundaryTypeParam()
{
	//设置参数
	//   int bcType = 4; 不由用户界面指定，在后端默认配置该参数
	//   int inflowParaType = 5; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("RefDimensionalVelocity", 1000.0);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalPressure", 1.0e5);
	_boundaryTypeParams->createDoubleParameter("Attacked", 0);
	_boundaryTypeParams->createDoubleParameter("AngleSlide", 0);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalTempature", 293);
}

Interface::FarfieldFive::FarfieldFive()
{
	//初始化
	this->init();
	this->setDataObjectName("Farfield5");
}

void Interface::FarfieldFive::initBoundaryTypeParam()
{
	//设置参数
	//   int bcType = 4; 不由用户界面指定，在后端默认配置该参数
	//   int inflowParaType = 6; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("RefMachNumber", 0.1);
	_boundaryTypeParams->createDoubleParameter("Attacked", 0.0);
	_boundaryTypeParams->createDoubleParameter("AngleSlide", 0.00);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalTempature", 293);
	_boundaryTypeParams->createDoubleParameter("RefDimensionalPressure", 8886.06);
}

Interface::FarfieldSix::FarfieldSix()
{
	//初始化
	this->init();
	this->setDataObjectName("Farfield6");
}

void Interface::FarfieldSix::initBoundaryTypeParam()
{
	//设置参数
	//   int bcType = 4; 不由用户界面指定，在后端默认配置该参数
	//   int inflowParaType = 9; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("PrimDensity", 41.45637);
	_boundaryTypeParams->createDoubleParameter("PrimU", 0.0);
	_boundaryTypeParams->createDoubleParameter("PrimV", 0.543918);
	_boundaryTypeParams->createDoubleParameter("PrimW", 0.0);
	_boundaryTypeParams->createDoubleParameter("PrimPressure", 8.76743560);
}

Interface::FarfieldNone::FarfieldNone()
{
	//初始化
	this->init();
	this->setDataObjectName("FarfieldNone");
}

void Interface::FarfieldNone::initBoundaryTypeParam()
{

}
