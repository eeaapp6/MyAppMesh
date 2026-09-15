/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKAbsGeoOffsetFace.h
 * @brief   偏移面抽象接口类。
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2026-07-06
 *********************************************************************/

#ifndef  FITKABSGEOOFFSETFACE_H
#define  FITKABSGEOOFFSETFACE_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"


namespace Interface
{
    /**
     * @brief       偏移面抽象类。
     *              支持两种偏移模式：基于目标面的相对偏移、直接距离偏移。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-07-06
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoOffsetFace : public FITKAbsGeoCommand
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbsGeoOffsetFace);

    public:
        /**
         * @brief       偏移模式枚举。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        enum OffsetMode
        {
            DirectDistance = 0,     ///< 模式二：直接距离偏移
            RelativeToTarget = 1,   ///< 模式一：基于目标面的相对偏移
        };
        Q_ENUM(OffsetMode);

        /**
         * @brief       相对偏移距离计算方式枚举。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        enum RelativeOffsetMode
        {
            ROHalfDistance = 0,         ///< 选项1：自动设置为源面与目标面距离的50%
            RONearestPointRatio = 1,    ///< 选项2：基于最近点距离的自定义比例
            ROFarthestPointRatio = 2,   ///< 选项3：基于最远点距离的自定义比例
        };
        Q_ENUM(RelativeOffsetMode);

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        FITKAbsGeoOffsetFace() = default;

        /**
         * @brief       析构函数（纯虚，抽象类）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        virtual ~FITKAbsGeoOffsetFace() = 0;

        /**
         * @brief       获取几何命令类型。
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置源面列表。
         * @param[in]   faces：源面虚拟拓扑列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        void setSourceFaces(const QList<VirtualShape>& faces);

        /**
         * @brief       获取源面列表。
         * @return      源面虚拟拓扑列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        QList<VirtualShape> sourceFaces() const;

        /**
         * @brief       设置目标面（用于相对偏移模式）。
         * @param[in]   face：目标面虚拟拓扑
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        void setTargetFace(VirtualShape face);

        /**
         * @brief       获取目标面。
         * @return      目标面虚拟拓扑
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        VirtualShape targetFace() const;

        /**
         * @brief       设置偏移模式。
         * @param[in]   mode：偏移模式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        void setOffsetMode(OffsetMode mode);

        /**
         * @brief       获取偏移模式。
         * @return      偏移模式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        OffsetMode offsetMode() const;

        /**
         * @brief       设置直接偏移距离（用于DirectDistance模式，支持正负值）。
         * @param[in]   distance：偏移距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        void setOffsetDistance(double distance);

        /**
         * @brief       获取直接偏移距离。
         * @return      偏移距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        double offsetDistance() const;

        /**
         * @brief       设置相对偏移子模式。
         * @param[in]   mode：相对偏移子模式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        void setRelativeMode(RelativeOffsetMode mode);

        /**
         * @brief       获取相对偏移子模式。
         * @return      相对偏移子模式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        RelativeOffsetMode relativeMode() const;

        /**
         * @brief       设置比例值（0.0~1.0，用于NearestPointRatio/FarthestPointRatio）。
         * @param[in]   ratio：比例值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        void setRatio(double ratio);

        /**
         * @brief       获取比例值。
         * @return      比例值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        double ratio() const;

    protected:
        /**
         * @brief       源面虚拟拓扑列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        QList<VirtualShape> m_sourceFaces;

        /**
         * @brief       目标面虚拟拓扑（相对偏移模式使用）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        VirtualShape m_targetFace{};

        /**
         * @brief       偏移模式。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        OffsetMode m_offsetMode = DirectDistance;

        /**
         * @brief       直接偏移距离。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        double m_offsetDistance = 0.0;

        /**
         * @brief       相对偏移子模式。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        RelativeOffsetMode m_relativeMode = ROHalfDistance;

        /**
         * @brief       比例值（0.0~1.0）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        double m_ratio = 0.5;
    };
}

#endif // !FITKABSGEOOFFSETFACE_H
