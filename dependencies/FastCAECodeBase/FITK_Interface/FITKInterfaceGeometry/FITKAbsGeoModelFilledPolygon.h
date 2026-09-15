/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelFilledPolygon.h
 * @brief       填充多边形建模命令类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-09
 *********************************************************************/

#ifndef  __FITKABSGEMODELFILLEDPOLYGON_H__
#define  __FITKABSGEMODELFILLEDPOLYGON_H__

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"
#include <QList>
#include <array>

namespace Interface
{
    /**
     * @brief       填充多边形建模命令类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-09
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelFilledPolygon :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelFilledPolygon);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        explicit FITKAbsGeoModelFilledPolygon() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        virtual ~FITKAbsGeoModelFilledPolygon();

        /**
         * @brief       获取点列表。
         * @return      点列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        QList<std::array<double, 3>> points() const;

        /**
         * @brief       添加点。
         * @param[in]   x: X坐标
         * @param[in]   y: Y坐标
         * @param[in]   z: Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void addPoint(double x, double y, double z);

        /**
         * @brief       添加点。
         * @param[in]   xyz: 坐标数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void addPoint(std::array<double, 3> xyz);

        /**
         * @brief       清空所有点。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        void clearPoints();

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
         * @brief       点列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        QList<std::array<double, 3>> m_Points;

        /**
         * @brief       是否填充内部面。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        bool m_IsFilled{ true };
    };
}

#endif
