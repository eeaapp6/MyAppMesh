/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKViewAdaptorRBODY.h"
#include "GraphVTKObjectRBODY.h"
#include "FITK_Component/FITKRadiossData/FITKConnectionRBODY.h"
namespace Exchange
{
    bool GraphVTKViewAdaptorRBODY::update()
    {
        if (_dataObj == nullptr) return false;

        if (!_needUpdate) return (_outputData != nullptr);

        Radioss::FITKConnectionRBODY* rbody = dynamic_cast<Radioss::FITKConnectionRBODY*>(_dataObj);
        if (rbody == nullptr) return false;

        if (_outputData)
        {
            delete _outputData;
            _outputData = nullptr;
        }
        GraphVTKObjectRBODY* outputData = new GraphVTKObjectRBODY(rbody);

        _needUpdate = false;

        if (outputData->getActorCount() == 0)
        {
            delete outputData;
            return false;
        }

        _outputData = outputData;
        return true;
    }
}
