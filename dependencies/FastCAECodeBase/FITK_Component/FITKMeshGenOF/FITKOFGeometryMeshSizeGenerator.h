/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOFGeometryMeshSizeGenerator.h
 * @brief 几何区域网格尺寸生成器声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-23
 */
#ifndef _FITK_GEOMETRY_MESH_SIZE_GEN_OF_H___
#define _FITK_GEOMETRY_MESH_SIZE_GEN_OF_H___


#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractGeometryMeshSizeGenerator.h"

namespace OF
{
    /**
     * @brief 几何区域网格尺寸生成器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-23
     */
    class FITKOFGeometryMeshSizeGenerator :
        public Interface::FITKAbstractGeometryMeshSizeGenerator
    {
    public:
        /**
         * @brief Construct a new FITKOFGeometryMeshSizeGenerator object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        explicit FITKOFGeometryMeshSizeGenerator() = default;
        /**
         * @brief Destroy the FITKOFGeometryMeshSizeGenerator object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        virtual ~FITKOFGeometryMeshSizeGenerator() = default;
        /**
         * @brief 生成全局尺寸信息
         * @return FITKGeometryMeshSize*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        virtual Interface::FITKGeometryMeshSize* createGeometryMeshSize() override;
    };
}


#endif
