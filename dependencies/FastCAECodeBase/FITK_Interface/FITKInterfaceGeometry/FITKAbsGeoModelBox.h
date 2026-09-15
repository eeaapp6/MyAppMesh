/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/


#ifndef  __FITK_ABSGEO_MODEL_BOX_H__
#define  __FITK_ABSGEO_MODEL_BOX_H__

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"

namespace Interface
{
    /**
     * @brief 长方体
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-05
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelBox :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelBox);
    public:
        /**
         * @brief Construct a new FITKOCCModelBox object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        explicit FITKAbsGeoModelBox() = default;
        /**
         * @brief Destroy the FITKOCCModelBox object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual ~FITKAbsGeoModelBox() = 0;
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

        /**
         * @brief 获取几何命令类型
         * @return FITKGeoEnum::FITKGeometryComType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置第一个方向参数（底面第一条边方向）。
         * @param[in]   dir：方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setDirection1(double* dir);

        /**
         * @brief       设置第二个方向参数（底面第二条边方向）。
         * @param[in]   dir：方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setDirection2(double* dir);

        /**
         * @brief       获取第一个方向参数（底面第一条边方向）。
         * @param[in]   dir：方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void getDirection1(double* dir);

        /**
         * @brief       获取第二个方向参数（底面第二条边方向）。
         * @param[in]   dir：方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void getDirection2(double* dir);
      
    protected:
        /**
         * @brief 第一点坐标，xyz最小点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _point1[3] = { 0,0,0 };
        /**
         * @brief 第二点坐标，xyz最大点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        double _lengthParam[3] = { 0,0,0 };

        /**
         * @brief       第一个方向参数（底面第一条边方向）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _direction1[3] = { 1., 0., 0. };

        /**
         * @brief       第二个方向参数（底面第二条边方向）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _direction2[3] = { 0., 1., 0. };
    };


}

#endif
