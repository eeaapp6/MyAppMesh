/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMaterialElastic.h"

namespace Radioss
{
    double FITKMaterialVoid::getDensity() const
    {
        return m_density;
    }

    void FITKMaterialVoid::setDensity(double density)
    {
        m_density = density;
    }

    double FITKMaterialVoid::getYoungsModulus() const
    {
        return m_youngsModulus;
    }

    void FITKMaterialVoid::setYoungsModulus(double modulus)
    {
        m_youngsModulus = modulus;
    }

    double FITKMaterialVoid::getPoissonsRatio() const
    {
        return m_poissonsRatio;
    }

    void FITKMaterialVoid::setPoissonsRatio(double ratio)
    {
        m_poissonsRatio = ratio;
    }
}
