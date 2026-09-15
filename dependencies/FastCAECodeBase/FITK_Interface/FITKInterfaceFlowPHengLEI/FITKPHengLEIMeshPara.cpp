/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIMeshPara.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	  FITKPHengLEIMeshPara::  FITKPHengLEIMeshPara()
	{
		_additionalMeshPara =       new Core::FITKParameter;

		_additionalForceReference = new Core::FITKParameter;

		_additionalReferencePoint = new Core::FITKParameter;
	}

	  FITKPHengLEIMeshPara::~  FITKPHengLEIMeshPara()
	{
		if (_additionalMeshPara) delete _additionalMeshPara;
	}

	Core::FITKParameter *   FITKPHengLEIMeshPara::getAdditionalMeshData()
	{
		return _additionalMeshPara;
	}

	void   FITKPHengLEIMeshPara::setAdditionalMeshData(Core::FITKParameter * data)
	{
		if (_additionalMeshPara && _additionalMeshPara != data) delete _additionalMeshPara;
		_additionalMeshPara = data;
	}
	Core::FITKParameter * FITKPHengLEIMeshPara::getAdditionalForceReference()
	{
		return _additionalForceReference;
	}
	void FITKPHengLEIMeshPara::setAdditionalForceReference(Core::FITKParameter * data)
	{
		if (_additionalForceReference&& _additionalForceReference != data) delete _additionalForceReference;
		_additionalForceReference = data;
	}
	Core::FITKParameter * FITKPHengLEIMeshPara::getAdditionalReferencePoint()
	{
		return _additionalReferencePoint;
	}
	void FITKPHengLEIMeshPara::setAdditionalReferencePoint(Core::FITKParameter* data)
	{
		if (_additionalReferencePoint&& _additionalReferencePoint != data) delete _additionalReferencePoint;
		_additionalReferencePoint = data;
	}
}
