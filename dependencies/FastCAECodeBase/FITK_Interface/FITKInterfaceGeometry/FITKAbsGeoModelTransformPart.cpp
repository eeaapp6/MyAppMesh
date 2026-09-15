/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelTransformPart.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelTranslatePart::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTTranslatePart;
    }

    void FITKAbsGeoModelTranslatePart::setTranslateFrom(double x, double y, double z)
    {
        // 设置平移起点坐标。
        m_from[0] = x;
        m_from[1] = y;
        m_from[2] = z;
    }

    void FITKAbsGeoModelTranslatePart::getTranslateFrom(double& x, double& y, double& z) const
    {
        x = m_from[0];
        y = m_from[1];
        z = m_from[2];
    }

    void FITKAbsGeoModelTranslatePart::setTranslateTo(double x, double y, double z)
    {
        // 设置平移终点坐标。
        m_to[0] = x;
        m_to[1] = y;
        m_to[2] = z;
    }

    void FITKAbsGeoModelTranslatePart::getTranslateTo(double& x, double& y, double& z) const
    {
        x = m_to[0];
        y = m_to[1];
        z = m_to[2];
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRotatePart::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTRotatePart;
    }

    void FITKAbsGeoModelRotatePart::setRotateOrigin(double x, double y, double z)
    {
        // 设置旋转中心坐标。
        m_origin[0] = x;
        m_origin[1] = y;
        m_origin[2] = z;
    }

    void FITKAbsGeoModelRotatePart::getRotateOrigin(double& x, double& y, double& z) const
    {
        x = m_origin[0];
        y = m_origin[1];
        z = m_origin[2];
    }

    void FITKAbsGeoModelRotatePart::setRotateDirection(double x, double y, double z)
    {
        // 设置旋转轴方向向量。
        m_direction[0] = x;
        m_direction[1] = y;
        m_direction[2] = z;
    }

    void FITKAbsGeoModelRotatePart::getRotateDirection(double& x, double& y, double& z) const
    {
        x = m_direction[0];
        y = m_direction[1];
        z = m_direction[2];
    }

    void FITKAbsGeoModelRotatePart::setRotateAngle(double angle)
    {
        // 设置旋转角度。
        m_angle = angle;
    }

    double FITKAbsGeoModelRotatePart::getRotateAngle() const
    {
        return m_angle;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelScalePart::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTScalePart;
    }

    void FITKAbsGeoModelScalePart::setScaleOrigin(double x, double y, double z)
    {
        // 设置缩放中心坐标。
        m_origin[0] = x;
        m_origin[1] = y;
        m_origin[2] = z;
    }

    void FITKAbsGeoModelScalePart::getScaleOrigin(double& x, double& y, double& z) const
    {
        x = m_origin[0];
        y = m_origin[1];
        z = m_origin[2];
    }

    void FITKAbsGeoModelScalePart::setScaleFactor(double factor)
    {
        // 设置缩放倍率。
        m_scaleFactor = factor;
    }

    double FITKAbsGeoModelScalePart::getScaleFactor() const
    {
        return m_scaleFactor;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelMirrorPart::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTMirrorPart;
    }

    void FITKAbsGeoModelMirrorPart::setMirrorOrigin(double x, double y, double z)
    {
        // 设置镜像平面中心坐标。
        m_origin[0] = x;
        m_origin[1] = y;
        m_origin[2] = z;
    }

    void FITKAbsGeoModelMirrorPart::getMirrorOrigin(double& x, double& y, double& z) const
    {
        x = m_origin[0];
        y = m_origin[1];
        z = m_origin[2];
    }

    void FITKAbsGeoModelMirrorPart::setMirrorPlaneType(FITKGeoEnum::PlaneType type)
    {
        // 设置镜像平面位置类型。
        m_planeType = type;
    }

    FITKGeoEnum::PlaneType FITKAbsGeoModelMirrorPart::getMirrorPlaneType() const
    {
        return m_planeType;
    }

    void FITKAbsGeoModelMirrorPart::setRotateX(double angle)
    {
        // 设置X方向旋转角度。
        m_rotateX = angle;
    }

    double FITKAbsGeoModelMirrorPart::getRotateX() const
    {
        return m_rotateX;
    }

    void FITKAbsGeoModelMirrorPart::setRotateY(double angle)
    {
        // 设置Y方向旋转角度。
        m_rotateY = angle;
    }

    double FITKAbsGeoModelMirrorPart::getRotateY() const
    {
        return m_rotateY;
    }

    void FITKAbsGeoModelMirrorPart::setRotateZ(double angle)
    {
        // 设置Z方向旋转角度。
        m_rotateZ = angle;
    }

    double FITKAbsGeoModelMirrorPart::getRotateZ() const
    {
        return m_rotateZ;
    }

    void FITKAbsGeoModelMirrorPart::setKeepOrigin(bool keep)
    {
        // 设置是否保留原始几何体。
        m_keepOrigin = keep;
    }

    bool FITKAbsGeoModelMirrorPart::getKeepOrigin() const
    {
        return m_keepOrigin;
    }

    void FITKAbsGeoModelMirrorPart::setCustomPlaneNormal(double x, double y, double z)
    {
        // 设置自定义平面法向量。
        m_customPlaneNormal[0] = x;
        m_customPlaneNormal[1] = y;
        m_customPlaneNormal[2] = z;
    }

    void FITKAbsGeoModelMirrorPart::getCustomPlaneNormal(double& x, double& y, double& z) const
    {
        x = m_customPlaneNormal[0];
        y = m_customPlaneNormal[1];
        z = m_customPlaneNormal[2];
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRectangularPatternPart::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTRectangularPatternPart;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCircularPatternPart::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTCircularPatternPart;
    }
}
