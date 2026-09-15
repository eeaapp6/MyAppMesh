/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelSolid.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelExtrudeSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTExtrudeSolid;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRevolSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRevolSolid;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSweepSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSweepSolid;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelClosedSurfaceSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTClosedSurfaceSolid;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelMultiSectionSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTMultiSectionSolid;
    }
}
