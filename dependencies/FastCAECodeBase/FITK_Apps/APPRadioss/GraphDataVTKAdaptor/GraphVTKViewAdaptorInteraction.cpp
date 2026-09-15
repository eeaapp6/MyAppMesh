/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKViewAdaptorInteraction.h"
#include "GraphVTKObjectInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
namespace Exchange
{
    bool GraphVTKViewAdaptorInteraction::update()
    {
        if (_dataObj == nullptr) return false;

        if (!_needUpdate) return (_outputData != nullptr);

        Radioss::FITKAbstractInteraction* Interaction = dynamic_cast<Radioss::FITKAbstractInteraction*>(_dataObj);
        if (Interaction == nullptr) return false;

        if (_outputData)
        {
            delete _outputData;
            _outputData = nullptr;
        }
        GraphVTKObjectInteraction* outputData = new GraphVTKObjectInteraction(Interaction);
        
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
