/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEISpeciesModel.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	FITKPHengLEISpeciesModel::FITKPHengLEISpeciesModel()
	{
		_additionalControl = new Core::FITKParameter;
	}

	FITKPHengLEISpeciesModel::~FITKPHengLEISpeciesModel()
	{
		if (_additionalControl) delete _additionalControl;
	}

	Core::FITKParameter * FITKPHengLEISpeciesModel::getAdditionalData()
	{
		return _additionalControl;
	}

	void FITKPHengLEISpeciesModel::setAdditionalData(Core::FITKParameter * data)
	{
		if (_additionalControl && _additionalControl != data) delete _additionalControl;
		_additionalControl = data;
	}
}
