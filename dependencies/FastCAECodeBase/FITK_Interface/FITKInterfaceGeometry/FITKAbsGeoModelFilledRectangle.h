/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelFilledRectangle.h
 * @brief       填充矩形建模命令类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-08
 *********************************************************************/

#ifndef  __FITKABSGEMODELFILLEDRECTANGLE_H__
#define  __FITKABSGEMODELFILLEDRECTANGLE_H__

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"
#include <array>

namespace Interface
{
    /**
     * @brief       填充矩形建模命令类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-08
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelFilledRectangle :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelFilledRectangle);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        explicit FITKAbsGeoModelFilledRectangle() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        virtual ~FITKAbsGeoModelFilledRectangle();

        /**
         * @brief       获取原点坐标。
         * @return      原点坐标数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        std::array<double, 3> origin() const;

        /**
         * @brief       设置原点坐标。
         * @param[in]   x: X坐标
         * @param[in]   y: Y坐标
         * @param[in]   z: Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setOrigin(double x, double y, double z);

        /**
         * @brief       设置原点坐标。
         * @param[in]   xyz: 原点坐标数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setOrigin(std::array<double, 3> xyz);

        /**
         * @brief       获取X轴方向。
         * @return      X轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        std::array<double, 3> xDirection() const;

        /**
         * @brief       获取Y轴方向。
         * @return      Y轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        std::array<double, 3> yDirection() const;

        /**
         * @brief       设置X轴方向。
         * @param[in]   x: X方向向量的x值
         * @param[in]   y: X方向向量的y值
         * @param[in]   z: X方向向量的z值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setXDirection(double x, double y, double z);

        /**
         * @brief       设置X轴方向。
         * @param[in]   xyz: X轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setXDirection(std::array<double, 3> xyz);

        /**
         * @brief       设置Y轴方向。
         * @param[in]   x: Y方向向量的x值
         * @param[in]   y: Y方向向量的y值
         * @param[in]   z: Y方向向量的z值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setYDirection(double x, double y, double z);

        /**
         * @brief       设置Y轴方向。
         * @param[in]   xyz: Y轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setYDirection(std::array<double, 3> xyz);

        /**
         * @brief       获取X方向边长。
         * @return      double
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double xLength() const;

        /**
         * @brief       设置X方向边长。
         * @param[in]   length: X方向边长
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setXLength(double length);

        /**
         * @brief       获取Y方向边长。
         * @return      double
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double yLength() const;

        /**
         * @brief       设置Y方向边长。
         * @param[in]   length: Y方向边长
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setYLength(double length);

        /**
         * @brief       获取是否填充内部面。
         * @return      true表示填充，false表示仅创建边框
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        bool isFilled() const;

        /**
         * @brief       设置是否填充内部面。
         * @param[in]   filled: true表示填充，false表示仅创建边框
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        void setFilled(bool filled);

        /**
         * @brief       获取几何命令类型。
         * @return      FITKGeoEnum::FITKGeometryComType
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    protected:
        /**
         * @brief       原点坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        std::array<double, 3> m_Origin{ { 0, 0, 0 } };

        /**
         * @brief       X轴方向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        std::array<double, 3> m_XDirection{ { 1, 0, 0 } };

        /**
         * @brief       Y轴方向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        std::array<double, 3> m_YDirection{ { 0, 1, 0 } };

        /**
         * @brief       X方向边长。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double m_XLength{ 0 };

        /**
         * @brief       Y方向边长。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        double m_YLength{ 0 };

        /**
         * @brief       是否填充内部面。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        bool m_IsFilled{ true };

    };
}

#endif
