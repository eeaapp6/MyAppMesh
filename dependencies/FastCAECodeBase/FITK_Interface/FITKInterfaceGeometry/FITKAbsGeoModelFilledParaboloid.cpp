/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelFilledParaboloid.h"
#include <cmath>

namespace Interface
{
    FITKAbsGeoModelFilledParaboloid::~FITKAbsGeoModelFilledParaboloid()
    {

    }

    std::array<double, 3> FITKAbsGeoModelFilledParaboloid::origin() const
    {
        return m_Origin;
    }

    void FITKAbsGeoModelFilledParaboloid::setOrigin(double x, double y, double z)
    {
        m_Origin[0] = x;
        m_Origin[1] = y;
        m_Origin[2] = z;
    }

    void FITKAbsGeoModelFilledParaboloid::setOrigin(std::array<double, 3> xyz)
    {
        m_Origin = xyz;
    }

    std::array<double, 3> FITKAbsGeoModelFilledParaboloid::axisDirection() const
    {
        return m_AxisDirection;
    }

    void FITKAbsGeoModelFilledParaboloid::setAxisDirection(double x, double y, double z)
    {
        setAxisDirection({ x, y, z });
    }

    void FITKAbsGeoModelFilledParaboloid::setAxisDirection(std::array<double, 3> xyz)
    {
        double len = std::sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
        if (len > 1e-9)
        {
            m_AxisDirection[0] = xyz[0] / len;
            m_AxisDirection[1] = xyz[1] / len;
            m_AxisDirection[2] = xyz[2] / len;
        }
    }

    std::array<double, 3> FITKAbsGeoModelFilledParaboloid::xDirection() const
    {
        return m_XDirection;
    }

    void FITKAbsGeoModelFilledParaboloid::setXDirection(double x, double y, double z)
    {
        setXDirection({ x, y, z });
    }

    void FITKAbsGeoModelFilledParaboloid::setXDirection(std::array<double, 3> xyz)
    {
        double len = std::sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
        if (len > 1e-9)
        {
            m_XDirection[0] = xyz[0] / len;
            m_XDirection[1] = xyz[1] / len;
            m_XDirection[2] = xyz[2] / len;
        }
    }

    double FITKAbsGeoModelFilledParaboloid::startRadius() const
    {
        return m_StartRadius;
    }

    void FITKAbsGeoModelFilledParaboloid::setStartRadius(double radius)
    {
        m_StartRadius = radius;
    }

    double FITKAbsGeoModelFilledParaboloid::focalDepth() const
    {
        return m_FocalDepth;
    }

    void FITKAbsGeoModelFilledParaboloid::setFocalDepth(double depth)
    {
        m_FocalDepth = depth;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelFilledParaboloid::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTFilledParaboloid;
    }

}
