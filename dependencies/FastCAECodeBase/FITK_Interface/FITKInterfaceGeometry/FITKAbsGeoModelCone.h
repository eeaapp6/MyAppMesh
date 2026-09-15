/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelCone.h
 * @brief       圆锥建模命令类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-08
 *********************************************************************/

#ifndef  __FITKABSGEOMODELCONE_H__
#define  __FITKABSGEOMODELCONE_H__

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"

namespace Interface
{
    /**
     * @brief       圆锥建模命令类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-08
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCone : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelCone);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        explicit FITKAbsGeoModelCone() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        virtual ~FITKAbsGeoModelCone();

        /**
         * @brief       设置端面圆心坐标。
         * @param[in]   p1：圆心坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setLocation(double* p1);

        /**
         * @brief       获取端面圆心坐标。
         * @param[out]  p1：圆心坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void getLocation(double* p1);

        /**
         * @brief       设置轴线方向。
         * @param[in]   dir: 轴线方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setDirection(double* dir);

        /**
         * @brief       获取轴线方向。
         * @param[out]  dir: 轴线方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void getDirection(double* dir);

        /**
         * @brief       设置底面半径。
         * @param[in]   r: 底面半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setBottomRadius(double r);

        /**
         * @brief       获取底面半径。
         * @return      double
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double getBottomRadius() const;

        /**
         * @brief       设置顶面半径。
         * @param[in]   r: 顶面半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setTopRadius(double r);

        /**
         * @brief       获取顶面半径。
         * @return      double
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double getTopRadius() const;

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
         * @brief       获取几何命令类型。
         * @return      FITKGeoEnum::FITKGeometryComType
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    protected:
        /**
         * @brief       端面圆心坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _location[3] = { 0. ,0. ,0. };

        /**
         * @brief       轴线方向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _direction[3] = { 0. ,0. ,1. };

        /**
         * @brief       底面半径。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _bottomRadius{ 0 };

        /**
         * @brief       顶面半径。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _topRadius{ 0 };

        /**
         * @brief       高度。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double _length{ 0 };

    };
}

#endif // !__FITKABSGEOMODELCONE_H__
