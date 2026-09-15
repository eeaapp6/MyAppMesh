/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKConnectionRBODY.h"

namespace Radioss
{
    FITKConnectionRBODY::~FITKConnectionRBODY()
    {
    }

    FITKAbstractConnection::ConnectionType FITKConnectionRBODY::getType() const
    {
        return ConnectionType::Connection_RBODY;
    }

    void FITKConnectionRBODY::setMainNodeID(int nodeID)
    {
        m_mainNodeID = nodeID;
    }

    int FITKConnectionRBODY::getMainNodeID()
    {
        return m_mainNodeID;
    }

    void FITKConnectionRBODY::setSensorID(int sensorID)
    {
        m_sensorID = sensorID;
    }

    int FITKConnectionRBODY::getSensorID()
    {
        return m_sensorID;
    }

    void FITKConnectionRBODY::setSystemID(int sysID)
    {
        m_systemID = sysID;
    }

    int FITKConnectionRBODY::getSystemID()
    {
        return m_systemID;
    }

    void FITKConnectionRBODY::setInertiaType(RbodyIspherType type)
    {
        m_inertiaType = type;
    }

    FITKConnectionRBODY::RbodyIspherType FITKConnectionRBODY::getInertiaType()
    {
        return m_inertiaType;
    }

    void FITKConnectionRBODY::setMass(double mass)
    {
        m_mass = mass;
    }

    double FITKConnectionRBODY::getMass()
    {
        return m_mass;
    }

    void FITKConnectionRBODY::setSecondaryNodesGroupID(int nodeSetID)
    {
        m_nodeSetID = nodeSetID;
    }

    int FITKConnectionRBODY::getSecondaryNodesGroupID()
    {
        return m_nodeSetID;
    }

    void FITKConnectionRBODY::setDeactivationRigidWall(bool isDeact)
    {
        m_isDeactivationRigidWall = isDeact;
    }

    bool FITKConnectionRBODY::getDeactivationRigidWall()
    {
        return m_isDeactivationRigidWall;
    }

    void FITKConnectionRBODY::setGravityCenterComputation(GravityCenterComputationType type)
    {
        m_gravityCenter = type;
    }

    FITKConnectionRBODY::GravityCenterComputationType FITKConnectionRBODY::getGravityCenterComputation()
    {
        return m_gravityCenter;
    }

    void FITKConnectionRBODY::setEnvelopeSurface(int surfID)
    {
        m_surfID = surfID;
    }

    int FITKConnectionRBODY::getEnvelopeSurface()
    {
        return m_surfID;
    }

    void FITKConnectionRBODY::setInertiaValue(double * data)
    {
        if (!data) return;
        m_inertiaValue[0] = data[0];
        m_inertiaValue[1] = data[1];
        m_inertiaValue[2] = data[2];
        m_inertiaValue[3] = data[3];
        m_inertiaValue[4] = data[4];
        m_inertiaValue[5] = data[5];
    }

    void FITKConnectionRBODY::getInertiaValue(double * data)
    {
        if (!data) return;
        data[0] = m_inertiaValue[0];
        data[1] = m_inertiaValue[1];
        data[2] = m_inertiaValue[2];
        data[3] = m_inertiaValue[3];
        data[4] = m_inertiaValue[4];
        data[5] = m_inertiaValue[5];
    }

    void FITKConnectionRBODY::setDomainDecomposition(bool isDom)
    {
        m_isDomainDecomposition = isDom;
    }

    bool FITKConnectionRBODY::getDomainDecomposition()
    {
        return m_isDomainDecomposition;
    }

    void FITKConnectionRBODY::setIfail(bool is)
    {
        m_Ifail = is;
    }

    bool FITKConnectionRBODY::getIfail()
    {
        return m_Ifail;
    }

    RBODY_OptionData & FITKConnectionRBODY::getOptionData()
    {
        return _optionData;
    }

}