/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractRWall.h"

namespace Radioss
{
    FITKAbstractRWall::~FITKAbstractRWall() {}

    int FITKAbstractRWall::getNodeId() const { return m_nodeId; }
    void FITKAbstractRWall::setNodeId(int v) { m_nodeId = v; }

    FITKAbstractRWall::RWallSliding FITKAbstractRWall::getSlidingType() const { return m_slidingType; }
    void FITKAbstractRWall::setSlidingType(RWallSliding v) { m_slidingType = v; }

    int FITKAbstractRWall::getNodeGroupId1() const { return m_nodeGroupId1; }
    void FITKAbstractRWall::setNodeGroupId1(int v)
    {
        m_nodeGroupId1 = v; 
    }

    int FITKAbstractRWall::getNodeGroupId2() const { return m_nodeGroupId2; }
    void FITKAbstractRWall::setNodeGroupId2(int v) { m_nodeGroupId2 = v; }

    double FITKAbstractRWall::getDSearch() const { return m_DSearch; }
    void FITKAbstractRWall::setDSearch(double v) { m_DSearch = v; }

    double FITKAbstractRWall::getFrictionCoef() const { return m_frictionCoef; }
    void FITKAbstractRWall::setFrictionCoef(double v) { m_frictionCoef = v; }

    double FITKAbstractRWall::getDiameter() const { return m_diameter; }
    void FITKAbstractRWall::setDiameter(double v) { m_diameter = v; }

    double FITKAbstractRWall::getFilteringFactor() const { return m_filteringFactor; }
    void FITKAbstractRWall::setFilteringFactor(double v) { m_filteringFactor = v; }

    int FITKAbstractRWall::getFilteringFlag() const { return m_filteringFlag; }
    void FITKAbstractRWall::setFilteringFlag(int v) { m_filteringFlag = v; }

    void FITKAbstractRWall::setMCoordinates(double x, double y, double z)
    {
        m_coordinatesM[0] = x;
        m_coordinatesM[1] = y;
        m_coordinatesM[2] = z;
    }

    void FITKAbstractRWall::getMCoordinates(double& x, double& y, double& z) const
    {
        x = m_coordinatesM[0];
        y = m_coordinatesM[1];
        z = m_coordinatesM[2];
    }

    void FITKAbstractRWall::setM1Coordinates(double x, double y, double z)
    {
        m_coordinatesM1[0] = x;
        m_coordinatesM1[1] = y;
        m_coordinatesM1[2] = z;
    }

    void FITKAbstractRWall::getM1Coordinates(double& x, double& y, double& z) const
    {
        x = m_coordinatesM1[0];
        y = m_coordinatesM1[1];
        z = m_coordinatesM1[2];
    }
}
