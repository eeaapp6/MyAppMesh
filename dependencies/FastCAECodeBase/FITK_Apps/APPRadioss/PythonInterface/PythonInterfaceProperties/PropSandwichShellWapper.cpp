/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PropSandwichShellWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKPropSandwichShell.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRadMaterial.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

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

    Radioss::FITKPropSandwichShell* getSandwichShellProperty(SandwichShell* obj)
    {
        if (obj == nullptr)
        {
            return nullptr;
        }

        return dynamic_cast<Radioss::FITKPropSandwichShell*>(obj->getProperty());
    }

    Radioss::FITKPropSandwichShellValue* getSandwichShellValue(SandwichShell* obj)
    {
        Radioss::FITKPropSandwichShell* property = getSandwichShellProperty(obj);
        if (property == nullptr)
        {
            return nullptr;
        }

        return property->getValue();
    }

    bool getSandwichShellLayerValue(
        SandwichShell* obj,
        int index,
        Radioss::FITKPropSandwichShellValue*& propValue,
        Radioss::SandwichShellLayerValue& layerValue,
        int& actualIndex)
    {
        propValue = getSandwichShellValue(obj);
        if (propValue == nullptr)
        {
            return false;
        }

        actualIndex = index - 1;
        if (actualIndex < 0 || actualIndex >= propValue->getN())
        {
            return false;
        }

        layerValue = propValue->getLayerValue(actualIndex);
        return true;
    }

    Interface::FITKAbstractMaterial* getSandwichShellMaterial(const QString& materialName)
    {
        Radioss::FITKRadiossCase* caseData =
            FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr)
        {
            return nullptr;
        }

        Interface::FITKMaterialManager* materialManager = caseData->getMaterialManager();
        if (materialManager == nullptr)
        {
            return nullptr;
        }

        return materialManager->getDataByName(materialName);
    }
}

SandwichShell* PropSandwichShellWapper::new_SandwichShell()
{
    return new SandwichShell();
}

void PropSandwichShellWapper::delete_SandwichShell(SandwichShell* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

SandwichShell* PropSandwichShellWapper::static_SandwichShell_GetSandwichShell(const QString& name)
{
    return new SandwichShell(name);
}

QString PropSandwichShellWapper::getName(SandwichShell* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString PropSandwichShellWapper::setName(SandwichShell* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    bool ok = obj->setName(name);
    if (!ok)
    {
        return sendMessageToServer(tr("Failed, Property with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString PropSandwichShellWapper::createProperty(SandwichShell* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    bool ok = obj->createProperty(name, static_cast<int>(Radioss::FITKPropAbstract::Sandwich));
    if (!ok)
    {
        return sendMessageToServer(tr("Creation failed! Property with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, SandwichShell property created with name %1").arg(name));
}

QString PropSandwichShellWapper::setIshell(SandwichShell* obj, int value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIshell(value);
    return sendMessageToServer(tr("Set SandwichShell Ishell successfully"));
}

QString PropSandwichShellWapper::setIsmstr(SandwichShell* obj, int value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIsmstr(value);
    return sendMessageToServer(tr("Set SandwichShell Ismstr successfully"));
}

QString PropSandwichShellWapper::setIsh3n(SandwichShell* obj, int value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIsh3n(value);
    return sendMessageToServer(tr("Set SandwichShell Ish3n successfully"));
}

QString PropSandwichShellWapper::setIdril(SandwichShell* obj, int value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIdril(value);
    return sendMessageToServer(tr("Set SandwichShell Idril successfully"));
}

QString PropSandwichShellWapper::setPthickFail(SandwichShell* obj, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setPThickfail(value);
    return sendMessageToServer(tr("Set SandwichShell PthickFail successfully"));
}

QString PropSandwichShellWapper::setHm(SandwichShell* obj, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setHm(value);
    return sendMessageToServer(tr("Set SandwichShell Hm successfully"));
}

QString PropSandwichShellWapper::setHf(SandwichShell* obj, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setHf(value);
    return sendMessageToServer(tr("Set SandwichShell Hf successfully"));
}

QString PropSandwichShellWapper::setHr(SandwichShell* obj, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setHr(value);
    return sendMessageToServer(tr("Set SandwichShell Hr successfully"));
}

QString PropSandwichShellWapper::setDm(SandwichShell* obj, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setDm(value);
    return sendMessageToServer(tr("Set SandwichShell Dm successfully"));
}

QString PropSandwichShellWapper::setDn(SandwichShell* obj, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setDn(value);
    return sendMessageToServer(tr("Set SandwichShell Dn successfully"));
}

QString PropSandwichShellWapper::setNumOfLayers(SandwichShell* obj, int value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    if (value < 0)
    {
        return sendMessageToServer(tr("Failed !Number of layers invalid!"));
    }

    propValue->setN(value);
    return sendMessageToServer(tr("Set SandwichShell N successfully"));
}

QString PropSandwichShellWapper::setIstrain(SandwichShell* obj, int value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIstrain(value);
    return sendMessageToServer(tr("Set SandwichShell Istrain successfully"));
}

QString PropSandwichShellWapper::setThick(SandwichShell* obj, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setThick(value);
    return sendMessageToServer(tr("Set SandwichShell Thick successfully"));
}

QString PropSandwichShellWapper::setAshear(SandwichShell* obj, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setAshear(value);
    return sendMessageToServer(tr("Set SandwichShell Ashear successfully"));
}

QString PropSandwichShellWapper::setIthick(SandwichShell* obj, int value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIthick(value);
    return sendMessageToServer(tr("Set SandwichShell Ithick successfully"));
}

QString PropSandwichShellWapper::setIplas(SandwichShell* obj, int value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIplas(value);
    return sendMessageToServer(tr("Set SandwichShell Iplas successfully"));
}

QString PropSandwichShellWapper::setVx(SandwichShell* obj, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setVx(value);
    return sendMessageToServer(tr("Set SandwichShell Vx successfully"));
}

QString PropSandwichShellWapper::setVy(SandwichShell* obj, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setVy(value);
    return sendMessageToServer(tr("Set SandwichShell Vy successfully"));
}

QString PropSandwichShellWapper::setVz(SandwichShell* obj, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setVz(value);
    return sendMessageToServer(tr("Set SandwichShell Vz successfully"));
}

// QString PropSandwichShellWapper::setSkewID(SandwichShell* obj, int value)
// {
//     Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
//     if (propValue == nullptr)
//     {
//         return tr("Failed !No property value found!");
//     }

//     propValue->setSkewID(value);
//     return tr("Set SandwichShell SkewID successfully");
// }

QString PropSandwichShellWapper::setIorth(SandwichShell* obj, int value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIorth(value);
    return sendMessageToServer(tr("Set SandwichShell Iorth successfully"));
}

QString PropSandwichShellWapper::setIpos(SandwichShell* obj, bool value)
{
    Radioss::FITKPropSandwichShellValue* propValue = getSandwichShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIpos(value);
    return sendMessageToServer(tr("Set SandwichShell Ipos successfully"));
}

QString PropSandwichShellWapper::setLayerAngle(SandwichShell* obj, int index, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = nullptr;
    Radioss::SandwichShellLayerValue layerValue;
    int actualIndex = -1;
    if (!getSandwichShellLayerValue(obj, index, propValue, layerValue, actualIndex))
    {
        return sendMessageToServer(tr("Failed !Index is 1-based and out of range!"));
    }

    layerValue._angle = value;
    propValue->setLayerValue(layerValue, actualIndex);
    return sendMessageToServer(tr("Set SandwichShell Layer Angle successfully"));
}

QString PropSandwichShellWapper::setLayerThickness(SandwichShell* obj, int index, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = nullptr;
    Radioss::SandwichShellLayerValue layerValue;
    int actualIndex = -1;
    if (!getSandwichShellLayerValue(obj, index, propValue, layerValue, actualIndex))
    {
        return sendMessageToServer(tr("Failed !Index is 1-based and out of range!"));
    }

    layerValue._thickness = value;
    propValue->setLayerValue(layerValue, actualIndex);
    return sendMessageToServer(tr("Set SandwichShell Layer Thickness successfully"));
}

QString PropSandwichShellWapper::setLayerPositionZ(SandwichShell* obj, int index, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = nullptr;
    Radioss::SandwichShellLayerValue layerValue;
    int actualIndex = -1;
    if (!getSandwichShellLayerValue(obj, index, propValue, layerValue, actualIndex))
    {
        return sendMessageToServer(tr("Failed !Index is 1-based and out of range!"));
    }

    layerValue._positionZ = value;
    propValue->setLayerValue(layerValue, actualIndex);
    return sendMessageToServer(tr("Set SandwichShell Layer PositionZ successfully"));
}

QString PropSandwichShellWapper::setLayerMaterial(
    SandwichShell* obj,
    int index,
    const QString& materialName)
{
    Radioss::FITKPropSandwichShellValue* propValue = nullptr;
    Radioss::SandwichShellLayerValue layerValue;
    int actualIndex = -1;
    if (!getSandwichShellLayerValue(obj, index, propValue, layerValue, actualIndex))
    {
        return sendMessageToServer(tr("Failed !Index is 1-based and out of range!"));
    }

    Interface::FITKAbstractMaterial* material = getSandwichShellMaterial(materialName);
    if (material == nullptr)
    {
        return sendMessageToServer(tr("Failed !Material with name %1 not found!").arg(materialName));
    }

    layerValue._matID = material->getDataObjectID();
    propValue->setLayerValue(layerValue, actualIndex);
    return sendMessageToServer(tr("Set SandwichShell Layer Material successfully"));
}

QString PropSandwichShellWapper::setLayerFailureWeight(SandwichShell* obj, int index, double value)
{
    Radioss::FITKPropSandwichShellValue* propValue = nullptr;
    Radioss::SandwichShellLayerValue layerValue;
    int actualIndex = -1;
    if (!getSandwichShellLayerValue(obj, index, propValue, layerValue, actualIndex))
    {
        return sendMessageToServer(tr("Failed !Index is 1-based and out of range!"));
    }

    layerValue._relFWeight = value;
    propValue->setLayerValue(layerValue, actualIndex);
    return sendMessageToServer(tr("Set SandwichShell Layer Failure Weight successfully"));
}

SandwichShell::SandwichShell(const QString& name)
    : PyInterface::PyPropertyBase(name)
{
}

SandwichShell::SandwichShell()
    : PyInterface::PyPropertyBase()
{
}