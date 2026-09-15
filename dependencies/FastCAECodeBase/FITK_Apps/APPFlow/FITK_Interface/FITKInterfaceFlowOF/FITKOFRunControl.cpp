/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFRunControl.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{

    FITKOFRunControl::FITKOFRunControl()
    {
        _timeControl = new Core::FITKParameter;
        _outputControl = new Core::FITKParameter;
    }

    FITKOFRunControl::~FITKOFRunControl()
    {
        if (_timeControl) delete _timeControl;
        if (_outputControl) delete _outputControl;
    }

    Core::FITKParameter* FITKOFRunControl::getTimeControl()
    {
        return _timeControl;
    }

    Core::FITKParameter* FITKOFRunControl::getOutputControl()
    {
        return _outputControl;
    }
}
