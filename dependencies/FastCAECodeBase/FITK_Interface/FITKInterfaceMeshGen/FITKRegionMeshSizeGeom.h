/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKRegionMeshSizeGeom.h
 * @brief 几何关联的网格区域尺寸
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-08-09
 * 
 */
#ifndef _FITK_REGION_MESH_SIZE_GEOM_H___
#define _FITK_REGION_MESH_SIZE_GEOM_H___

#include "FITKRegionMeshSize.h"
 

namespace Interface
{
    /**
     * @brief 几何关联的网格区域尺寸
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-08-09
     */
    class FITKInterfaceMeshGenAPI FITKRegionMeshSizeGeom :
        public FITKAbstractRegionMeshSize
    {
    public:
        /**
         * @brief Construct a new FITKRegionMeshSizeGeom object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        explicit FITKRegionMeshSizeGeom() = default;

        /**
         * @brief Destroy the FITKRegionMeshSizeGeom object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        virtual ~FITKRegionMeshSizeGeom() = default;

        /**
         * @brief 获取区域类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         * @return RegionType
         */
        virtual RegionType getRegionType() override;
        /**
         * @brief 设置几何ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         * @param id 几何ID
         */
        void setGeomID(const int id);
        /**
         * @brief 获取几何ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         * @return int 几何ID
         */ 
        int getGeomID() const;


    private:
        /**
         * @brief 几何id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        int _geoID{ -1 };
    };
}


#endif
