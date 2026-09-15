/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKParticalMesh.h
 * @brief 粒子网格
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-15
 *
 */
#ifndef __FITKPARTICAL_MESH_H__
#define __FITKPARTICAL_MESH_H__

#include "FITKInterfaceModelAPI.h"
#include "FITKNodeList.h"
#include "FITKAbstractMesh.h"

namespace Interface
{
    /**
     * @brief 粒子网格声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-15
     */
    class FITKInterfaceModelAPI FITKParticalMesh
        : public FITKAbstractMesh, public FITKNodeList
    {
    public:
        /**
         * @brief Construct a new FITKParticalMesh object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        explicit FITKParticalMesh() = default;
        /**
         * @brief Destroy the FITKParticalMesh object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual ~FITKParticalMesh() = default;

        /**
          * @brief 获取模型类型
          * @return AbsModelType
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-02-27
          */
        virtual FITKModelEnum::AbsModelType getAbsModelType() override;
        /**
         * @brief 更新数据
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        virtual void update() override;
        /**
         * @brief 获取网格维度
         * @return FITKModelEnum::FITKMeshDim
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        virtual FITKModelEnum::FITKMeshDim getMeshDim() override;
    };
}

#endif
