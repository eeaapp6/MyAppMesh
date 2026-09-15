/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIDriver.h"

#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractCommandRunner.h"

namespace PHengLEIDriver
{
    int FITKPHengLEIDriver::getProgramType()
    {
        return 1;
    }

    QString FITKPHengLEIDriver::getProgramName()
    {
        return "PHengLEI";
    }

	void FITKPHengLEIDriver::threadFinishedSlot()
	{
		//发送执行完成信号
		emit sig_Finish();
	}
}
