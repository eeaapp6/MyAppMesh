/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEISolution.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	  FITKPHengLEISolution::  FITKPHengLEISolution()
	{
		_additionalMomentumEquationPara = new Core::FITKParameter;
		_additionalEnergyEquationPara = new Core::FITKParameter;
		_additionalTurbulenceEquationPara = new Core::FITKParameter;
		_additionalSpeciesEquationPara = new Core::FITKParameter;
		_additionalRelaxationPara = new Core::FITKParameter;
		_additionalPressurePossionEquationPara = new Core::FITKParameter;
	}

	  FITKPHengLEISolution::~FITKPHengLEISolution()
	{
		if (_additionalMomentumEquationPara) delete _additionalMomentumEquationPara;
		if (_additionalEnergyEquationPara) delete _additionalEnergyEquationPara;
		if (_additionalTurbulenceEquationPara) delete _additionalTurbulenceEquationPara;
		if (_additionalSpeciesEquationPara) delete _additionalSpeciesEquationPara;
		if (_additionalRelaxationPara) delete _additionalRelaxationPara;
		if (_additionalPressurePossionEquationPara) delete _additionalPressurePossionEquationPara;
	}

	  Core::FITKParameter * FITKPHengLEISolution::getAdditionalMomentumEquationData()
	  {
		  return _additionalMomentumEquationPara;
	  }

	  void FITKPHengLEISolution::setAdditionalMomentumEquationData(Core::FITKParameter * data)
	  {
		  _additionalMomentumEquationPara = data;
	  }

	  Core::FITKParameter * FITKPHengLEISolution::getAdditionalTurbulenceEquationData()
	  {
		  return _additionalTurbulenceEquationPara;
	  }

	  void FITKPHengLEISolution::setAdditionalTurbulenceEquationData(Core::FITKParameter * data)
	  {
		  _additionalTurbulenceEquationPara;
	  }

	  Core::FITKParameter * FITKPHengLEISolution::getAdditionalEnergyEquationData()
	  {
		  return _additionalEnergyEquationPara;
	  }

	  void FITKPHengLEISolution::setAdditionalEnergyEquationData(Core::FITKParameter * data)
	  {
		  _additionalEnergyEquationPara = data;
	  }

	  Core::FITKParameter * FITKPHengLEISolution::getAdditionalSpeciesEquationData()
	  {
		  return _additionalSpeciesEquationPara;
	  }

	  void FITKPHengLEISolution::setAdditionalSpeciesEquationData(Core::FITKParameter * data)
	  {
		  _additionalSpeciesEquationPara = data;
	  }

	  Core::FITKParameter * FITKPHengLEISolution::getAdditionalRelaxationData()
	  {
		  return _additionalRelaxationPara;
	  }

	  void FITKPHengLEISolution::setAdditionalRelaxationData(Core::FITKParameter * data)
	  {
		  _additionalRelaxationPara = data;
	  }

	  Core::FITKParameter * FITKPHengLEISolution::getAdditionalPressurePossionEquation()
	  {
		  return _additionalPressurePossionEquationPara;
	  }

	  void FITKPHengLEISolution::setAdditionalPressurePossionEquation(Core::FITKParameter * data)
	  {
		  _additionalPressurePossionEquationPara = data;
	  }


}
