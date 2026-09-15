/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFGeometryData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace Interface
{
    FITKOFGeometryData* FITKOFGeometryData::getGeometryDataFromFrame()
    {
        if (FITKAPP == nullptr) return nullptr;
        return FITKAPP->getGlobalData()->getGeometryData<FITKOFGeometryData>();
    }
}


