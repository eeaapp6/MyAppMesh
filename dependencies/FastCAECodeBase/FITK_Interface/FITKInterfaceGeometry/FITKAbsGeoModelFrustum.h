/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelFrustum.h
 * @brief       棱台建模命令类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-08
 *********************************************************************/

#ifndef  __FITKABSGEMODELFRUSTUM_H__
#define  __FITKABSGEMODELFRUSTUM_H__

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"

namespace Interface
{
    /**
     * @brief       棱台建模命令类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-08
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelFrustum :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelFrustum);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        explicit FITKAbsGeoModelFrustum() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        virtual ~FITKAbsGeoModelFrustum();

        /**
         * @brief       设置底面中心坐标。
         * @param[in]   p: 底面中心坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setLocation(double* p);

        /**
         * @brief       获取底面中心坐标。
         * @param[out]  p: 底面中心坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void getLocation(double* p);

        /**
         * @brief       设置底面长度。
         * @param[in]   length: 底面长度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setBottomLength(double length);

        /**
         * @brief       获取底面长度。
         * @return      double
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double getBottomLength() const;

        /**
         * @brief       设置底面宽度。
         * @param[in]   width: 底面宽度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setBottomWidth(double width);

        /**
         * @brief       获取底面宽度。
         * @return      double
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double getBottomWidth() const;

        /**
         * @brief       设置顶面长度。
         * @param[in]   length: 顶面长度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setTopLength(double length);

        /**
         * @brief       获取顶面长度。
         * @return      double
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double getTopLength() const;

        /**
         * @brief       设置顶面宽度。
         * @param[in]   width: 顶面宽度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setTopWidth(double width);

        /**
         * @brief       获取顶面宽度。
         * @return      double
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double getTopWidth() const;

        /**
         * @brief       设置高度。
         * @param[in]   length: 高度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setLength(double length);

        /**
         * @brief       获取高度。
         * @return      double
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double getLength() const;

        /**
         * @brief       设置第一个方向参数（底面X轴方向）。
         * @param[in]   dir: 方向向量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setDirection1(double* dir);

        /**
         * @brief       获取第一个方向参数（底面X轴方向）。
         * @param[out]  dir: 方向向量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void getDirection1(double* dir);

        /**
         * @brief       设置第二个方向参数（底面Y轴方向）。
         * @param[in]   dir: 方向向量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setDirection2(double* dir);

        /**
         * @brief       获取第二个方向参数（底面Y轴方向）。
         * @param[out]  dir: 方向向量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void getDirection2(double* dir);

        /**
         * @brief       获取几何命令类型。
         * @return      FITKGeoEnum::FITKGeometryComType
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    protected:
        /**
         * @brief       底面中心坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _location[3] = { 0, 0, 0 };

        /**
         * @brief       底面长度。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _bottomLength{ 4 };

        /**
         * @brief       底面宽度。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _bottomWidth{ 4 };

        /**
         * @brief       顶面长度。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _topLength{ 3 };

        /**
         * @brief       顶面宽度。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _topWidth{ 3 };

        /**
         * @brief       高度。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _length{ 3 };

        /**
         * @brief       第一个方向参数（底面X轴方向）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _direction1[3] = { 1., 0., 0. };

        /**
         * @brief       第二个方向参数（底面Y轴方向）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _direction2[3] = { 0., 1., 0. };
    };
}

#endif // !__FITKABSGEMODELFRUSTUM_H__
