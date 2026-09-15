/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MaterialPlasTabWapper.h"

#include "FITK_Component/FITKRadiossData/FITKMaterialPlasTab.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"


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

    Radioss::FITKMaterialPlasTab* getPlasTabMaterial(PlasTab* obj)
    {
        if (obj == nullptr)
        {
            return nullptr;
        }

        return dynamic_cast<Radioss::FITKMaterialPlasTab*>(obj->getMaterial());
    }
}

PlasTab* MaterialPlasTabWapper::new_PlasTab()
{
    return new PlasTab();
}

void MaterialPlasTabWapper::delete_PlasTab(PlasTab* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

PlasTab* MaterialPlasTabWapper::static_PlasTab_GetPlasTab(const QString& name)
{
    return new PlasTab(name);
}

QString MaterialPlasTabWapper::getName(PlasTab* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString MaterialPlasTabWapper::setName(PlasTab* obj, const QString& name)
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

QString MaterialPlasTabWapper::createMaterial(PlasTab* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    bool ok = obj->createMaterial(name, static_cast<int>(Radioss::FITKAbstractRadMaterial::LAW36));
    if (!ok)
    {
        return sendMessageToServer(tr("Creation failed! Material with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, PlasTab material created with name %1").arg(name));
}

QString MaterialPlasTabWapper::setDensity(PlasTab* obj, double density)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found");
    }

    material->getMaterialData().m_density = density;
    return sendMessageToServer("Set Density successfully");
}

QString MaterialPlasTabWapper::setYoungsModulus(PlasTab* obj, double modulus)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found");
    }

    material->getMaterialData().m_youngsModulus = modulus;
    return sendMessageToServer("Set Youngs Modulus successfully");
}

QString MaterialPlasTabWapper::setPoissonsRatio(PlasTab* obj, double ratio)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_poissonsRatio = ratio;
    return sendMessageToServer("Set Poissons Ratio successfully");
}

QString MaterialPlasTabWapper::setFailureStrain(PlasTab* obj, double strain)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_failureStrain = strain;
    return sendMessageToServer("Set Failure Strain successfully");
}

QString MaterialPlasTabWapper::setTensileFailureStrain(PlasTab* obj, double strain)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_tensileFailureStrain = strain;
    return sendMessageToServer("Set Tensile Failure Strain successfully");
}

QString MaterialPlasTabWapper::setMaxTensileFailureStrain(PlasTab* obj, double strain)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_maxTensileFailureStrain = strain;
    return sendMessageToServer("Set Max Tensile Failure Strain successfully");
}

QString MaterialPlasTabWapper::setNumberOfFunctions(PlasTab* obj, int count)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    if (count < 0)
    {
        count = 0;
    }

    Radioss::FITKMaterialPlasTabData& data = material->getMaterialData();
    data.m_numberOfFunctions = count;

    while (data.m_fctID.size() < data.m_numberOfFunctions)
    {
        data.m_fctID.append(-1);
    }

    while (data.m_fctScale.size() < data.m_numberOfFunctions)
    {
        data.m_fctScale.append(1.0);
    }

    while (data.m_epsilon.size() < data.m_numberOfFunctions)
    {
        data.m_epsilon.append(0.0);
    }

    while (data.m_fctID.size() > data.m_numberOfFunctions)
    {
        data.m_fctID.removeLast();
    }

    while (data.m_fctScale.size() > data.m_numberOfFunctions)
    {
        data.m_fctScale.removeLast();
    }

    while (data.m_epsilon.size() > data.m_numberOfFunctions)
    {
        data.m_epsilon.removeLast();
    }

    return sendMessageToServer("Set Number Of Functions successfully");
}

QString MaterialPlasTabWapper::setFunction(PlasTab* obj, int index, const QString& curveName)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    Radioss::FITKMaterialPlasTabData& data = material->getMaterialData();
    int actualIndex = index - 1;
    if (actualIndex < 0 || actualIndex >= data.m_fctID.size())
    {
        return sendMessageToServer("Failed !Index is 1-based and out of range!");
    }
    // 获取 Radioss Case
    Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (caseData == nullptr)
    {
        return sendMessageToServer("Failed !No Radioss case found!");
    }
    // 获取曲线管理器
    Radioss::FITKNonLinerCurveManager* curveManager = caseData->getNonLinerCurveManager();
    if (curveManager == nullptr)
    {
        return sendMessageToServer("Failed !No non liner curve manager found!");
    }
    auto curve = curveManager->getDataByName(curveName);
    if (curve == nullptr)
    {
        return sendMessageToServer(tr("Failed !Curve with name %1 not found!").arg(curveName));
    }

    data.m_fctID[actualIndex] = curve->getDataObjectID();
    return sendMessageToServer("Set Function successfully");
}

QString MaterialPlasTabWapper::setFctScale(PlasTab* obj, int index, double param)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    Radioss::FITKMaterialPlasTabData& data = material->getMaterialData();
    int actualIndex = index - 1;
    if (actualIndex < 0 || actualIndex >= data.m_fctScale.size())
    {
        return sendMessageToServer("Failed !Index is 1-based and out of range!");
    }

    data.m_fctScale[actualIndex] = param;
    return sendMessageToServer("Set Fct Scale successfully");
}

QString MaterialPlasTabWapper::setEpsilon(PlasTab* obj, int index, double param)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    Radioss::FITKMaterialPlasTabData& data = material->getMaterialData();
    int actualIndex = index - 1;
    if (actualIndex < 0 || actualIndex >= data.m_epsilon.size())
    {
        return sendMessageToServer("Failed !Index is 1-based and out of range!");
    }

    data.m_epsilon[actualIndex] = param;
    return sendMessageToServer("Set Epsilon successfully");
}

QString MaterialPlasTabWapper::setSmoothStrainRateOptFlag(PlasTab* obj, int flag)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_smoothStrainRateOptFlag = flag;
    return sendMessageToServer("Set Smooth Strain Rate Opt Flag successfully");
}

QString MaterialPlasTabWapper::setChard(PlasTab* obj, double hardening)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_Chard = hardening;
    return sendMessageToServer("Set Chard successfully");
}

QString MaterialPlasTabWapper::setFcut(PlasTab* obj, double frequency)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_Fcut = frequency;
    return sendMessageToServer("Set Fcut successfully");
}

QString MaterialPlasTabWapper::setVpFlag(PlasTab* obj, int flag)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_vpFlag = flag;
    return sendMessageToServer("Set Vp Flag successfully");
}

QString MaterialPlasTabWapper::setTensileStrainForElementDeletion(PlasTab* obj, double strain)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_tensileStrainForElementDeletion = strain;
    return sendMessageToServer("Set Tensile Strain For Element Deletion successfully");
}

QString MaterialPlasTabWapper::setFctIDp(PlasTab* obj, int id)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_fctIDp = id;
    return sendMessageToServer("Set Fct IDp successfully");
}

QString MaterialPlasTabWapper::setFscaleFactor(PlasTab* obj, double factor)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_fscaleFactor = factor;
    return sendMessageToServer("Set Fscale Factor successfully");
}

QString MaterialPlasTabWapper::setFctIODE(PlasTab* obj, int id)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_fctIODE = id;
    return sendMessageToServer("Set Fct IODE successfully");
}

QString MaterialPlasTabWapper::setEint(PlasTab* obj, double modulus)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_Eint = modulus;
    return sendMessageToServer("Set Eint successfully");
}

QString MaterialPlasTabWapper::setCe(PlasTab* obj, double ce)
{
    Radioss::FITKMaterialPlasTab* material = getPlasTabMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    material->getMaterialData().m_Ce = ce;
    return sendMessageToServer("Set Ce successfully");
}

PlasTab::PlasTab(const QString& name)
    : PyInterface::PyMaterialsBase(name)
{
}

PlasTab::PlasTab()
    : PyInterface::PyMaterialsBase()
{
}