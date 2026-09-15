/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKOCCModelCylinder.h
 * @brief  圆柱体模型
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-05
 *
 */
#ifndef  __FITK_ABSGEO_MODEL_CYLINDER_H__
#define  __FITK_ABSGEO_MODEL_CYLINDER_H__

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"

namespace Interface
{
    /**
     * @brief 圆柱体模型数据
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-05
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCylinder :
        public  FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelCylinder);
    public:
        /**
         * @brief Construct a new FITKOCCModelCylinder object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        explicit FITKAbsGeoModelCylinder() = default;
        /**
         * @brief Destroy the FITKOCCModelCylinder object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual ~FITKAbsGeoModelCylinder();
        /**
         * @brief 设置端面圆心坐标
         * @param[i]  p1         圆心坐标
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
         * @brief 设置轴线方向
         * @param[i]  dir             轴线方向
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void setDirection(double* dir);
        /**
         * @brief 获取轴线方向
         * @param[o]  dir             轴线方向
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void getDirection(double* dir);
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
         * @date 2024-06-05
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
         * @brief 获取长度，圆柱高度
         * @return  double   
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double getLength( ) const;
        /**
         * @brief 获取几何命令类型
         * @return FITKGeoEnum::FITKGeometryComType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */ 
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

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
        double _direction[3] = { 0,0,0 };
        /**
         * @brief 半径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _radius{ 0 };
        /**
         * @brief 长度
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _length{ 0 };
    };


}

#endif
