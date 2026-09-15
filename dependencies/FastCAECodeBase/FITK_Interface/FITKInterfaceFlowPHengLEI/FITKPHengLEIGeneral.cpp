/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIGeneral.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	FITKPHengLEIGeneral::FITKPHengLEIGeneral()
	{
		_axisRotateData = new Core::FITKParameter;
		_transientData = new Core::FITKParameter;
		_meshScale = new Core::FITKParameter;
		initGenral();
	}

	FITKPHengLEIGeneral::~FITKPHengLEIGeneral()
	{
		if (_axisRotateData) delete _axisRotateData;
		if (_transientData) delete _transientData;
		if (_meshScale) delete _meshScale;
	}

	Core::FITKParameter * FITKPHengLEIGeneral::getAxisRotateData()
	{
		return _axisRotateData;
	}

	void FITKPHengLEIGeneral::setAxisRotateData(Core::FITKParameter * data)
	{
		if (_axisRotateData && _axisRotateData != data) delete _axisRotateData;
		_axisRotateData = data;
	}
	Core::FITKParameter * FITKPHengLEIGeneral::getTransientData()
	{
		return _transientData;
	}
	void FITKPHengLEIGeneral::setTransientData(Core::FITKParameter * data)
	{
		if (_transientData && _transientData != data) delete _transientData;
		_transientData = data;
	}
	void FITKPHengLEIGeneral::setMeshScale(Core::FITKParameter * data)
	{
		if (_meshScale != data && _meshScale) delete _meshScale;
		_meshScale = data;
	}
	Core::FITKParameter * FITKPHengLEIGeneral::getMeshScale()
	{
		return _meshScale;
	}
	void FITKPHengLEIGeneral::initGenral()
	{
		if (!_axisRotateData) return;
		_axisRotateData->createIntParameter("Axis Rotate Count", 3, 3, 0);

		QList<QStringList> s3 = { {"X","Y","Z"},{"X","y","Z"},{"X","Y","Z"} };
		_axisRotateData->createComBoxVectorParameter("Axis Rotate Order", s3);
		QList<double> s4 = { 90,90,90 };
		_axisRotateData->createDoubleListParameter("Axis Rotate Angles", s4);
		
		if (!_transientData) return;
		QStringList TimeList = { "Steady State","Unsteady State" };
		_transientData->createComboxParameter("Time", TimeList, 0);

		if (!_meshScale) return;
		QStringList convertUnits = { "m","cm","mm","in","ft"};
		_meshScale->createComboxParameter("Convert Units", convertUnits,0);
	}


}
