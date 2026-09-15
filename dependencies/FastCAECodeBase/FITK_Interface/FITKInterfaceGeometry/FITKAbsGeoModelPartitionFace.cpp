/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelPartitionFace.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithSketch::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithSketch;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithTwoPoints::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithTwoPoints;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithDatumPlane::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithDatumPlane;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithCurvedPath::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithCurvedPath;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithExtendFace::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithExtendFace;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithIntersectFace::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithIntersectFace;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithProjectEdges::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithProjectEdges;
    }

}
