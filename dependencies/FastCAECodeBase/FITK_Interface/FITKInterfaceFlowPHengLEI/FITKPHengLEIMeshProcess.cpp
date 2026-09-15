/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIMeshProcess.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamRadioGroup.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Interface
{
	FITKPHengLEIMeshProcess::FITKPHengLEIMeshProcess()
	{
		_additionalMeshData =new Core::FITKParameter;

		_additionalMeshPartion =new Core::FITKParameter;

		_additionalMeshConversion =new Core::FITKParameter;

		initMeshProcess();
	}
	FITKPHengLEIMeshProcess:: ~FITKPHengLEIMeshProcess()
	{
		if (_additionalMeshData) delete _additionalMeshData;
		if (_additionalMeshPartion) delete _additionalMeshPartion;
		if (_additionalMeshConversion) delete _additionalMeshConversion;
	}

	Core::FITKParameter* FITKPHengLEIMeshProcess::getAdditionalMeshData()
	{
		return _additionalMeshData;
	}

	void FITKPHengLEIMeshProcess::setAdditionalMeshData(Core::FITKParameter* data)
	{
		if (_additionalMeshData && _additionalMeshData != data) delete _additionalMeshData;
		_additionalMeshData = data;
	}

	Core::FITKParameter* FITKPHengLEIMeshProcess::getAdditionalMeshPartion()
	{
		return _additionalMeshPartion;
	}

	void FITKPHengLEIMeshProcess::setAdditionalMeshPartion(Core::FITKParameter* data)
	{
		if (_additionalMeshPartion && _additionalMeshPartion != data) delete _additionalMeshPartion;
		_additionalMeshPartion = data;
	}

	Core::FITKParameter* FITKPHengLEIMeshProcess::getAdditionalMeshConversion()
	{
		return _additionalMeshConversion;
	}

	void FITKPHengLEIMeshProcess::setAdditionaleshConversion(Core::FITKParameter* data)
	{
		if (_additionalMeshConversion && _additionalMeshConversion != data) delete _additionalMeshConversion;
		_additionalMeshConversion = data;
	}

	void FITKPHengLEIMeshProcess::initMeshProcess()
	{
		if (!_additionalMeshData) return;
		QStringList s1 = { "2D" ,"3D"};
		QStringList s2 = { "Struct" ,"Unstruct" };
		_additionalMeshData->createComboxParameter("Grid Dimension", s1,1);

		QString sex = "male";
		Core::FITKParamRadioGroup* location = _additionalMeshData->createRadioGroupParameter(sex);
		location->appendValue("2D", nullptr);
		location->appendValue("3D", nullptr);


		_additionalMeshData->createComboxParameter("Grid Type", s2,1);

		if (!_additionalMeshConversion) return;
		_additionalMeshConversion->createIntParameter("Axis Rotate Count", 3,3,0);

		QList<QStringList> s3 = { {"X","Y","Z"},{"X","y","Z"},{"X","Y","Z"} };
		_additionalMeshConversion->createComBoxVectorParameter("Axis Rotate Order", s3);
		QList<double> s4 = { 90,90,90 };
		_additionalMeshConversion->createDoubleListParameter("Axis Rotate Angles", s4);

		if (!_additionalMeshPartion) return;
		_additionalMeshPartion->createBoolParameter("Enable", true); 
		_additionalMeshPartion->createIntParameter("Max Processors",1);
		_additionalMeshPartion->createIntParameter("Multi-grid Count", 0);

	}
	
}





