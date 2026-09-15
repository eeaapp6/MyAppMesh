/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PropBeamWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeam.h"

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

    int getBeamDirectionIndex(const QString& direction)
    {
        QString normalizedDirection = direction.trimmed().toLower();
        if (normalizedDirection == "x")
        {
            return 0;
        }

        if (normalizedDirection == "y")
        {
            return 1;
        }

        if (normalizedDirection == "z")
        {
            return 2;
        }

        return -1;
    }

    Radioss::FITKPropBeam* getBeamProperty(Beam* obj)
    {
        if (obj == nullptr)
        {
            return nullptr;
        }

        return dynamic_cast<Radioss::FITKPropBeam*>(obj->getProperty());
    }

    Radioss::FITKPropValueBeam* getBeamValue(Beam* obj)
    {
        Radioss::FITKPropBeam* property = getBeamProperty(obj);
        if (property == nullptr)
        {
            return nullptr;
        }

        return property->getValue();
    }
}

Beam* PropBeamWapper::new_Beam()
{
    return new Beam();
}

void PropBeamWapper::delete_Beam(Beam* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

Beam* PropBeamWapper::static_Beam_GetBeam(const QString& name)
{
    return new Beam(name);
}

QString PropBeamWapper::getName(Beam* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString PropBeamWapper::setName(Beam* obj, const QString& name)
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

QString PropBeamWapper::createProperty(Beam* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    bool ok = obj->createProperty(name, static_cast<int>(Radioss::FITKPropAbstract::Beam));
    if (!ok)
    {
        return sendMessageToServer(tr("Creation failed! Property with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Beam property created with name %1").arg(name));
}

QString PropBeamWapper::setSmallStrainOption(Beam* obj, int value)
{
    Radioss::FITKPropValueBeam* propValue = getBeamValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setSmallStrainOption(value);
    return sendMessageToServer(tr("Set Beam SmallStrainOption successfully"));
}

QString PropBeamWapper::setBeamMembraneDamping(Beam* obj, double value)
{
    Radioss::FITKPropValueBeam* propValue = getBeamValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setBeamMembraneDamping(value);
    return sendMessageToServer(tr("Set Beam MembraneDamping successfully"));
}

QString PropBeamWapper::setBeamFlexuralDamping(Beam* obj, double value)
{
    Radioss::FITKPropValueBeam* propValue = getBeamValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setBeamFlexuralDamping(value);
    return sendMessageToServer(tr("Set Beam FlexuralDamping successfully"));
}

QString PropBeamWapper::setCrossSection(Beam* obj, double value)
{
    Radioss::FITKPropValueBeam* propValue = getBeamValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setCrossSection(value);
    return sendMessageToServer(tr("Set Beam CrossSection successfully"));
}

QString PropBeamWapper::setAreaMomentInertiaIyy(Beam* obj, double value)
{
    Radioss::FITKPropValueBeam* propValue = getBeamValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setAreaMomentInertiaIyy(value);
    return sendMessageToServer(tr("Set Beam AreaMomentInertiaIyy successfully"));
}

QString PropBeamWapper::setAreaMomentInertiaIzz(Beam* obj, double value)
{
    Radioss::FITKPropValueBeam* propValue = getBeamValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setAreaMomentInertiaIzz(value);
    return sendMessageToServer(tr("Set Beam AreaMomentInertiaIzz successfully"));
}

QString PropBeamWapper::setAreaMomentInertiaIxx(Beam* obj, double value)
{
    Radioss::FITKPropValueBeam* propValue = getBeamValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setAreaMomentInertiaIxx(value);
    return sendMessageToServer(tr("Set Beam AreaMomentInertiaIxx successfully"));
}

QString PropBeamWapper::setNeglectsShear(Beam* obj, bool isNeglects)
{
    Radioss::FITKPropValueBeam* propValue = getBeamValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setNeglectsShear(isNeglects);
    return sendMessageToServer(tr("Set Beam NeglectsShear successfully"));
}

QString PropBeamWapper::setRotationDOF1(Beam* obj, const QString& direction, bool isEnable)
{
    Radioss::FITKPropValueBeam* propValue = getBeamValue(obj);
    int index = getBeamDirectionIndex(direction);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    if (index < 0)
    {
        return sendMessageToServer(tr("Failed !Direction invalid!"));
    }

    propValue->setRotationDOF1(index, isEnable);
    return sendMessageToServer(tr("Set Beam RotationDOF1 successfully"));
}

QString PropBeamWapper::setRotationDOF2(Beam* obj, const QString& direction, bool isEnable)
{
    Radioss::FITKPropValueBeam* propValue = getBeamValue(obj);
    int index = getBeamDirectionIndex(direction);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    if (index < 0)
    {
        return sendMessageToServer(tr("Failed !Direction invalid!"));
    }

    propValue->setRotationDOF2(index, isEnable);
    return sendMessageToServer(tr("Set Beam RotationDOF2 successfully"));
}

Beam::Beam(const QString& name)
    : PyInterface::PyPropertyBase(name)
{
}

Beam::Beam()
    : PyInterface::PyPropertyBase()
{
}