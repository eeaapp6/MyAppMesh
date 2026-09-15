/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoImportedPart.h" 

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{
    // 获取数据类型。
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoImportedPart::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTImportedPart;
    }
}