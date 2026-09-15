/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIDiscretisation.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKPHengLEIAbsSpatialDiscretisation.h"

namespace Interface
{
	FITKPHengLEIDiscretisation::FITKPHengLEIDiscretisation()
	{
		_additionalTemporal = new Core::FITKParameter;
		_additionalFlowPara = new Core::FITKParameter;
	}

	FITKPHengLEIDiscretisation::~FITKPHengLEIDiscretisation()
	{
		if (_additionalTemporal) delete _additionalTemporal;
		if (_spatialDiscretisation) delete _spatialDiscretisation;
		if (_additionalFlowPara) delete _additionalFlowPara;
	}

	Core::FITKParameter * FITKPHengLEIDiscretisation::getTemporalAdditionalData()
	{
		return _additionalTemporal;
	}

	FITKPHengLEIAbsSpatialDiscretisation * FITKPHengLEIDiscretisation::getSpatialDiscretisation()
	{
		return _spatialDiscretisation;
	}

	void FITKPHengLEIDiscretisation::setSpatialDiscretisation(FITKPHengLEIAbsSpatialDiscretisation * spatialDiscretisation)
	{
		if (_spatialDiscretisation != spatialDiscretisation && _spatialDiscretisation) delete _spatialDiscretisation;
		_spatialDiscretisation = spatialDiscretisation;
	}
	Core::FITKParameter * FITKPHengLEIDiscretisation::getFlowDiscretisation()
	{
		return _additionalFlowPara;
	}
	void FITKPHengLEIDiscretisation::setFlowDiscretisation(Core::FITKParameter * data)
	{
		if (_additionalFlowPara != data && _additionalFlowPara) delete _additionalFlowPara;
		_additionalFlowPara = data;
	}
}
