/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractRadMaterial.h"

namespace Radioss
{
    FITKAbstractRadMaterial::RadMatType FITKAbstractRadMaterial::getRadMatType()
    {
        return RadMatType();
    }
    void FITKAbstractRadMaterial::setFailID(int id)
    {
        _failID = id;
    }
    int FITKAbstractRadMaterial::getFailID()
    {
        return _failID;
    }
    void FITKAbstractRadMaterial::setFailState(bool state)
    {
        _isFail = state;
    }
    bool FITKAbstractRadMaterial::isFailState()
    {
        return _isFail;
    }
}