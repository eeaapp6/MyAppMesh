/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKOCCModelSphere.h
 * @brief  球体模型
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-05
 *
 */
#ifndef  __FITK_ABSGEO_MODEL_SPHERE_H__
#define  __FITK_ABSGEO_MODEL_SPHERE_H__

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"

namespace Interface
{
    /**
     * @brief 球体模型
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-05
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSphere : 
        public  FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelSphere);
    public:
        /**
         * @brief Construct a new FITKOCCModelSphere object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        explicit FITKAbsGeoModelSphere() = default;
        /**
         * @brief Destroy the FITKOCCModelSphere object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual ~FITKAbsGeoModelSphere();
        /**
         * @brief 设置球心位置
         * @param[i]  p1             球心坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void setLocation(double* p1);
        /**
         * @brief 获取端面圆心坐标
         * @param[o]  p1         圆心坐标
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
         * @brief 获取半径
         * @return double
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double getRadius() const;
        /**
         * @brief 获取几何命令类型
         * @return FITKGeoEnum::FITKGeometryComType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
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
