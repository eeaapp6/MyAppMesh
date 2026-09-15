/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/


#include "FITKZonePoints.h"

Interface::FITKZonePointManager::FITKZonePointManager()
{
    //默认有一个成员，位置是0，0，0
    FITKZonePoint* p = new FITKZonePoint(0, 0, 0);
    this->appendDataObj(p);
}

