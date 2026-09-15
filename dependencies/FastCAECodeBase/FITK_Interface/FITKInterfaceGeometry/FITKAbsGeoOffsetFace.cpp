/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoOffsetFace.h"

namespace Interface
{
    // 析构函数。
    //{
    FITKAbsGeoOffsetFace::~FITKAbsGeoOffsetFace() = default;
    //}

    // 获取几何命令类型。
    //{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoOffsetFace::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTOffsetFace;
    }
    //}

    // 源面列表。
    //@{
    void FITKAbsGeoOffsetFace::setSourceFaces(const QList<VirtualShape>& faces)
    {
        // 保存源面虚拟拓扑列表。
        m_sourceFaces = faces;
    }

    QList<VirtualShape> FITKAbsGeoOffsetFace::sourceFaces() const
    {
        return m_sourceFaces;
    }
    //@}

    // 目标面。
    //@{
    void FITKAbsGeoOffsetFace::setTargetFace(VirtualShape face)
    {
        // 保存目标面虚拟拓扑，仅在相对偏移模式下使用。
        m_targetFace = face;
    }

    VirtualShape FITKAbsGeoOffsetFace::targetFace() const
    {
        return m_targetFace;
    }
    //@}

    // 偏移模式。
    //@{
    void FITKAbsGeoOffsetFace::setOffsetMode(OffsetMode mode)
    {
        // 设置偏移模式：DirectDistance（直接距离）或 RelativeToTarget（相对偏标）。
        m_offsetMode = mode;
    }

    FITKAbsGeoOffsetFace::OffsetMode FITKAbsGeoOffsetFace::offsetMode() const
    {
        return m_offsetMode;
    }
    //@}

    // 直接偏移距离。
    //@{
    void FITKAbsGeoOffsetFace::setOffsetDistance(double distance)
    {
        // 设置直接偏移距离，正值沿法线方向，负值沿反方向。
        m_offsetDistance = distance;
    }

    double FITKAbsGeoOffsetFace::offsetDistance() const
    {
        return m_offsetDistance;
    }
    //@}

    // 相对偏移子模式。
    //@{
    void FITKAbsGeoOffsetFace::setRelativeMode(RelativeOffsetMode mode)
    {
        // 设置相对偏移子模式：半距离、最近点比例或最远点比例。
        m_relativeMode = mode;
    }

    FITKAbsGeoOffsetFace::RelativeOffsetMode FITKAbsGeoOffsetFace::relativeMode() const
    {
        return m_relativeMode;
    }
    //@}

    // 比例值。
    //@{
    void FITKAbsGeoOffsetFace::setRatio(double ratio)
    {
        // 保存比例值，范围0.0~1.0，用于NearestPointRatio/FarthestPointRatio模式。
        m_ratio = ratio;
    }

    double FITKAbsGeoOffsetFace::ratio() const
    {
        return m_ratio;
    }
    //@}
}
