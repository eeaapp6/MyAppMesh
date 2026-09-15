/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIFlow.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Interface
{
	FITKPHengLEIFlow::FITKPHengLEIFlow()
	{
		_additionalSolverName =new Core::FITKParameter;

		_additionalInitialValue =new Core::FITKParameter;

		_additionalMomentumEquation =new Core::FITKParameter;

		_additionalPressurePossionEquation=new Core::FITKParameter;

		_additionalSurfaceFlux = new Core::FITKParameter;
	}

	FITKPHengLEIFlow:: ~FITKPHengLEIFlow()
	{
		if (_additionalSolverName) delete _additionalSolverName;
		if (_additionalInitialValue) delete _additionalInitialValue;
		if (_additionalMomentumEquation) delete _additionalMomentumEquation;
		if (_additionalPressurePossionEquation) delete _additionalPressurePossionEquation;
		if (_additionalSurfaceFlux) delete _additionalSurfaceFlux;
	}

	Core::FITKParameter* FITKPHengLEIFlow::getAdditionalSolverName()
	{
		return _additionalSolverName;
	}

	void FITKPHengLEIFlow::setAdditionalSolverName(Core::FITKParameter* data)
	{
		if (_additionalSolverName && _additionalSolverName != data) delete _additionalSolverName;
		_additionalSolverName = data;
	}

	Core::FITKParameter* FITKPHengLEIFlow::getAdditionalInitialValue()
	{
		return _additionalInitialValue;
	}

	void FITKPHengLEIFlow::setAdditionalInitialValue(Core::FITKParameter* data)
	{
		if (_additionalInitialValue && _additionalInitialValue != data) delete _additionalInitialValue;
		_additionalInitialValue = data;
	}

	Core::FITKParameter* FITKPHengLEIFlow::getAdditionalMomentumEquation()
	{
		return _additionalMomentumEquation;
	}

	void FITKPHengLEIFlow::setAdditionalMomentumEquation(Core::FITKParameter* data)
	{
		if (_additionalMomentumEquation && _additionalMomentumEquation != data) delete _additionalMomentumEquation;
		_additionalMomentumEquation = data;
	}

	Core::FITKParameter* FITKPHengLEIFlow::getAdditionalPressurePossionEquation()
	{
		return _additionalPressurePossionEquation;
	}

	void FITKPHengLEIFlow::setAdditionalPressurePossionEquation(Core::FITKParameter* data)
	{
		if (_additionalPressurePossionEquation && _additionalPressurePossionEquation != data) delete _additionalPressurePossionEquation;
		_additionalPressurePossionEquation = data;
	}

	Core::FITKParameter* FITKPHengLEIFlow::getAdditionalSurfaceFlux()
	{
		return _additionalSurfaceFlux;
	}

	void FITKPHengLEIFlow::setAdditionalSurfaceFlux(Core::FITKParameter* data)
	{
		if (_additionalSurfaceFlux && _additionalSurfaceFlux != data) delete _additionalSurfaceFlux;
		_additionalSurfaceFlux = data;
	}
	
}





