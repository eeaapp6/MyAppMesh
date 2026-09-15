/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_REGION_MESH_SIZE_SPHERE_H___
#define _FITK_REGION_MESH_SIZE_SPHERE_H___

#include "FITKRegionMeshSize.h"

namespace Interface
{
    class FITKInterfaceMeshGenAPI FITKRegionMeshSizeSphere :
        public FITKAbstractRegionMeshSize
    {
    public:
        explicit FITKRegionMeshSizeSphere();
        virtual ~FITKRegionMeshSizeSphere() = default;

        /**
         * @brief 获取区域类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         * @return RegionType
         */
        virtual RegionType getRegionType() override;

        /**
         * @brief 设置球心位置
         * @param[i]  p1             球心坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void setLocation(double* p1);

        /**
         * @brief 获取球心位置
         * @param[i]  p1             球心坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void getLocation(double* p1);

        /**
         * @brief 设置球半径
         * @param[i]  r             球半径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void setRadius(double r);
        /**
         * @brief 设置球半径
         * @return double
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double getRadius() const;



    protected:
        /**
         * @brief 圆心坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _location[3] = { 0,0,0 };
        /**
         * @brief 半径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _radius{ 0 };
    };
}


#endif
