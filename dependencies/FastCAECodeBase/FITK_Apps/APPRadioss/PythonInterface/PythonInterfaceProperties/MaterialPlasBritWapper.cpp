/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MaterialPlasBritWapper.h"

#include "FITK_Component/FITKRadiossData/FITKMaterialPlasBrit.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"

namespace
{
    QString sendMessageToServer(const QString& message, bool isToServer = true)
    {
        if (isToServer)
        {
            HttpPython::FITKHttpCommons::sendMessageToServer(message);
        }
        return message;
    }

    Radioss::FITKMaterialPlasBrit* getPlasBritMaterial(PlasBrit* obj)
    {
        if (obj == nullptr)
        {
            return nullptr;
        }

        return dynamic_cast<Radioss::FITKMaterialPlasBrit*>(obj->getMaterial());
    }
}

PlasBrit* MaterialPlasBritWapper::new_PlasBrit()
{
    return new PlasBrit();
}

void MaterialPlasBritWapper::delete_PlasBrit(PlasBrit* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

PlasBrit* MaterialPlasBritWapper::static_PlasBrit_GetPlasBrit(const QString& name)
{
    return new PlasBrit(name);
}

QString MaterialPlasBritWapper::getName(PlasBrit* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString MaterialPlasBritWapper::setName(PlasBrit* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    bool ok = obj->setName(name);
    if (!ok)
    {
        return sendMessageToServer(tr("Failed, Material with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString MaterialPlasBritWapper::createMaterial(PlasBrit* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    bool ok = obj->createMaterial(name, static_cast<int>(Radioss::FITKAbstractRadMaterial::LAW27));
    if (!ok)
    {
        return sendMessageToServer(tr("Creation failed! Material with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, PlasBrit material created with name %1").arg(name));
}

QString MaterialPlasBritWapper::setDensity(PlasBrit* obj, double density)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found");
    }

    material->getMaterialData().m_density = density;
    return sendMessageToServer("Set Density successfully");
}

QString MaterialPlasBritWapper::setYoungsModulus(PlasBrit* obj, double modulus)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found");
    }

    material->getMaterialData().m_youngsModulus = modulus;
    return sendMessageToServer("Set Youngs Modulus successfully");
}

QString MaterialPlasBritWapper::setPoissonsRatio(PlasBrit* obj, double ratio)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_poissonsRatio = ratio;
    return sendMessageToServer("Set Poissons Ratio successfully");
}

QString MaterialPlasBritWapper::setPlasticityYieldStress(PlasBrit* obj, double stress)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_plasticityYieldStress = stress;
    return sendMessageToServer("Set Plasticity Yield Stress successfully");
}

QString MaterialPlasBritWapper::setPlasticityHardeningParameter(PlasBrit* obj, double parameter)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_plasticityHardeningParameter = parameter;
    return sendMessageToServer("Set Plasticity Hardening Parameter successfully");
}

QString MaterialPlasBritWapper::setPlasticityHardeningExponent(PlasBrit* obj, double exponent)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_plasticityHardeningExponent = exponent;
    return sendMessageToServer("Set Plasticity Hardening Exponent successfully");
}

QString MaterialPlasBritWapper::setPlasticityMaximumStress(PlasBrit* obj, double stress)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_plasticityMaximumStress = stress;
    return sendMessageToServer("Set Plasticity Maximum Stress successfully");
}

QString MaterialPlasBritWapper::setStrainRateCoefficient(PlasBrit* obj, double coefficient)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_strainRateCoefficient = coefficient;
    return sendMessageToServer("Set Strain Rate Coefficient successfully");
}

QString MaterialPlasBritWapper::setReferenceStrainRate(PlasBrit* obj, double rate)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_referenceStrainRate = rate;
    return sendMessageToServer("Set Reference Strain Rate successfully");
}

QString MaterialPlasBritWapper::setStrainRateComputation(PlasBrit* obj, int computation)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_strainRateComputation = computation;
    return sendMessageToServer("Set Strain Rate Computation successfully");
}

QString MaterialPlasBritWapper::setStrainRateSmoothing(PlasBrit* obj, int smoothing)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_strainRateSmoothing = smoothing;
    return sendMessageToServer("Set Strain Rate Smoothing successfully");
}

QString MaterialPlasBritWapper::setCutoffFreq(PlasBrit* obj, double frequency)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_cutoffFreq = frequency;
    return sendMessageToServer("Set Cutoff Frequency successfully");
}

QString MaterialPlasBritWapper::setTensileFilureStrain1(PlasBrit* obj, double strain)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_tensileFilureStrain1 = strain;
    return sendMessageToServer("Set Tensile Filure Strain1 successfully");
}

QString MaterialPlasBritWapper::setMaximumTensileFilureStrain1(PlasBrit* obj, double strain)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_maximumTensileFilureStrain1 = strain;
    return sendMessageToServer("Set Maximum Tensile Filure Strain1 successfully");
}

QString MaterialPlasBritWapper::setMaximumDamageFactor1(PlasBrit* obj, double factor)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_maximumDamageFactor1 = factor;
    return sendMessageToServer("Set Maximum Damage Factor1 successfully");
}

QString MaterialPlasBritWapper::setMaximumTensileStrain1(PlasBrit* obj, double strain)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_maximumTensileStrain1 = strain;
    return sendMessageToServer("Set Maximum Tensile Strain1 successfully");
}

QString MaterialPlasBritWapper::setTensileFilureStrain2(PlasBrit* obj, double strain)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_tensileFilureStrain2 = strain;
    return sendMessageToServer("Set Tensile Filure Strain2 successfully");
}

QString MaterialPlasBritWapper::setMaximumTensileFilureStrain2(PlasBrit* obj, double strain)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_maximumTensileFilureStrain2 = strain;
    return sendMessageToServer("Set Maximum Tensile Filure Strain2 successfully");
}

QString MaterialPlasBritWapper::setMaximumDamageFactor2(PlasBrit* obj, double factor)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_maximumDamageFactor2 = factor;
    return sendMessageToServer("Set Maximum Damage Factor2 successfully");
}

QString MaterialPlasBritWapper::setMaximumTensileStrain2(PlasBrit* obj, double strain)
{
    Radioss::FITKMaterialPlasBrit* material = getPlasBritMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_maximumTensileStrain2 = strain;
    return sendMessageToServer("Set Maximum Tensile Strain2 successfully");
}

PlasBrit::PlasBrit(const QString& name)
    : PyInterface::PyMaterialsBase(name)
{
}

PlasBrit::PlasBrit()
    : PyInterface::PyMaterialsBase()
{
}