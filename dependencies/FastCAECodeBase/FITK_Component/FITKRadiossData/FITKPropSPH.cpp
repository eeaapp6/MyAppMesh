/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPropSPH.h"

namespace Radioss
{
    FITKPropSPH::FITKPropSPH()
    {
        _value = new FITKPropSPHValue;
    }

    FITKPropSPH::~FITKPropSPH()
    {
        FITKPropSPHValue* value = _value.getObjectPtr();
        if (value) delete value;
    }

    FITKPropAbstract::RadPropType FITKPropSPH::getRadPropType()
    {
        return FITKPropAbstract::RadPropType::SPH;
    }

    FITKPropSPHValue * FITKPropSPH::getValue()
    {
        return _value.getObjectPtr();
    }


    double FITKPropSPHValue::getMP() const
    {
        //Get Mass of the particles.
        return m_mp;
    }

    void FITKPropSPHValue::setMP(double value)
    {
        //Set Mass of the particles.
        m_mp = value;
    }

    double FITKPropSPHValue::getQA() const
    {
        //Get Quadratic bulk viscosity.
        return m_qa;
    }

    void FITKPropSPHValue::setQA(double value)
    {
        //Set Quadratic bulk viscosity.
        m_qa = value;
    }

    double FITKPropSPHValue::getQB() const
    {
        //Get Linear bulk viscosity.
        return m_qb;
    }

    void FITKPropSPHValue::setQB(double value)
    {
        // Set Linear bulk viscosity.
        m_qb = value;
    }

    double FITKPropSPHValue::getACS() const
    {
        //Get Conservative smoothing coefficient.
        return m_acs;
    }

    void FITKPropSPHValue::setACS(double value)
    {
        //Set Conservative smoothing coefficient.
        m_acs = value;
    }

    int FITKPropSPHValue::getSkewID() const
    {
        //Get skew_ID
        return m_skew_ID;
    }

    void FITKPropSPHValue::setSkewID(int value)
    {
        //Set skew_ID
        m_skew_ID = value;
    }

    int FITKPropSPHValue::getHID() const
    {
        //Get Smoothing length change based on volume.
        return m_h_ID;
    }

    void FITKPropSPHValue::setHID(int value)
    {
        //Get Smoothing length change based on volume.
        m_h_ID = value;
    }

    int FITKPropSPHValue::getOrderID() const
    {
        //Get SPH correction order.
        return m_order;
    }

    void FITKPropSPHValue::setOrderID(int value)
    {
        //Set SPH correction order.
        m_order = value;
    }

    double FITKPropSPHValue::getH() const
    {
        //Get Smoothing length.
        return m_h;
    }

    void FITKPropSPHValue::setH(double value)
    {
        //Set Smoothing length.
        m_h = value;
    }

    double FITKPropSPHValue::getStab() const
    {
        //Get Coefficient for solving tensile instability.
        return m_stab;
    }

    void FITKPropSPHValue::setStab(double value)
    {
        //Set Coefficient for solving tensile instability.
        m_stab = value;
    }

}