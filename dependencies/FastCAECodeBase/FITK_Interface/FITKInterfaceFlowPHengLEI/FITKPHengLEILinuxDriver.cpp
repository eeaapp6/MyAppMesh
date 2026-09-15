/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEILinuxDriver.h"

#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractCommandRunner.h"

namespace PHengLEIDriver
{
	int FITKPHengLEILinuxDriver::getProgramType()
	{
		return 1;
	}

	QString FITKPHengLEILinuxDriver::getProgramName()
	{
		return "PHengLEI";
	}

}
