/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelSplitPart.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSplitPart::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTSplitPart;
    }

    void FITKAbsGeoModelSplitPart::setSourcePartId(int partId)
    {
        // 设置源部件ID。
        m_sourcePartId = partId;
    }

    int FITKAbsGeoModelSplitPart::getSourcePartId() const
    {
        return m_sourcePartId;
    }

    void FITKAbsGeoModelSplitPart::setSplitOrigin(double x, double y, double z)
    {
        // 设置分割平面中心坐标。
        m_origin[0] = x;
        m_origin[1] = y;
        m_origin[2] = z;
    }

    void FITKAbsGeoModelSplitPart::getSplitOrigin(double& x, double& y, double& z) const
    {
        x = m_origin[0];
        y = m_origin[1];
        z = m_origin[2];
    }

    void FITKAbsGeoModelSplitPart::setSplitPlaneType(FITKGeoEnum::PlaneType type)
    {
        // 设置分割平面位置类型。
        m_planeType = type;
    }

    FITKGeoEnum::PlaneType FITKAbsGeoModelSplitPart::getSplitPlaneType() const
    {
        return m_planeType;
    }

    void FITKAbsGeoModelSplitPart::setRotateX(double angle)
    {
        // 设置X方向旋转角度。
        m_rotateX = angle;
    }

    double FITKAbsGeoModelSplitPart::getRotateX() const
    {
        return m_rotateX;
    }

    void FITKAbsGeoModelSplitPart::setRotateY(double angle)
    {
        // 设置Y方向旋转角度。
        m_rotateY = angle;
    }

    double FITKAbsGeoModelSplitPart::getRotateY() const
    {
        return m_rotateY;
    }

    void FITKAbsGeoModelSplitPart::setRotateZ(double angle)
    {
        // 设置Z方向旋转角度。
        m_rotateZ = angle;
    }

    double FITKAbsGeoModelSplitPart::getRotateZ() const
    {
        return m_rotateZ;
    }
}
