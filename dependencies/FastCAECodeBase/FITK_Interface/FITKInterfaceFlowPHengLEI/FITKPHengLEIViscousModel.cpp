/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIViscousModel.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKPHengLEIViscousType.h"

namespace Interface
{
	FITKPHengLEIViscousModel::FITKPHengLEIViscousModel()
	{
		_additionalViscous = new Core::FITKParameter;
	}

	FITKPHengLEIViscousModel::~FITKPHengLEIViscousModel()
	{
		if (_additionalViscous) delete _additionalViscous;
		if (_viscousType) delete _viscousType;
	}

	Core::FITKParameter * FITKPHengLEIViscousModel::getAdditionalVicousModelData()
	{
		return _additionalViscous;
	}

	void FITKPHengLEIViscousModel::setAdditionalVicousModelData(Core::FITKParameter * data)
	{
		if (_additionalViscous && _additionalViscous != data) delete _additionalViscous;
		_additionalViscous = data;
	}
	void FITKPHengLEIViscousModel::setViscousType(FITKPHengLEIViscousType * viscousType)
	{
		if (_viscousType != viscousType && _viscousType) delete _viscousType;
		_viscousType = viscousType;
	}
	FITKPHengLEIViscousType * FITKPHengLEIViscousModel::getViscousType()
	{
		return _viscousType;
	}

	FITKPHengLEIAbsSolver::PHengLEIViscousType FITKPHengLEIViscousModel::getViscousEnumType()
	{
		return _type;
	}
	void FITKPHengLEIViscousModel::setViscousEnumType(FITKPHengLEIAbsSolver::PHengLEIViscousType type)
	{
		_type = type;
	}

}
