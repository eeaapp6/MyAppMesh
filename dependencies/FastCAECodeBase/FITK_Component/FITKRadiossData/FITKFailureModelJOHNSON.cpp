/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFailureModelJOHNSON.h"

namespace Radioss
{
    FITKFailureModelJOHNSON::FITKFailureModelJOHNSON()
    {

    }

    FITKFailureModelJOHNSON::~FITKFailureModelJOHNSON()
    {
    }

    FITKAbstractFailureModel::FailureModelType FITKFailureModelJOHNSON::getFailureModelType()
    {
        return FITKAbstractFailureModel::FailureModelType::JOHNSON;
    }

    void FITKFailureModelJOHNSON::setD1(const double value)
    {
        m_d1 = value;
    }

    double FITKFailureModelJOHNSON::getD1() const
    {
        return m_d1;
    }

    void FITKFailureModelJOHNSON::setD2(const double value)
    {
        m_d2 = value;
    }

    double FITKFailureModelJOHNSON::getD2() const
    {
        return m_d2;
    }

    void FITKFailureModelJOHNSON::setD3(const double value)
    {
        m_d3 = value;
    }

    double FITKFailureModelJOHNSON::getD3() const
    {
        return m_d3;
    }

    void FITKFailureModelJOHNSON::setD4(const double value)
    {
        m_d4 = value;
    }

    double FITKFailureModelJOHNSON::getD4() const
    {
        return m_d4;
    }

    void FITKFailureModelJOHNSON::setD5(const double value)
    {
        m_d5 = value;
    }

    double FITKFailureModelJOHNSON::getD5() const
    {
        return m_d5;
    }

    void FITKFailureModelJOHNSON::setEps0(const double value)
    {
        m_eps_0 = value;
    }

    double FITKFailureModelJOHNSON::getEps0() const
    {
        return m_eps_0;
    }

    void FITKFailureModelJOHNSON::setIfailsh(const int flag)
    {
        m_Ifail_sh = flag;
    }

    int FITKFailureModelJOHNSON::getIfailsh() const
    {
        return m_Ifail_sh;
    }

    void FITKFailureModelJOHNSON::setIfailso(const int flag)
    {
        m_Ifail_so = flag;
    }

    int FITKFailureModelJOHNSON::getIfailso() const
    {
        return m_Ifail_so;
    }

    void FITKFailureModelJOHNSON::setDadv(const double value)
    {
        m_Dadv = value;
    }

    double FITKFailureModelJOHNSON::getDadv() const
    {
        return m_Dadv;
    }

    void FITKFailureModelJOHNSON::setIxfem(const int flag)
    {
        m_Ixfem = flag;
    }

    int FITKFailureModelJOHNSON::getIxfem() const
    {
        return m_Ixfem;
    }

} // namespace Radioss
