/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbstractGeometryMeshSizeGenerator.h
 * @brief 几何网格区域尺寸生成器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-23
 */
#ifndef _FITKABSTRACT_GEOMETRY_MESH_SIZE_GENERATOR_H___
#define _FITKABSTRACT_GEOMETRY_MESH_SIZE_GENERATOR_H___

#include "FITKInterfaceMeshGenAPI.h"
#include "FITKGeometryMeshSize.h"

namespace Interface
{
    /**
     * @brief 几何网格尺寸生产器抽象类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-23
     */
    class FITKInterfaceMeshGenAPI FITKAbstractGeometryMeshSizeGenerator
    {
    public:
        /**
         * @brief Construct a new FITKAbstractGeometryMeshSize object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        explicit FITKAbstractGeometryMeshSizeGenerator() = default;
        /**
         * @brief Destroy the FITKAbstractGeometryMeshSize object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        virtual ~FITKAbstractGeometryMeshSizeGenerator() = 0;
        /**
         * @brief 生成几何区域网格尺寸
         * @return FITKGeometryMeshSize*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        virtual FITKGeometryMeshSize* createGeometryMeshSize();
    };

}


#endif
