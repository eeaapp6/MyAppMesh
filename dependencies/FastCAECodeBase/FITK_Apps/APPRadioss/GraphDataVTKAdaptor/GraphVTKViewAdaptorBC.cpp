/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKViewAdaptorBC.h"
#include "GraphVTKObjectBCS.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"
namespace Exchange
{
    bool GraphVTKViewAdaptorBC::update()
    {
        if (_dataObj == nullptr) return false;

        if (!_needUpdate) return (_outputData != nullptr);

        Radioss::FITKAbstractBCS* bc = dynamic_cast<Radioss::FITKAbstractBCS*>(_dataObj);
        if (bc == nullptr) return false;

        if (_outputData)
        {
            delete _outputData;
            _outputData = nullptr;
        }
        GraphVTKObjectBCS* outputData = nullptr;
        outputData = new GraphVTKObjectBCS(dynamic_cast<Radioss::FITKRadiossBCS*>(bc));
        
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
