/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelPoint.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITKAbsGeoReferencePoint.h"

namespace Interface
{
    Interface::FITKGeoEnum::FITKDatumType FITKAbsGeoReferencePoint::getDatumType()
    {
        return Interface::FITKGeoEnum::FITKDatumType::FDTPoint;
    }


    Interface::FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceCurveRadio::getDatumType()
    {
        return Interface::FITKGeoEnum::FITKDatumType::FDTCurveRadio;
    }

}
