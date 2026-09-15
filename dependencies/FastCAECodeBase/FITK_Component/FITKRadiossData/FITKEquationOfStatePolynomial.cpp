/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKEquationOfStatePolynomial.h"

namespace Radioss
{
    FITKEquationOfStatePolynomial::FITKEquationOfStatePolynomial()
    {
    }

    FITKEquationOfStatePolynomial::~FITKEquationOfStatePolynomial()
    {
    }

    FITKAbstractEquationOfState::EOS_Type FITKEquationOfStatePolynomial::getEOSType()
    {
        return EOS_Type::Polynomial;
    }

    void FITKEquationOfStatePolynomial::setC0(double value)
    {
        //Set Hydrodynamic pressure law coefficient
        m_c0 = value;
    }

    double FITKEquationOfStatePolynomial::getC0()
    {
        //Get Hydrodynamic pressure law coefficient
        return m_c0;
    }

    void FITKEquationOfStatePolynomial::setC1(double value)
    {
        //Set Hydrodynamic pressure law coefficient
        m_c1 = value;
    }

    double FITKEquationOfStatePolynomial::getC1()
    {
        // Get Hydrodynamic pressure law coefficient
        return m_c1;
    }

    void FITKEquationOfStatePolynomial::setC2(double value)
    {
        //Set Hydrodynamic pressure law coefficient
        m_c2 = value;
    }

    double FITKEquationOfStatePolynomial::getC2()
    {
        //Get Hydrodynamic pressure law coefficient
        return m_c2;
    }

    void FITKEquationOfStatePolynomial::setC3(double value)
    {
        //Set Hydrodynamic pressure law coefficient
        m_c3 = value;
    }

    double FITKEquationOfStatePolynomial::getC3()
    {
        //Get Hydrodynamic pressure law coefficient
        return m_c3;
    }

    void FITKEquationOfStatePolynomial::setC4(double value)
    {
        //Set Energy pressure law coefficient
        m_c4 = value;
    }

    double FITKEquationOfStatePolynomial::getC4()
    {
        //Get Energy pressure law coefficient
        return m_c4;
    }

    void FITKEquationOfStatePolynomial::setC5(double value)
    {
        //Set Energy pressure law coefficient
        m_c5 = value;
    }

    double FITKEquationOfStatePolynomial::getC5()
    {
        //Get Energy pressure law coefficient
        return m_c5;
    }

    void FITKEquationOfStatePolynomial::setE0(double value)
    {
        //Set Initial internal energy per unit reference volume
        m_e0 = value;
    }

    double FITKEquationOfStatePolynomial::getE0()
    {
        //Get Initial internal energy per unit reference volume
        return m_e0;
    }

    void FITKEquationOfStatePolynomial::setPsh(double value)
    {
        //Set Pressure shift
        m_psh = value;
    }

    double FITKEquationOfStatePolynomial::getPsh()
    {
        //Get Pressure shift
        return m_psh;
    }

}