/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRegionMeshSizeGeom.h"


namespace Interface
{
    FITKAbstractRegionMeshSize::RegionType FITKRegionMeshSizeGeom::getRegionType()
    {
        /** Geometry region type */
        return FITKAbstractRegionMeshSize::RigonGeom;
    }

    void FITKRegionMeshSizeGeom::setGeomID(const int id)
    {
        // Set geometry ID
        _geoID = id;
    }

    int FITKRegionMeshSizeGeom::getGeomID() const
    {
        /** Geometry ID */
        return _geoID;
    }

}



