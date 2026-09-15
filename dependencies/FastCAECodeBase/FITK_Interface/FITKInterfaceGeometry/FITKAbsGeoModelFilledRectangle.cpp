/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelFilledRectangle.h"
#include <cmath>
namespace Interface
{

    FITKAbsGeoModelFilledRectangle::~FITKAbsGeoModelFilledRectangle()
    {

    }

    std::array<double, 3> FITKAbsGeoModelFilledRectangle::origin() const
    {
        return m_Origin;
    }

    void FITKAbsGeoModelFilledRectangle::setOrigin(double x, double y, double z)
    {
        m_Origin[0] = x;
        m_Origin[1] = y;
        m_Origin[2] = z;
    }

    void FITKAbsGeoModelFilledRectangle::setOrigin(std::array<double, 3> xyz)
    {
        m_Origin = xyz;
    }

    std::array<double, 3> FITKAbsGeoModelFilledRectangle::xDirection() const
    {
        return m_XDirection;
    }

    std::array<double, 3> FITKAbsGeoModelFilledRectangle::yDirection() const
    {
        return m_YDirection;
    }

    void FITKAbsGeoModelFilledRectangle::setXDirection(double x, double y, double z)
    {
        setXDirection({ x, y, z });
    }

    void FITKAbsGeoModelFilledRectangle::setXDirection(std::array<double, 3> xyz)
    {
        double len = std::sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
        if (len > 1e-9)
        {
            m_XDirection[0] = xyz[0] / len;
            m_XDirection[1] = xyz[1] / len;
            m_XDirection[2] = xyz[2] / len;
        }
    }

    void FITKAbsGeoModelFilledRectangle::setYDirection(double x, double y, double z)
    {
        setYDirection({ x, y, z });
    }

    void FITKAbsGeoModelFilledRectangle::setYDirection(std::array<double, 3> xyz)
    {
        double len = std::sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
        if (len > 1e-9)
        {
            m_YDirection[0] = xyz[0] / len;
            m_YDirection[1] = xyz[1] / len;
            m_YDirection[2] = xyz[2] / len;
        }
    }

    double FITKAbsGeoModelFilledRectangle::xLength() const
    {
        return m_XLength;
    }

    void FITKAbsGeoModelFilledRectangle::setXLength(double length)
    {
        m_XLength = length;
    }

    double FITKAbsGeoModelFilledRectangle::yLength() const
    {
        return m_YLength;
    }

    void FITKAbsGeoModelFilledRectangle::setYLength(double length)
    {
        m_YLength = length;
    }

    bool FITKAbsGeoModelFilledRectangle::isFilled() const
    {
        return m_IsFilled;
    }

    void FITKAbsGeoModelFilledRectangle::setFilled(bool filled)
    {
        m_IsFilled = filled;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelFilledRectangle::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTFilledRectangle;
    }

}
