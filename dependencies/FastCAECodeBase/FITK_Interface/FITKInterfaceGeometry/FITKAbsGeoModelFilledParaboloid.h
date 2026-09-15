/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelFilledParaboloid.h
 * @brief       填充抛物面建模命令类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-09
 *********************************************************************/

#ifndef  __FITKABSGEMODELFILLEDPARABOLOID_H__
#define  __FITKABSGEMODELFILLEDPARABOLOID_H__

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"
#include <array>

namespace Interface
{
    /**
     * @brief       填充抛物面建模命令类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-09
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelFilledParaboloid :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelFilledParaboloid);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        explicit FITKAbsGeoModelFilledParaboloid() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        virtual ~FITKAbsGeoModelFilledParaboloid();

        /**
         * @brief       获取顶点坐标。
         * @return      顶点坐标数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> origin() const;

        /**
         * @brief       设置顶点坐标。
         * @param[in]   x: X坐标
         * @param[in]   y: Y坐标
         * @param[in]   z: Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setOrigin(double x, double y, double z);

        /**
         * @brief       设置顶点坐标。
         * @param[in]   xyz: 顶点坐标数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setOrigin(std::array<double, 3> xyz);

        /**
         * @brief       获取轴方向。
         * @return      轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> axisDirection() const;

        /**
         * @brief       设置轴方向。
         * @param[in]   x: 轴方向向量的x值
         * @param[in]   y: 轴方向向量的y值
         * @param[in]   z: 轴方向向量的z值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setAxisDirection(double x, double y, double z);

        /**
         * @brief       设置轴方向。
         * @param[in]   xyz: 轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setAxisDirection(std::array<double, 3> xyz);

        /**
         * @brief       获取X轴方向（定义抛物线起始位置）。
         * @return      X轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> xDirection() const;

        /**
         * @brief       设置X轴方向（定义抛物线起始位置）。
         * @param[in]   x: X轴方向向量的x值
         * @param[in]   y: X轴方向向量的y值
         * @param[in]   z: X轴方向向量的z值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setXDirection(double x, double y, double z);

        /**
         * @brief       设置X轴方向（定义抛物线起始位置）。
         * @param[in]   xyz: X轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setXDirection(std::array<double, 3> xyz);

        /**
         * @brief       获取起始位置半径。
         * @return      起始位置半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        double startRadius() const;

        /**
         * @brief       设置起始位置半径。
         * @param[in]   radius: 起始位置半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setStartRadius(double radius);

        /**
         * @brief       获取焦点深度（二分之一高度）。
         * @return      焦点深度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        double focalDepth() const;

        /**
         * @brief       设置焦点深度（二分之一高度）。
         * @param[in]   depth: 焦点深度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setFocalDepth(double depth);

        /**
         * @brief       获取几何命令类型。
         * @return      FITKGeoEnum::FITKGeometryComType
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    protected:
        /**
         * @brief       顶点坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> m_Origin{ { 0, 0, 0 } };

        /**
         * @brief       轴方向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> m_AxisDirection{ { 0, 0, 1 } };

        /**
         * @brief       X轴方向（定义抛物线起始位置）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> m_XDirection{ { 1, 0, 0 } };

        /**
         * @brief       起始位置半径。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        double m_StartRadius{ 0 };

        /**
         * @brief       焦点深度（二分之一高度）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        double m_FocalDepth{ 0 };
    };
}

#endif
