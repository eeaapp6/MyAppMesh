/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFailureModelBIQUAD.h"

namespace Radioss
{

    FITKFailureModelBIQUAD::FITKFailureModelBIQUAD()
    {
    }

    FITKFailureModelBIQUAD::~FITKFailureModelBIQUAD()
    {
    }

    FITKAbstractFailureModel::FailureModelType FITKFailureModelBIQUAD::getFailureModelType()
    {
        return FITKAbstractFailureModel::FailureModelType::BIQUAD;
    }

    void FITKFailureModelBIQUAD::setC1(const double value)
    {
        m_c1 = value;
    }

    double FITKFailureModelBIQUAD::getC1() const
    {
        return m_c1;
    }

    void FITKFailureModelBIQUAD::setC2(const double value)
    {
        m_c2 = value;
    }

    double FITKFailureModelBIQUAD::getC2() const
    {
        return m_c2;
    }

    void FITKFailureModelBIQUAD::setC3(const double value)
    {
        m_c3 = value;
    }

    double FITKFailureModelBIQUAD::getC3() const
    {
        return m_c3;
    }

    void FITKFailureModelBIQUAD::setC4(const double value)
    {
        m_c4 = value;
    }

    double FITKFailureModelBIQUAD::getC4() const
    {
        return m_c4;
    }

    void FITKFailureModelBIQUAD::setC5(const double value)
    {
        m_c5 = value;
    }

    double FITKFailureModelBIQUAD::getC5() const
    {
        return m_c5;
    }

    void FITKFailureModelBIQUAD::setPthickfail(const double value)
    {
        m_P_thickfail = value;
    }

    double FITKFailureModelBIQUAD::getPthickfail() const
    {
        return m_P_thickfail;
    }

    void FITKFailureModelBIQUAD::setMFlag(const int flag)
    {
        m_M_Flag = flag;
    }

    int FITKFailureModelBIQUAD::getMFlag() const
    {
        return m_M_Flag;
    }

    void FITKFailureModelBIQUAD::setSFlag(const int flag)
    {
        m_S_Flag = flag;
    }

    int FITKFailureModelBIQUAD::getSFlag() const
    {
        return m_S_Flag;
    }

    void FITKFailureModelBIQUAD::setInststart(const double value)
    {
        m_Inst_start = value;
    }

    double FITKFailureModelBIQUAD::getInststart() const
    {
        return m_Inst_start;
    }

    void FITKFailureModelBIQUAD::setFctIDel(const int id)
    {
        m_fct_IDel = id;
    }

    int FITKFailureModelBIQUAD::getFctIDel() const
    {
        return m_fct_IDel;
    }

    void FITKFailureModelBIQUAD::setElref(const double value)
    {
        m_El_ref = value;
    }

    double FITKFailureModelBIQUAD::getElref() const
    {
        return m_El_ref;
    }

    void FITKFailureModelBIQUAD::setR1(const double value)
    {
        m_r1 = value;
    }

    double FITKFailureModelBIQUAD::getR1() const
    {
        return m_r1;
    }

    void FITKFailureModelBIQUAD::setR2(const double value)
    {
        m_r2 = value;
    }

    double FITKFailureModelBIQUAD::getR2() const
    {
        return m_r2;
    }

    void FITKFailureModelBIQUAD::setR4(const double value)
    {
        m_r4 = value;
    }

    double FITKFailureModelBIQUAD::getR4() const
    {
        return m_r4;
    }

    void FITKFailureModelBIQUAD::setR5(const double value)
    {
        m_r5 = value;
    }

    double FITKFailureModelBIQUAD::getR5() const
    {
        return m_r5;
    }

} // namespace Radioss
