/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPropSandwichShell.h"

namespace Radioss
{
    FITKPropSandwichShell::FITKPropSandwichShell()
    {
        _value = new FITKPropSandwichShellValue();
    }

    FITKPropSandwichShell::~FITKPropSandwichShell()
    {
        FITKPropSandwichShellValue* value = _value.getObjectPtr();
        if (value) delete value;
    }

    FITKPropAbstract::RadPropType FITKPropSandwichShell::getRadPropType()
    {
        return FITKPropAbstract::RadPropType::Sandwich;
    }

    FITKPropSandwichShellValue * FITKPropSandwichShell::getValue()
    {
        return _value.getObjectPtr();
    }

    int FITKPropSandwichShellValue::getIshell() const
    {
        //Get Shell element formulation flag.
        return m_Ishell;
    }

    void FITKPropSandwichShellValue::setIshell(int value)
    {
        //Set Shell element formulation flag.
        m_Ishell = value;
    }

    int FITKPropSandwichShellValue::getIsmstr() const
    {
        //Get Shell small strain fommulation flag. 
        return m_Ismstr;
    }

    void FITKPropSandwichShellValue::setIsmstr(int value)
    {
        //Set Shell small strain fommulation flag.
        m_Ismstr = value;
    }

    int FITKPropSandwichShellValue::getIsh3n() const
    {
        // Get 3 node shell element formulation flag.
        return m_Ish3n;
    }

    void FITKPropSandwichShellValue::setIsh3n(int value)
    {
        //Set 3 node shell element formulation flag.
        m_Ish3n = value;
    }

    int FITKPropSandwichShellValue::getIdril() const
    {
        //Get Drilling DOF stiffness flag
        return m_Idril;
    }

    void FITKPropSandwichShellValue::setIdril(int value)
    {
        //Set Drilling DOF stiffness flag
        m_Idril = value;
    }

    double FITKPropSandwichShellValue::getPThickfail() const
    {
        //Get Percentage of layer thickness that must fail before the element isdeleted.
        return m_P_thickfail;
    }

    void FITKPropSandwichShellValue::setPThickfail(double value)
    {
        //Set Percentage of layer thickness that must fail before the element isdeleted.
        m_P_thickfail = value;
    }

    double FITKPropSandwichShellValue::getHm() const
    {
        //Get Shell membrane hourglass coefficient.
        return m_hm;
    }

    void FITKPropSandwichShellValue::setHm(double value)
    {
        //Set Shell membrane hourglass coefficient.
        m_hm = value;
    }

    double FITKPropSandwichShellValue::getHf() const
    {
        //Get Shell out-of-plane hourglass.
        return m_hf;
    }

    void FITKPropSandwichShellValue::setHf(double value)
    {
        //Set Shell out-of-plane hourglass.
        m_hf = value;
    }

    double FITKPropSandwichShellValue::getHr() const
    {
        //Get Shell rotation hourglass coefficient.
        return m_hr;
    }

    void FITKPropSandwichShellValue::setHr(double value)
    {
        //Set Shell rotation hourglass coefficient.
        m_hr = value;
    }

    double FITKPropSandwichShellValue::getDm() const
    {
        //Get Shell Membrane Damping.
        return m_dm;
    }

    void FITKPropSandwichShellValue::setDm(double value)
    {
        //Set Shell Membrane Damping.
        m_dm = value;
    }

    double FITKPropSandwichShellValue::getDn() const
    {
        //Get Shell numerical damping.
        return m_dn;
    }

    void FITKPropSandwichShellValue::setDn(double value)
    {
        //Set Shell numerical damping.
        m_dn = value;
    }

    int FITKPropSandwichShellValue::getN() const
    {
        // Get Number of layers.
        return _layerValues.size();
    }

    void FITKPropSandwichShellValue::setN(int value)
    {
        // Set Number of layers.
        _layerValues.resize(value);
    }

    int FITKPropSandwichShellValue::getIstrain() const
    {
        //Get Compute strains for post-processing flag.
        return m_Istrain;
    }

    void FITKPropSandwichShellValue::setIstrain(int value)
    {
        //Set Compute strains for post-processing flag.
        m_Istrain = value;
    }

    void FITKPropSandwichShellValue::setThick(double thick)
    {
        m_Thick = thick;
    }

    double FITKPropSandwichShellValue::getThick(bool isAllLayer)
    {
        if (!isAllLayer)
            return m_Thick;
        //Get Shell thickness.
        double thickness = 0.0;
        for (SandwichShellLayerValue& v : _layerValues)
        {
            thickness += v._thickness;
        }
        return thickness;
    }

    double FITKPropSandwichShellValue::getAshear() const
    {
        //Get Shear factor.
        return m_Ashear;
    }

    void FITKPropSandwichShellValue::setAshear(double value)
    {
        //Set Shear factor.
        m_Ashear = value;
    }

    int FITKPropSandwichShellValue::getIthick() const
    {
        //Get Shell resultant stresses calculation flag.
        return m_Ithick;
    }

    void FITKPropSandwichShellValue::setIthick(int value)
    {
        //Set Shell resultant stresses calculation flag.
        m_Ithick = value;
    }

    int FITKPropSandwichShellValue::getIplas() const
    {
        //Get Shell plane stress plasticity flag.
        return m_Iplas;
    }

    void FITKPropSandwichShellValue::setIplas(int value)
    {
        //Set Shell plane stress plasticity flag.
        m_Iplas = value;
    }

    double FITKPropSandwichShellValue::getVx() const
    {
        //Get X component for reference vector.
        return m_vx;
    }

    void FITKPropSandwichShellValue::setVx(double value)
    {
        //Set X component for reference vector.
        m_vx = value;
    }

    double FITKPropSandwichShellValue::getVy() const
    {
        //Get Y component for reference vector.
        return m_vy;
    }

    void FITKPropSandwichShellValue::setVy(double value)
    {
        //Set Y component for reference vector.
        m_vy = value;
    }

    double FITKPropSandwichShellValue::getVz() const
    {
        //Get Z component for reference vector.
        return m_vz;
    }

    void FITKPropSandwichShellValue::setVz(double value)
    {
        //Set Z component for reference vector.
        m_vz = value;
    }

    int FITKPropSandwichShellValue::getSkewID() const
    {
        //Get Skew identifier for reference vector. 
        return m_skew_id;
    }

    void FITKPropSandwichShellValue::setSkewID(int value)
    {
        //Set Skew identifier for reference vector. 
        m_skew_id = value;
    }

    int FITKPropSandwichShellValue::getIorth() const
    {
        //Get Orthotropic system formulation flag for reference vector.
        return m_Iorth;
    }

    void FITKPropSandwichShellValue::setIorth(int value)
    {
        //Set Orthotropic system formulation flag for reference vector.
        m_Iorth = value;
    }

    bool FITKPropSandwichShellValue::getIpos() const
    {
        //Get Orthotropic system formulation flag for reference vector.
        return m_Ipos;
    }

    void FITKPropSandwichShellValue::setIpos(bool value)
    {
        //Set Orthotropic system formulation flag for reference vector.
        m_Ipos = value;
    }

    SandwichShellLayerValue FITKPropSandwichShellValue::getLayerValue(int index) const
    {
        //Get Value for Layer i.
        if (_layerValues.size() <= index || index < 0)
            return SandwichShellLayerValue();
        return _layerValues[index];
    }

    void FITKPropSandwichShellValue::setLayerValue(SandwichShellLayerValue & v, int index)
    {
        //Set Value for Layer i.
        if (_layerValues.size() <= index || index < 0)
            return;
        _layerValues[index] = v;
    }

}
