/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAbstractMeshAlgorithmInfoGenerator.h
 * @brief  网格算法生成器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-10-15
 *********************************************************************/
#ifndef _FITKABSTRACT_MESH_Algorithm_INFO_GENERATOR_H___
#define _FITKABSTRACT_MESH_Algorithm_INFO_GENERATOR_H___

#include "FITKInterfaceMeshGenAPI.h"

namespace Interface
{
    class FITKGlobalMeshGenerateAlgorithmInfo;
    /**
     * @brief 网格算法生成器抽象类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-10-15
     */
    class FITKInterfaceMeshGenAPI FITKAbstractMeshAlgorithmInfoGenerator
    {
    public:
        /**
         * @brief Construct a new FITKAbstractMeshSizeInfoGenerator object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-15
         */
        explicit FITKAbstractMeshAlgorithmInfoGenerator() = default;
        /**
         * @brief Destroy the FITKAbstractMeshSizeInfoGenerator object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-15
         */
        virtual ~FITKAbstractMeshAlgorithmInfoGenerator() = 0;

        /**
         * @brief 生成全局网格算法信息
         * @return FITKGlobalMeshSizeInfo*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-15
         */
        virtual FITKGlobalMeshGenerateAlgorithmInfo* generateGlobalMeshAlgorithmInfo();
    };

}


#endif
