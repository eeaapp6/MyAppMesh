/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEICalculation.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	  FITKPHengLEICalculation::  FITKPHengLEICalculation()
	{
		_additionalConvergencePara = new Core::FITKParameter;
		_additionalParallel = new Core::FITKParameter;
		_additionalSavingIntervals = new Core::FITKParameter;
		_additionalSolverControl = new Core::FITKParameter;
	}

	  FITKPHengLEICalculation::~FITKPHengLEICalculation()
	{
		  if (_additionalConvergencePara) delete _additionalConvergencePara;
		  if (_additionalSavingIntervals) delete _additionalSavingIntervals;
		  if (_additionalParallel) delete _additionalParallel;
		  if (_additionalSolverControl) delete _additionalSolverControl;
	}

	  Core::FITKParameter * FITKPHengLEICalculation::getAdditionalConvergenceData()
	  {
		  return _additionalConvergencePara;
	  }

	  void FITKPHengLEICalculation::setAdditionalConvergenceData(Core::FITKParameter * data)
	  {
		  if (_additionalConvergencePara != data && _additionalConvergencePara) delete _additionalConvergencePara;
		  _additionalConvergencePara = data;
	  }

	  Core::FITKParameter * FITKPHengLEICalculation::getAdditionalSavingIntervalsData()
	  {
		  return _additionalSavingIntervals;
	  }

	  void FITKPHengLEICalculation::setAdditionalSavingIntervalsData(Core::FITKParameter * data)
	  {
		  if (_additionalSavingIntervals != data && _additionalSavingIntervals) delete _additionalSavingIntervals;
		  _additionalSavingIntervals = data;
	  }

	  Core::FITKParameter * FITKPHengLEICalculation::getAdditionalParallelData()
	  {
		  return _additionalParallel;
	  }

	  void FITKPHengLEICalculation::setAdditionalParallelData(Core::FITKParameter * data)
	  {
		  if (_additionalParallel != data && _additionalParallel) delete _additionalParallel;
		  _additionalParallel = data;
	  }
	  Core::FITKParameter * FITKPHengLEICalculation::getAdditionalSolverControlData()
	  {
		  return _additionalSolverControl;
	  }
	  void FITKPHengLEICalculation::setAdditionalSolverControlData(Core::FITKParameter * data)
	  {
		  _additionalSolverControl = data;	  
	  }
}
