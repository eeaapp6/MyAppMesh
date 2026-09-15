/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractMeshSizeInfoGenerator.h
 * @brief 网格区域尺寸生产器
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-18
 *
 */
#ifndef _FITKABSTRACT_MESH_SIZE_INFO_GENERATOR_H___
#define _FITKABSTRACT_MESH_SIZE_INFO_GENERATOR_H___

#include "FITKInterfaceMeshGenAPI.h"
#include "FITKRegionMeshSize.h"

namespace Interface
{
    class FITKGlobalMeshSizeInfo;
    /**
     * @brief 网格尺寸生成器抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-18
     */
    class FITKInterfaceMeshGenAPI FITKAbstractMeshSizeInfoGenerator
    {
    public:
        /**
         * @brief Construct a new FITKAbstractMeshSizeInfoGenerator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        explicit FITKAbstractMeshSizeInfoGenerator() = default;
        /**
         * @brief Destroy the FITKAbstractMeshSizeInfoGenerator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual ~FITKAbstractMeshSizeInfoGenerator() = 0;

        /**
         * @brief 生成全局尺寸信息
         * @return FITKGlobalMeshSizeInfo*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        virtual FITKGlobalMeshSizeInfo* generateGlobalMeshSizeInfo();
        /**
         * @brief 生成局部区域网格尺寸
         * @param[i]  t              区域类型
         * @return FITKAbstractRegionMeshSize*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        virtual FITKAbstractRegionMeshSize* createRegionMeshSize(FITKAbstractRegionMeshSize::RegionType t);
    };

}


#endif
