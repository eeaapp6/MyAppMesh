/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIEnergyModel.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	FITKPHengLEIEnergyModel::FITKPHengLEIEnergyModel()
	{
		_additionalEnergyModel = new Core::FITKParameter;
	}

	FITKPHengLEIEnergyModel::~FITKPHengLEIEnergyModel()
	{
		if (_additionalEnergyModel) delete _additionalEnergyModel;
	}

	Core::FITKParameter * FITKPHengLEIEnergyModel::getAdditionalData()
	{
		return _additionalEnergyModel;
	}

	void FITKPHengLEIEnergyModel::setAdditionalData(Core::FITKParameter * data)
	{
		if (_additionalEnergyModel && _additionalEnergyModel != data) delete _additionalEnergyModel;
		_additionalEnergyModel = data;
	}
}
