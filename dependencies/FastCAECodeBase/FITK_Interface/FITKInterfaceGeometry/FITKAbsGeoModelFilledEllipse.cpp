/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelFilledEllipse.h"
#include <cmath>

namespace Interface
{
    FITKAbsGeoModelFilledEllipse::~FITKAbsGeoModelFilledEllipse()
    {

    }

    std::array<double, 3> FITKAbsGeoModelFilledEllipse::origin() const
    {
        return m_Origin;
    }

    void FITKAbsGeoModelFilledEllipse::setOrigin(double x, double y, double z)
    {
        m_Origin[0] = x;
        m_Origin[1] = y;
        m_Origin[2] = z;
    }

    void FITKAbsGeoModelFilledEllipse::setOrigin(std::array<double, 3> xyz)
    {
        m_Origin = xyz;
    }

    std::array<double, 3> FITKAbsGeoModelFilledEllipse::xDirection() const
    {
        return m_XDirection;
    }

    std::array<double, 3> FITKAbsGeoModelFilledEllipse::yDirection() const
    {
        return m_YDirection;
    }

    void FITKAbsGeoModelFilledEllipse::setXDirection(double x, double y, double z)
    {
        setXDirection({ x, y, z });
    }

    void FITKAbsGeoModelFilledEllipse::setXDirection(std::array<double, 3> xyz)
    {
        double len = std::sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
        if (len > 1e-9)
        {
            m_XDirection[0] = xyz[0] / len;
            m_XDirection[1] = xyz[1] / len;
            m_XDirection[2] = xyz[2] / len;
        }
    }

    void FITKAbsGeoModelFilledEllipse::setYDirection(double x, double y, double z)
    {
        setYDirection({ x, y, z });
    }

    void FITKAbsGeoModelFilledEllipse::setYDirection(std::array<double, 3> xyz)
    {
        double len = std::sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
        if (len > 1e-9)
        {
            m_YDirection[0] = xyz[0] / len;
            m_YDirection[1] = xyz[1] / len;
            m_YDirection[2] = xyz[2] / len;
        }
    }

    double FITKAbsGeoModelFilledEllipse::xRadius() const
    {
        return m_XRadius;
    }

    void FITKAbsGeoModelFilledEllipse::setXRadius(double radius)
    {
        m_XRadius = radius;
    }

    double FITKAbsGeoModelFilledEllipse::yRadius() const
    {
        return m_YRadius;
    }

    void FITKAbsGeoModelFilledEllipse::setYRadius(double radius)
    {
        m_YRadius = radius;
    }

    bool FITKAbsGeoModelFilledEllipse::isFilled() const
    {
        return m_IsFilled;
    }

    void FITKAbsGeoModelFilledEllipse::setFilled(bool filled)
    {
        m_IsFilled = filled;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelFilledEllipse::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTFilledEllipse;
    }

}
