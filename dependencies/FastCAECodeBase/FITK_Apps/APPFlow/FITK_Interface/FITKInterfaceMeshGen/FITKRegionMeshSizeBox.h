/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKRegionMeshSizeBox.h
 * @brief 长方体区域声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-17
 *
 */
#ifndef _FITK_REGION_MESH_SIZE_BOX_H___
#define _FITK_REGION_MESH_SIZE_BOX_H___

#include "FITKRegionMeshSize.h"

namespace Interface
{
    /**
     * @brief 长方体区域
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-17
     */
    class FITKInterfaceMeshGenAPI FITKRegionMeshSizeBox :
        public FITKAbstractRegionMeshSize
    {
    public:
        /**
         * @brief Construct a new FITKRegionMeshSizeBox object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        explicit FITKRegionMeshSizeBox();
        /**
         * @brief Destroy the FITKRegionMeshSizeBox object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        virtual ~FITKRegionMeshSizeBox() = default;
        /**
         * @brief 获取区域类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         * @return RegionType
         */
        virtual RegionType getRegionType() override;
        /**
         * @brief 设置第一点坐标，xyz最小点
         * @param[i]  p1             坐标值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void setPoint1(double* p1);
        /**
         * @brief 设置长度
         * @param[i]  length            长度
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void setLength(double* length);
        /**
         * @brief 获取第一点坐标，xyz最小点
         * @param[i]  p1             坐标值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void getPoint1(double* p1);
        /**
         * @brief 获取长度，x y z
         * @param[o]  length             长度值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void getLength(double* length);

    protected:
        /**
         * @brief 第一点坐标，xyz最小点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _point1[3] = { 0,0,0 };
        /**
         * @brief 长度参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _lengthParam[3] = { 10,10,10 };
    };
}


#endif
