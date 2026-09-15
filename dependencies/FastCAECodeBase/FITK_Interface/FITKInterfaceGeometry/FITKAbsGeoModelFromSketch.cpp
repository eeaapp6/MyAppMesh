/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelFromSketch.h"

namespace Interface 
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelFromSketch::getGeometryCommandType()
    {
        // 从草图创建模型。
        return FITKGeoEnum::FITKGeometryComType::FGTFromSketch;
    }

    void FITKAbsGeoModelFromSketch::setSketchDataID(int sketchDataID)
    {
        m_sketchDataID = sketchDataID;
    }

    int FITKAbsGeoModelFromSketch::getSketchDataID() const
    {
        return m_sketchDataID;
    }
}