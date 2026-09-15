/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MaterialPlasJohnsWapper.h"

#include "FITK_Component/FITKRadiossData/FITKMaterialPlasJohns.h"
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

    Radioss::FITKMaterialPlasJohns* getPlasJohnsMaterial(PlasJohns* obj)
    {
        if (obj == nullptr)
        {
            return nullptr;
        }

        return dynamic_cast<Radioss::FITKMaterialPlasJohns*>(obj->getMaterial());
    }
}

PlasJohns* MaterialPlasJohnsWapper::new_PlasJohns()
{
    return new PlasJohns();
}

void MaterialPlasJohnsWapper::delete_PlasJohns(PlasJohns* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

PlasJohns* MaterialPlasJohnsWapper::static_PlasJohns_GetPlasJohns(const QString& name)
{
    return new PlasJohns(name);
}

QString MaterialPlasJohnsWapper::getName(PlasJohns* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString MaterialPlasJohnsWapper::setName(PlasJohns* obj, const QString& name)
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

QString MaterialPlasJohnsWapper::createMaterial(PlasJohns* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    bool ok = obj->createMaterial(name, static_cast<int>(Radioss::FITKAbstractRadMaterial::LAW2));
    if (!ok)
    {
        return sendMessageToServer(tr("Creation failed! Material with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Plas-Johns material created with name %1").arg(name));
}

QString MaterialPlasJohnsWapper::setType(PlasJohns* obj, int type)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found! ");
    }

    if (type != static_cast<int>(Radioss::FITKMaterialPlasJohns::MATERIAL_PLAS_JOHNS_TYPE_CLASSIC)
        && type != static_cast<int>(Radioss::FITKMaterialPlasJohns::MATERIAL_PLAS_JOHNS_TYPE_SIMPLE))
    {
        return sendMessageToServer("Failed !Material exists, But the PlasJohns Type invalid !");
    }

    material->setType(static_cast<Radioss::FITKMaterialPlasJohns::MaterialPlasJohnsType>(type));
    return sendMessageToServer("Set PlasJohns Type successfully");
}

QString MaterialPlasJohnsWapper::setDensity(PlasJohns* obj, double density)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found");
    }

    material->getMaterialData().m_density = density;

    return sendMessageToServer("Set Density successfully");
}

QString MaterialPlasJohnsWapper::setYoungsModulus(PlasJohns* obj, double modulus)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found");
    }

    material->getMaterialData().m_youngsModulus = modulus;

    return sendMessageToServer("Set Youngs Modulus successfully");
}

QString MaterialPlasJohnsWapper::setPoissonsRatio(PlasJohns* obj, double ratio)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_poissonsRatio = ratio;

    return sendMessageToServer("Set Poissons Ratio successfully");
}

QString MaterialPlasJohnsWapper::setYieldStress(PlasJohns* obj, double stress)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_yieldStress = stress;

    return sendMessageToServer("Set Yield Stress successfully");
}

QString MaterialPlasJohnsWapper::setPlasticHardening(PlasJohns* obj, double hardening)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_plasticHardening = hardening;

    return sendMessageToServer("Set Plastic Hardening successfully");
}

QString MaterialPlasJohnsWapper::setPlasticHardeningExponent(PlasJohns* obj, double exponent)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_plasticHardeningExponent = exponent;

    return sendMessageToServer("Set Plastic Hardening Exponent successfully");
}

QString MaterialPlasJohnsWapper::setFailureStrain(PlasJohns* obj, double strain)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_failureStrain = strain;

    return sendMessageToServer("Set Failure Strain successfully");
}

QString MaterialPlasJohnsWapper::setMaxStress(PlasJohns* obj, double stress)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_maxStress = stress;

    return sendMessageToServer("Set Max Stress successfully");
}

QString MaterialPlasJohnsWapper::setUTS(PlasJohns* obj, double uts)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_UTS = uts;

    return sendMessageToServer("Set UTS successfully");
}

QString MaterialPlasJohnsWapper::setEpsilonUTS(PlasJohns* obj, double strain)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_EpsilonUTS = strain;

    return sendMessageToServer("Set Epsilon UTS successfully");
}

QString MaterialPlasJohnsWapper::setStrainRateCoefficient(PlasJohns* obj, double coefficient)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_c = coefficient;

    return sendMessageToServer("Set Strain Rate Coefficient successfully");
}

QString MaterialPlasJohnsWapper::setReferenceStrainRate(PlasJohns* obj, double epsilon0)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_Epsilon0 = epsilon0;

    return sendMessageToServer("Set Reference Strain Rate successfully");
}

QString MaterialPlasJohnsWapper::setStrainRateComputation(PlasJohns* obj, int icc)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_ICC = icc;

    return sendMessageToServer("Set Strain Rate Computation successfully");
}

QString MaterialPlasJohnsWapper::setStrainRateSmoothing(PlasJohns* obj, int smoothing)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_Fsmooth = smoothing;

    return sendMessageToServer("Set Strain Rate Smoothing successfully");
}

QString MaterialPlasJohnsWapper::setCutoffFreq(PlasJohns* obj, double frequency)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_Fcut = frequency;

    return sendMessageToServer("Set Cutoff Frequency successfully");
}

QString MaterialPlasJohnsWapper::setChard(PlasJohns* obj, double hardening)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_Chard = hardening;

    return sendMessageToServer("Set Chard successfully");
}

QString MaterialPlasJohnsWapper::setTempExponent(PlasJohns* obj, double exponent)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_tempExponent = exponent;

    return sendMessageToServer("Set Temp Exponent successfully");
}

QString MaterialPlasJohnsWapper::setMeltingTemperature(PlasJohns* obj, double temperature)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_meltingTemperature = temperature;

    return sendMessageToServer("Set Melting Temperature successfully");
}

QString MaterialPlasJohnsWapper::setRhoCp(PlasJohns* obj, double rhoCp)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_rhoCp = rhoCp;

    return sendMessageToServer("Set RhoCp successfully");
}

QString MaterialPlasJohnsWapper::setTref(PlasJohns* obj, double temperature)
{
    Radioss::FITKMaterialPlasJohns* material = getPlasJohnsMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_Tref = temperature;

    return sendMessageToServer("Set Tref successfully");
}

PlasJohns::PlasJohns(const QString& name)
    : PyInterface::PyMaterialsBase(name)
{
}

PlasJohns::PlasJohns()
    : PyInterface::PyMaterialsBase()
{
}