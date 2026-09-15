/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKStructuredMesh.h
 * @brief 结构化网格声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-15
 *
 */
#ifndef __FITKSTRUCTUNSTRUCTED_MESH_H__
#define __FITKSTRUCTUNSTRUCTED_MESH_H__

#include "FITKInterfaceModelAPI.h"
#include "FITKNodeList.h"
#include "FITKAbstractMesh.h"

namespace Interface
{
    /**
     * @brief 结构化网格声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-15
     */
    class FITKInterfaceModelAPI FITKStructuredMesh
        : public FITKAbstractMesh, public FITKNodeList
    {
    public:
        /**
         * @brief Construct a new FITKStructuredMesh object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        explicit FITKStructuredMesh() = default;
        /**
         * @brief Destroy the FITKStructuredMesh object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual ~FITKStructuredMesh() =default;
        /**
         * @brief 设置三个维度的点数量
         * @param[i]  i              i方向点数量
         * @param[i]  j              j方向点数量
         * @param[i]  k              k方向点数量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-15
         */
        void setIJKDim(const int i, const int j, const int k);
        /**
         * @brief 获取三个维度点的数量
         * @param[o]  ijk            三个维度点数量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-15
         */
        void getIJKDim(int *ijk);

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


    private:
        /**
         * @brief 三个维度点数量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-15
         */
        int _ijkDim[3] = { 1,1,1 };
    };



}



#endif