/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoTransformation.h"

namespace Interface {

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelTransform::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTTransform;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelTransformByTwoPoints::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTTransformByTwoPoints;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelTransformByDirAndDis::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTTransformByDirAndDis;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRotate::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRotate;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelScale::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTScale;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelMirror::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTMirror;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPattern::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTPattern;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRectangularPattern::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRectangularPattern;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCircularPattern::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTCircularPattern;
    }
}
