/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelCut.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelExtrudeCut::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTExtrudeCut;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRevolCut::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRevolCut;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSweepCut::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSweepCut;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelMultiSectionCut::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTMultiSectionCut;
    }
}
