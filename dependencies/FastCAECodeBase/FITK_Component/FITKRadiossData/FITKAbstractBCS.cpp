/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractBCS.h"

namespace Radioss
{

FITKAbstractBCS::FITKAbstractBCS()
    : Core::FITKAbstractNDataObject()
{
}

FITKAbstractBCS::~FITKAbstractBCS() = default;

void FITKAbstractBCS::setCoordinateSystemID(int id)
{
    _coordinateSystemID = id;
}

int FITKAbstractBCS::getCoordinateSystemID() const
{
    return _coordinateSystemID;
}

void FITKAbstractBCS::setGroupID(int id1, int id2)
{
    _groupID1 = id1;
    _groupID2 = id2;
}

int FITKAbstractBCS::getGroupID1() const
{
    return _groupID1;
}

int FITKAbstractBCS::getGroupID2() const
{
    return _groupID2;
}

FITKAbstractBCS::BCSType FITKAbstractBCS::getBCSType() const
{
    // Default to BCS, can be overridden in derived classes
    return FITKAbstractBCS::BCSType::BCS;
}

} // namespace Radioss
