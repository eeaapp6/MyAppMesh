/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPropBeamSPR.h"

namespace Radioss
{
    FITKPropValueBeamSPR::FITKPropValueBeamSPR()
        : _TensionCompression(Tension_Compression),_ShearXY(Shear_XY),_ShearXZ(Shear_XZ),
        _Torsion(Torsion),_BendingY(Bending_Y),_BendingZ(Bending_Z)
    {
    }

    ValueDOF& FITKPropValueBeamSPR::getValueDOF(DOF_SPR dof)
    {
        switch (dof)
        {
        case Tension_Compression:
            return _TensionCompression;
        case Shear_XY:
            return _ShearXY;
        case Shear_XZ:
            return _ShearXZ;
        case Torsion:
            return _Torsion;
        case Bending_Y:
            return _BendingY;
        case Bending_Z:
            return _BendingZ;
        default:
            break;
        }
    }

    FITKPropBeamSPR::FITKPropBeamSPR()
    {
        _value = new FITKPropValueBeamSPR();
    }
    FITKPropBeamSPR::~FITKPropBeamSPR()
    {
        FITKPropValueBeamSPR* value = _value.getObjectPtr();
        if (value) delete value;
    }
    FITKPropAbstract::RadPropType FITKPropBeamSPR::getRadPropType()
    {
        return FITKPropAbstract::RadPropType::BeamSPR;
    }
    FITKPropValueBeamSPR* FITKPropBeamSPR::getValue()
    {
        return _value.getObjectPtr();
    }

    QString FITKPropBeamSPR::getStrDOF(DOF_SPR dof)
    {
        switch (dof)
        {
        case Tension_Compression:
            return "Tension Compression";
        case Shear_XY:
            return "Shear XY";
        case Shear_XZ:
            return "Shear XZ";
        case Torsion:
            return "Torsion";
        case Bending_Y:
            return "Bending Y";
        case Bending_Z:
            return "Bending Z";
        default:
            break;
        }
    }

}