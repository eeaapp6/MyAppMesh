/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPropBeam.h"

namespace Radioss
{
    FITKPropBeam::FITKPropBeam()
    {
        _value = new FITKPropValueBeam;
    }

    FITKPropBeam::~FITKPropBeam()
    {
        FITKPropValueBeam* value = _value.getObjectPtr();
        if (value) delete value;
    }

    FITKPropAbstract::RadPropType FITKPropBeam::getRadPropType()
    {
        return RadPropType::Beam;
    }

    FITKPropValueBeam * FITKPropBeam::getValue()
    {
        return _value.getObjectPtr();
    }

    void FITKPropValueBeam::setSmallStrainOption(int smallStrainOption)
    {
        _smallStrainOption = smallStrainOption;
    }

    int FITKPropValueBeam::getSmallStrainOption()
    {
        return _smallStrainOption;
    }

    void FITKPropValueBeam::setBeamMembraneDamping(double beamMembraneDamping)
    {
        _beamMembraneDamping = beamMembraneDamping;
    }

    double FITKPropValueBeam::getBeamMembraneDamping()
    {
        return _beamMembraneDamping;
    }

    void FITKPropValueBeam::setBeamFlexuralDamping(double beamFlexuralDamping)
    {
        _beamFlexuralDamping = beamFlexuralDamping;
    }

    double FITKPropValueBeam::getBeamFlexuralDamping()
    {
        return _beamFlexuralDamping;
    }

    void FITKPropValueBeam::setCrossSection(double crossSection)
    {
        _crossSection = crossSection;
    }

    double FITKPropValueBeam::getCrossSection()
    {
        return _crossSection;
    }

    void FITKPropValueBeam::setAreaMomentInertiaIyy(double areaMomentInertiaIyy)
    {
        _areaMomentInertiaIyy = areaMomentInertiaIyy;
    }

    double FITKPropValueBeam::getAreaMomentInertiaIyy()
    {
        return _areaMomentInertiaIyy;
    }

    void FITKPropValueBeam::setAreaMomentInertiaIzz(double areaMomentInertiaIzz)
    {
        _areaMomentInertiaIzz = areaMomentInertiaIzz;
    }

    double FITKPropValueBeam::getAreaMomentInertiaIzz()
    {
        return _areaMomentInertiaIzz;
    }

    void FITKPropValueBeam::setAreaMomentInertiaIxx(double areaMomentInertiaIxx)
    {
        _areaMomentInertiaIxx = areaMomentInertiaIxx;
    }

    double FITKPropValueBeam::getAreaMomentInertiaIxx()
    {
        return _areaMomentInertiaIxx;
    }

    void FITKPropValueBeam::setNeglectsShear(bool isNeglects)
    {
        _isNeglectsShear = isNeglects;
    }

    bool FITKPropValueBeam::isNeglectsShear()
    {
        return _isNeglectsShear;
    }
    void FITKPropValueBeam::setRotationDOF1(int index, bool is)
    {
        if (index < 0 || index > 2) return;
        _rotationDOF1[index] = is;
    }
    bool FITKPropValueBeam::isRotationDOF1(int index)
    {
        if (index < 0 || index > 2) return false;
        return _rotationDOF1[index];
    }
    void FITKPropValueBeam::setRotationDOF2(int index, bool is)
    {
        if (index < 0 || index > 2) return;
        _rotationDOF2[index] = is;
    }
    bool FITKPropValueBeam::isRotationDOF2(int index)
    {
        if (index < 0 || index > 2) return false;
        return _rotationDOF2[index];
    }

}