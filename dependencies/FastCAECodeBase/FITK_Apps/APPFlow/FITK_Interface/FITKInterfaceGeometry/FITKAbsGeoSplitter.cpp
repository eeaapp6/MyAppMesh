/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoSplitter.h"

namespace Interface {
    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoCurveSplitter::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTCurveSplitter;
    }

    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoSurfaceSplitter::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSurfaceSplitter;
    }

    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoSolidSplitter::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSolidSplitter;
    }

}