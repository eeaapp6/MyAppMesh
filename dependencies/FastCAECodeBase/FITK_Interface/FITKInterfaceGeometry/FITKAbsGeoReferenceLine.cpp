/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelCurve.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITKAbsGeoReferenceLine.h"

namespace Interface
{
    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceLine::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTLine;
    }

    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceSegment::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTSegment;
    }

    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceLineByIntersectPlanes::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTLineByIntersectPlanes;
    }

    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceLineByRotateLine::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTLineByRotateLine;
    }
}
