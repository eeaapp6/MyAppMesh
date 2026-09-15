/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelFilledEllipse.h
 * @brief       填充椭圆建模命令类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-09
 *********************************************************************/

#ifndef  __FITKABSGEMODELFILLEDELLIPSE_H__
#define  __FITKABSGEMODELFILLEDELLIPSE_H__

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"
#include <array>

namespace Interface
{
    /**
     * @brief       填充椭圆建模命令类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-09
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelFilledEllipse :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelFilledEllipse);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        explicit FITKAbsGeoModelFilledEllipse() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        virtual ~FITKAbsGeoModelFilledEllipse();

        /**
         * @brief       获取原点坐标。
         * @return      原点坐标数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> origin() const;

        /**
         * @brief       设置原点坐标。
         * @param[in]   x: X坐标
         * @param[in]   y: Y坐标
         * @param[in]   z: Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setOrigin(double x, double y, double z);

        /**
         * @brief       设置原点坐标。
         * @param[in]   xyz: 原点坐标数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setOrigin(std::array<double, 3> xyz);

        /**
         * @brief       获取长轴方向。
         * @return      长轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> xDirection() const;

        /**
         * @brief       获取短轴方向。
         * @return      短轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> yDirection() const;

        /**
         * @brief       设置长轴方向。
         * @param[in]   x: 长轴方向向量的x值
         * @param[in]   y: 长轴方向向量的y值
         * @param[in]   z: 长轴方向向量的z值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setXDirection(double x, double y, double z);

        /**
         * @brief       设置长轴方向。
         * @param[in]   xyz: 长轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setXDirection(std::array<double, 3> xyz);

        /**
         * @brief       设置短轴方向。
         * @param[in]   x: 短轴方向向量的x值
         * @param[in]   y: 短轴方向向量的y值
         * @param[in]   z: 短轴方向向量的z值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setYDirection(double x, double y, double z);

        /**
         * @brief       设置短轴方向。
         * @param[in]   xyz: 短轴方向数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setYDirection(std::array<double, 3> xyz);

        /**
         * @brief       获取长轴半径。
         * @return      长轴半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        double xRadius() const;

        /**
         * @brief       设置长轴半径。
         * @param[in]   radius: 长轴半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setXRadius(double radius);

        /**
         * @brief       获取短轴半径。
         * @return      短轴半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        double yRadius() const;

        /**
         * @brief       设置短轴半径。
         * @param[in]   radius: 短轴半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setYRadius(double radius);

        /**
         * @brief       获取是否填充内部面。
         * @return      true表示填充，false表示仅创建边框
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        bool isFilled() const;

        /**
         * @brief       设置是否填充内部面。
         * @param[in]   filled: true表示填充，false表示仅创建边框
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void setFilled(bool filled);

        /**
         * @brief       获取几何命令类型。
         * @return      FITKGeoEnum::FITKGeometryComType
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    protected:
        /**
         * @brief       原点坐标（圆心）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> m_Origin{ { 0, 0, 0 } };

        /**
         * @brief       长轴方向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> m_XDirection{ { 1, 0, 0 } };

        /**
         * @brief       短轴方向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        std::array<double, 3> m_YDirection{ { 0, 1, 0 } };

        /**
         * @brief       长轴半径。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        double m_XRadius{ 0 };

        /**
         * @brief       短轴半径。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        double m_YRadius{ 0 };

        /**
         * @brief       是否填充内部面。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        bool m_IsFilled{ true };
    };
}

#endif // !__FITKABSGEMODELFILLEDELLIPSE_H__
