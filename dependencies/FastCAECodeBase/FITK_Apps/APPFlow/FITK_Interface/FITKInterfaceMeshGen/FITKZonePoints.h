/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_ZONE_POINTS_H___
#define _FITK_ZONE_POINTS_H___
 
#include "FITKInterfaceMeshGenAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"

namespace Interface
{
    class FITKInterfaceMeshGenAPI FITKZonePoint : public Core::FITKAbstractDataObject,
        public Core::FITKPoint
    {
    public:
        using Core::FITKPoint::FITKPoint;
        virtual ~FITKZonePoint() = default;

    };

    class FITKInterfaceMeshGenAPI FITKZonePointManager : public Core::FITKAbstractDataObject
        ,public Core::FITKAbstractDataManager<FITKZonePoint>
    {
        FITKCLASS(Interface, FITKZonePointManager);
    public:
        explicit FITKZonePointManager();
        virtual ~FITKZonePointManager() = default;
    };

}


#endif
