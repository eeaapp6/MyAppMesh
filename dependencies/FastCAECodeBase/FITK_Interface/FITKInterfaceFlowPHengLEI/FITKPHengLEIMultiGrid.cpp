/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIMultiGrid.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	FITKPHengLEIMultiGrid::FITKPHengLEIMultiGrid()
	{
		_additionalMultiGrid = new Core::FITKParameter;
	}

	FITKPHengLEIMultiGrid::~FITKPHengLEIMultiGrid()
	{
		if (_additionalMultiGrid) delete _additionalMultiGrid;
	}

	Core::FITKParameter * FITKPHengLEIMultiGrid::getAdditionalMultiGridData()
	{
		return _additionalMultiGrid;
	}

	void FITKPHengLEIMultiGrid::setAdditionalMultiGridData(Core::FITKParameter * data)
	{
		if (_additionalMultiGrid && _additionalMultiGrid != data) delete _additionalMultiGrid;
		_additionalMultiGrid = data;
	}
	void FITKPHengLEIMultiGrid::setEnableMultiGrid(bool enable)
	{
		_isEnableMultiGrid = enable;
	}

	bool FITKPHengLEIMultiGrid::getEnableMultiGrid()
	{
		return _isEnableMultiGrid;
	}


}
