/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKViewAdaptorSurf.h"
#include "GraphVTKObjectSurf.h"

#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurface.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"

namespace Exchange
{
    bool GraphVTKViewAdaptorSurf::update()
    {
        if (_dataObj == nullptr) {
            return false;
        }

        if (!_needUpdate)
        {
            return (_outputData != nullptr);
        }

        Interface::FITKMeshSurface* model = dynamic_cast<Interface::FITKMeshSurface*>(_dataObj);
        if (model == nullptr) {
            return false;
        }

        if (_outputData) {
            delete _outputData;
            _outputData = nullptr;
        }

        GraphVTKObjectSurf* outputData = new GraphVTKObjectSurf(model, _details.toInt());
        _needUpdate = false;

        if (outputData->getActorCount() == 0) {
            delete outputData;
            return false;
        }

        _outputData = outputData;
        return true;
    }
}