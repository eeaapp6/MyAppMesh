/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelPartitionSolid.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionSolidWithPlane::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionSolidWithPlane;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionSolidWithExtendFace::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionSolidWithExtendFace;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionSolidWithSweepEdge::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionSolidWithSweepEdge;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionSolidWithNSidedPatch::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionSolidWithNSidedPatch;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionSolidWithSketchPlanar::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionSolidWithSketchPlanar;
    }

}
