/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKRegionMeshSizeCylinder.h
 * @brief 圆柱区域声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-17
 *
 */
#ifndef _FITK_REGION_MESH_SIZE_CYLINDER_H___
#define _FITK_REGION_MESH_SIZE_CYLINDER_H___

#include "FITKRegionMeshSize.h"

namespace Interface
{
    /**
     * @brief 圆柱区域声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-17
     */
    class FITKInterfaceMeshGenAPI FITKRegionMeshSizeCylinder :
        public FITKAbstractRegionMeshSize
    {
    public:
        /**
         * @brief Construct a new FITKRegionMeshSizeCylinder object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        explicit FITKRegionMeshSizeCylinder();
        /**
         * @brief Destroy the FITKRegionMeshSizeCylinder object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        virtual ~FITKRegionMeshSizeCylinder() = default;

       /**
         * @brief 获取区域类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         * @return RegionType
         */
        virtual RegionType getRegionType() override;
        /**
       * @brief 设置端面圆心坐标
       * @param[i]  p1         圆心坐标
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-06-05
       */
        void setLocation(double* p1);
        /**
         * @brief 获取端面圆心坐标
         * @param[i]  p1             端面圆心坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-22
         */
        void getLocation(double* p1);
        /**
         * @brief 设置轴线方向
         * @param[i]  d             轴线方向
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void setDirection(double* d);
        /**
         * @brief 获取轴线方向
         * @param[i]  d              轴线方向
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-22
         */
        void getDirection(double* d);
        /**
         * @brief 设置半径
         * @param[i]  r          半径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void setRadius(double r);
         /**
         * @brief 获取半径
         * @return double
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-22
         */
        double getRadius() const;
        /**
         * @brief 设置长度，圆柱高度
         * @param[i]  length         长度
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void setLength(double length);
        /**
         * @brief 获取长度
         * @return double
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-22
         */
        double getLength() const;
        /**
         * @brief 设置半径因子
         * @param[i]  fac       半径因子
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void setRadialFraction(double fac);
        /**
         * @brief 获取半径因子
         * @return double
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-22
         */
        double getRadialFraction() const;

        /**
         * @brief 计算断面上与轴线两两垂直的两条轴线
         * @param[o]  axis 六个分量，axis[0-2] 第一坐标轴     axis[3-5] 第二坐标轴
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void getPlaneAxis(double* axis);
    protected:
        /**
         * @brief 端面圆心坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _location[3] = { 0,0,0 };
        /**
         * @brief 轴线方向
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _direction[3] = { 1,0,0 };
        /**
         * @brief 半径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _radius{ 5.0 };
        /**
         * @brief 长度
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _length{ 10.0 };
        /**
         * @brief 半径因子
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-16
         */
        double _radiusFaction{ 0.5 };
    };
}


#endif
