/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshParameterBase.h"

namespace Interface
{
    void FITKMeshParameterBase::setModelID(int modelID)
    {
        _modelID = modelID;
    }

    int FITKMeshParameterBase::getModelID()
    {
        return _modelID;
    }
}
