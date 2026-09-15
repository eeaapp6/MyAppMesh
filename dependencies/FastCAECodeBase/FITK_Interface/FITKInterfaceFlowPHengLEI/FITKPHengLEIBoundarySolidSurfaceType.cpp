/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIBoundarySolidSurfaceType.h"

Interface::WallOne::WallOne()
{
	//初始化
	this->init();
	this->setDataObjectName("Wall1");
}

void Interface::WallOne::initBoundaryTypeParam()
{
	//设置参数
	//   int bcType = 2; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("forceReferenceLength", 1.0);
	_boundaryTypeParams->createDoubleParameter("forceReferenceLengthSpanWise", 1.0);
	_boundaryTypeParams->createDoubleParameter("forceReferenceArea", 1.0);
	_boundaryTypeParams->createDoubleParameter("TorqueRefX", 0.0);
	_boundaryTypeParams->createDoubleParameter("TorqueRefY", 0.0);
	_boundaryTypeParams->createDoubleParameter("TorqueRefZ", 0.0);
}

Interface::WallTwo::WallTwo()
{
	//初始化
	this->init();
	this->setDataObjectName("Wall2");
}

void Interface::WallTwo::initBoundaryTypeParam()
{
	//设置参数
//   int bcType = 2; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createDoubleParameter("forceReferenceLength", 1.0);
	_boundaryTypeParams->createDoubleParameter("forceReferenceLengthSpanWise", 1.0);
	_boundaryTypeParams->createDoubleParameter("forceReferenceArea", 1.0);
	_boundaryTypeParams->createDoubleParameter("TorqueRefX", 0.0);
	_boundaryTypeParams->createDoubleParameter("TorqueRefY", 0.0);
	_boundaryTypeParams->createDoubleParameter("TorqueRefZ", 0.0);
	_boundaryTypeParams->createIntParameter("dumpHingeMoment", 0.0);

	QList<double> localCoordAxis0 = { 3.81856010431 ,-0.52758233177 ,0.9137994038 };
	_boundaryTypeParams->createDoubleListParameter("localCoordAxis0", localCoordAxis0);
	QList<double> localCoordAxis1 = { 3.93624962598 ,-0.13193438962 ,0.2285170661 };
	_boundaryTypeParams->createDoubleListParameter("localCoordAxis1", localCoordAxis1);

}

Interface::WallThree::WallThree()
{
	//初始化
	this->init();
	this->setDataObjectName("Wall3");
}

void Interface::WallThree::initBoundaryTypeParam()
{
	//设置参数
//   int bcType = 2; 不由用户界面指定，在后端默认配置该参数
	_boundaryTypeParams->createIntParameter("ViscousType", 1);
	_boundaryTypeParams->createDoubleParameter("wallTemperature", -1.0);
	_boundaryTypeParams->createDoubleParameter("forceReferenceArea", 1.0);
	_boundaryTypeParams->createDoubleParameter("uWall", 0.0);
	_boundaryTypeParams->createDoubleParameter("vWall", 0.0);
	_boundaryTypeParams->createDoubleParameter("wWall", 0.0);
}

Interface::WallNone::WallNone()
{
	//初始化
	this->init();
	this->setDataObjectName("WallNone");
}

void Interface::WallNone::initBoundaryTypeParam()
{
}
