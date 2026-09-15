/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOpenFoamMeshingDriverBase.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractCommandRunner.h"

namespace FoamDriver
{
    FITKOpenFoamMeshingDriverBase::~FITKOpenFoamMeshingDriverBase()
    {
    }


    //执行完成的槽函数
    void FITKOpenFoamMeshingDriverBase::threadFinishedSlot()
    {
        //发送执行完成信号
        emit sig_Finish();
    }
}


