/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFSnappyHexMeshParallelDriver.h"

#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractCommandRunner.h"

namespace FoamDriver
{
    int FITKOFSnappyHexMeshParallelDriver::getProgramType()
    {
        return 1;
    }

    QString FITKOFSnappyHexMeshParallelDriver::getProgramName()
    {
        return "mpirun";
    }

    void FITKOFSnappyHexMeshParallelDriver::start()
    {
        //输入参数判空
        if (!_inputInfo) return;

    }

    void FITKOFSnappyHexMeshParallelDriver::stop()
    {
        _commandRunner->killProcess(_commandRunner->getID());
    }
}
