/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKViewAdaptorRigidWall.h"
#include "GraphVTKObjectRigidWall.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
namespace Exchange
{
    bool GraphVTKViewAdaptorRigidWall::update()
    {
        if (_dataObj == nullptr) return false;

        if (!_needUpdate) return (_outputData != nullptr);

        Radioss::FITKAbstractRWall* wall = dynamic_cast<Radioss::FITKAbstractRWall*>(_dataObj);
        if (wall == nullptr) return false;

        if (_outputData)
        {
            delete _outputData;
            _outputData = nullptr;
        }

        GraphVTKObjectRigidWall* outputData = new GraphVTKObjectRigidWall(wall);
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
