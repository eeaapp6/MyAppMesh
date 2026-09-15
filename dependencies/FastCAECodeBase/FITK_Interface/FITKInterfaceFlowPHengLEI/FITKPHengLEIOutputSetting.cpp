/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIOutputSetting.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	FITKPHengLEIOutputSetting::FITKPHengLEIOutputSetting()
	{
		_basicFlowData = new Core::FITKParameter;
		_viscosityData = new Core::FITKParameter;
		_vorticityData = new Core::FITKParameter;
		_derivedFlowData = new Core::FITKParameter;
		_turbulenceModelData = new Core::FITKParameter;
		_nonEquilibriumData = new Core::FITKParameter;
		_dimensionalData = new Core::FITKParameter;
		_dimensionlessData = new Core::FITKParameter;
	}

	FITKPHengLEIOutputSetting::~FITKPHengLEIOutputSetting()
	{
		if (_basicFlowData) delete _basicFlowData;
		if (_viscosityData) delete _viscosityData;
		if (_vorticityData) delete _vorticityData;
		if (_derivedFlowData) delete _derivedFlowData;
		if (_turbulenceModelData) delete _turbulenceModelData;
		if (_nonEquilibriumData) delete _nonEquilibriumData;
		if (_dimensionalData) delete _dimensionalData;
		if (_dimensionlessData) delete _dimensionlessData;

	}
	Core::FITKParameter * FITKPHengLEIOutputSetting::getBasicFlowData()
	{
		return _basicFlowData;
	}
	void FITKPHengLEIOutputSetting::setBasicFlowData(Core::FITKParameter * data)
	{
		if (_basicFlowData != _basicFlowData && _basicFlowData) delete _basicFlowData;
		_basicFlowData = data;
	}
	Core::FITKParameter * FITKPHengLEIOutputSetting::getViscosityData()
	{
		return _viscosityData;
	}
	void FITKPHengLEIOutputSetting::setViscosityData(Core::FITKParameter * data)
	{
		if (_viscosityData != data && _viscosityData) delete _viscosityData;
		_viscosityData = data;
	}
	Core::FITKParameter * FITKPHengLEIOutputSetting::getVorticityData()
	{
		return _vorticityData;
	}
	void FITKPHengLEIOutputSetting::setVorticityData(Core::FITKParameter * data)
	{
		if (_vorticityData != data && _vorticityData) delete _vorticityData;
		_vorticityData = data;
	}
	Core::FITKParameter * FITKPHengLEIOutputSetting::getDerivedFlowData()
	{
		return _derivedFlowData;
	}
	void FITKPHengLEIOutputSetting::setDerivedFlowData(Core::FITKParameter * data)
	{
		if (_derivedFlowData != data && _derivedFlowData) delete _derivedFlowData;
		_derivedFlowData = data;
	}
	Core::FITKParameter * FITKPHengLEIOutputSetting::getTurbulenceModelData()
	{
		return _turbulenceModelData;
	}
	void FITKPHengLEIOutputSetting::setTurbulenceModelData(Core::FITKParameter * data)
	{
		if (_turbulenceModelData != data && _turbulenceModelData) delete _turbulenceModelData;
		_turbulenceModelData = data;
	}
	Core::FITKParameter * FITKPHengLEIOutputSetting::getNonEquilibriumData()
	{
		return _nonEquilibriumData;
	}
	void FITKPHengLEIOutputSetting::setNonEquilibriumData(Core::FITKParameter * data)
	{
		if (_nonEquilibriumData != data && _nonEquilibriumData) delete _nonEquilibriumData;
		_nonEquilibriumData = data;
	}
	Core::FITKParameter * FITKPHengLEIOutputSetting::getDimensionalData()
	{
		return _dimensionalData;
	}
	void FITKPHengLEIOutputSetting::setDimensionalData(Core::FITKParameter * data)
	{
		if (_dimensionlessData != data && _dimensionlessData) delete _dimensionlessData;
		_dimensionlessData = data;
	}
	Core::FITKParameter * FITKPHengLEIOutputSetting::getDimensionlessData()
	{
		return _dimensionlessData;
	}
	void FITKPHengLEIOutputSetting::setDimensionlessData(Core::FITKParameter * data)
	{
		if (_dimensionlessData != data && _dimensionlessData) delete _dimensionlessData;
		_dimensionlessData = data;
	}
}
