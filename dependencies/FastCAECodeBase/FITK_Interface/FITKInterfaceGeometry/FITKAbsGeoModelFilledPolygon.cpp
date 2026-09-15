/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelFilledPolygon.h"

namespace Interface
{
    FITKAbsGeoModelFilledPolygon::~FITKAbsGeoModelFilledPolygon()
    {

    }

    QList<std::array<double, 3>> FITKAbsGeoModelFilledPolygon::points() const
    {
        return m_Points;
    }

    void FITKAbsGeoModelFilledPolygon::addPoint(double x, double y, double z)
    {
        m_Points.append({ x, y, z });
    }

    void FITKAbsGeoModelFilledPolygon::addPoint(std::array<double, 3> xyz)
    {
        m_Points.append(xyz);
    }

    void FITKAbsGeoModelFilledPolygon::clearPoints()
    {
        m_Points.clear();
    }

    bool FITKAbsGeoModelFilledPolygon::isFilled() const
    {
        return m_IsFilled;
    }

    void FITKAbsGeoModelFilledPolygon::setFilled(bool filled)
    {
        m_IsFilled = filled;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelFilledPolygon::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTFilledPolygon;
    }

}
