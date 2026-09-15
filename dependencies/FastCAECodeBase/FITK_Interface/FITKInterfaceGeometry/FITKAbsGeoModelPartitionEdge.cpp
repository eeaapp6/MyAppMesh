/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelPartitionEdge.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionEdgeWithParameter::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTPartitionEdgeWithParameter;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionEdgeWithPoint::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTPartitionEdgeWithPoint;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionEdgeWithDatumPlane::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTPartitionEdgeWithDatumPlane;
    }

}
