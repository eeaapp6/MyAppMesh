/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFInterface.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractCommandRunner.h"

namespace FoamDriver
{

    FITKOFInterface::FITKOFInterface()
    {
        _commandRunner = new AppFrame::FITKLinuxCommandRunner();
    }

    FITKOFInterface::~FITKOFInterface()
    {
        if (_commandRunner != nullptr)
        {
            delete _commandRunner;
            _commandRunner = nullptr;
        }
    }

    void FITKOFInterface::init()
    {
        //初始化 todo
    }

    QString FITKOFInterface::getComponentName()
    {
        //自定义名称
        return "FITKopenFOAMDriver";
    }
}
