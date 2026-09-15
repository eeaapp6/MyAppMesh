/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIInFlow.h"
#include "FITKPHengLEIAbstractInFlowType.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	FITKPHengLEIInFlow::FITKPHengLEIInFlow()
	{
		_additionalInFlow = new Core::FITKParameter;
	}

	FITKPHengLEIInFlow::~FITKPHengLEIInFlow()
	{
		if (_additionalInFlow) delete _additionalInFlow;
		if (_inFlowType) delete _inFlowType;
	}

	Core::FITKParameter * FITKPHengLEIInFlow::getInFlowAdditionalData()
	{
		return _additionalInFlow;
	}

	void FITKPHengLEIInFlow::setInFlowAdditionalData(Core::FITKParameter * data)
	{
		if (_additionalInFlow && _additionalInFlow != data) delete _additionalInFlow;
		_additionalInFlow = data;
	}
	void FITKPHengLEIInFlow::setInFlowType(FITKPHengLEIAbstractInFlowType * inFLowType)
	{
		if (_inFlowType != inFLowType && _inFlowType) delete _inFlowType;
		_inFlowType = inFLowType;
	}
	FITKPHengLEIAbstractInFlowType * FITKPHengLEIInFlow::getInFlowType()
	{
		return _inFlowType;
	}
}
