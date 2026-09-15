/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITKOFGEOMETRYDATA__H__
#define _FITKOFGEOMETRYDATA__H__

#include "FITKInterfaceFlowOFAPI.h" 
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"

namespace Interface
{
    class FITKInterfaceFlowOFAPI FITKOFGeometryData :  
         public FITKGeoCommandList
    {
    public:
        explicit FITKOFGeometryData() = default;
        virtual ~FITKOFGeometryData() = default;

        static FITKOFGeometryData* getGeometryDataFromFrame();
    
    };
}


#endif
