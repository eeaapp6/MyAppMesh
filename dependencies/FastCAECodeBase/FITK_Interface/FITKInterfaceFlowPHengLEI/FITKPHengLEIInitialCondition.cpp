/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIInitialCondition.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	FITKPHengLEIInitialCondition::FITKPHengLEIInitialCondition()
	{
		_additionalInitialCondition = new Core::FITKParameter;
	}

	FITKPHengLEIInitialCondition::~FITKPHengLEIInitialCondition()
	{
		if (_additionalInitialCondition) delete _additionalInitialCondition;
	}

	Core::FITKParameter * FITKPHengLEIInitialCondition::getInitialConditionAdditionalData()
	{
		return _additionalInitialCondition;
	}

	void FITKPHengLEIInitialCondition::setInitialConditionAdditionalData(Core::FITKParameter * data)
	{
		if (_additionalInitialCondition && _additionalInitialCondition != data) delete _additionalInitialCondition;
		_additionalInitialCondition = data;
	}
}
