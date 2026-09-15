/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKViewAdaptorPartMesh.h"
#include "GraphVTKObjectPartMesh.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

namespace Exchange
{
    bool GraphVTKViewAdaptorPartMesh::update()
    {
        if (_dataObj == nullptr) return false;

        if (!_needUpdate) return (_outputData != nullptr);

        Radioss::FITKRadiossPart* model = dynamic_cast<Radioss::FITKRadiossPart*>(_dataObj);
        if (model == nullptr) return false;

        if (_outputData)
        {
            delete _outputData;
            _outputData = nullptr;
        }

        GraphVTKObjectPartMesh* outputData = new GraphVTKObjectPartMesh(model);
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
