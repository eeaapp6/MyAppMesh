/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPropShell.h"

namespace Radioss
{ 
    FITKPropShell::FITKPropShell()
    {
        _value = new FITKPropShellValue();
    }

    FITKPropShell::~FITKPropShell()
    {

        FITKPropShellValue* value = _value.getObjectPtr();
        if (value) delete value;
    }

    FITKPropAbstract::RadPropType FITKPropShell::getRadPropType()
    {
        return FITKPropAbstract::RadPropType::Shell;
    }

    FITKPropShellValue * FITKPropShell::getValue()
    {
        return _value.getObjectPtr();
    }

    int FITKPropShellValue::getIshell() const
    {
        return m_Ishell;
    }
    void FITKPropShellValue::setIshell(int value)
    {
        m_Ishell = value;
    }

    int FITKPropShellValue::getIsmstr() const
    {
        return m_Ismstr;
    }
    void FITKPropShellValue::setIsmstr(int value)
    {
        m_Ismstr = value;
    }

    int FITKPropShellValue::getIthick() const
    {
        return m_Ithick;
    }
    void FITKPropShellValue::setIthick(int value)
    {
        m_Ithick = value;
    }

    int FITKPropShellValue::getIplas() const
    {
        return m_Iplas;
    }
    void FITKPropShellValue::setIplas(int value)
    {
        m_Iplas = value;
    }

    int FITKPropShellValue::getIsh3n() const
    {
        return m_Ish3n;
    }
    void FITKPropShellValue::setIsh3n(int value)
    {
        m_Ish3n = value;
    }

    int FITKPropShellValue::getIdril() const
    {
        return m_Idril;
    }
    void FITKPropShellValue::setIdril(int value)
    {
        m_Idril = value;
    }

    int FITKPropShell::getIpinch() const
    {
        return m_Ipinch;
    }
    void FITKPropShell::setIpinch(int value)
    {
        m_Ipinch = value;
    }

    double FITKPropShell::getPthickFail() const
    {
        return m_PthickFail;
    }
    void FITKPropShell::setPthickFail(double value)
    {
        m_PthickFail = value;
    }

    double FITKPropShell::getHm() const
    {
        return m_hm;
    }
    void FITKPropShell::setHm(double value)
    {
        m_hm = value;
    }

    double FITKPropShell::getHf() const
    {
        return m_hf;
    }
    void FITKPropShell::setHf(double value)
    {
        m_hf = value;
    }

    double FITKPropShell::getHr() const
    {
        return m_hr;
    }
    void FITKPropShell::setHr(double value)
    {
        m_hr = value;
    }

    double FITKPropShell::getDm() const
    {
        return m_dm;
    }
    void FITKPropShell::setDm(double value)
    {
        m_dm = value;
    }

    double FITKPropShell::getDn() const
    {
        return m_dn;
    }
    void FITKPropShell::setDn(double value)
    {
        m_dn = value;
    }

    int FITKPropShell::getN() const
    {
        return m_N;
    }
    void FITKPropShell::setN(int value)
    {
        m_N = value;
    }

    double FITKPropShell::getThick() const
    {
        return m_Thick;
    }
    void FITKPropShell::setThick(double value)
    {
        m_Thick = value;
    }

    double FITKPropShell::getAshear() const
    {
        return m_Ashear;
    }
    void FITKPropShell::setAshear(double value)
    {
        m_Ashear = value;
    }

}
