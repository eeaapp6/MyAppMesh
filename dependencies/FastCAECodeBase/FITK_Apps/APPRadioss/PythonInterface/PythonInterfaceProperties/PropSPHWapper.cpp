/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PropSPHWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKPropSPH.h"

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

    Radioss::FITKPropSPH* getSPHProperty(SPH* obj)
    {
        if (obj == nullptr)
        {
            return nullptr;
        }

        return dynamic_cast<Radioss::FITKPropSPH*>(obj->getProperty());
    }

    Radioss::FITKPropSPHValue* getSPHValue(SPH* obj)
    {
        Radioss::FITKPropSPH* property = getSPHProperty(obj);
        if (property == nullptr)
        {
            return nullptr;
        }

        return property->getValue();
    }
}

SPH* PropSPHWapper::new_SPH()
{
    return new SPH();
}

void PropSPHWapper::delete_SPH(SPH* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

SPH* PropSPHWapper::static_SPH_GetSPH(const QString& name)
{
    return new SPH(name);
}

QString PropSPHWapper::getName(SPH* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString PropSPHWapper::setName(SPH* obj, const QString& name)
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

QString PropSPHWapper::createProperty(SPH* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    bool ok = obj->createProperty(name, static_cast<int>(Radioss::FITKPropAbstract::SPH));
    if (!ok)
    {
        return sendMessageToServer(tr("Creation failed! Property with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, SPH property created with name %1").arg(name));
}

QString PropSPHWapper::setMP(SPH* obj, double value)
{
    Radioss::FITKPropSPHValue* propValue = getSPHValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setMP(value);
    return sendMessageToServer(tr("Set SPH MP successfully"));
}

QString PropSPHWapper::setQA(SPH* obj, double value)
{
    Radioss::FITKPropSPHValue* propValue = getSPHValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setQA(value);
    return sendMessageToServer(tr("Set SPH QA successfully"));
}

QString PropSPHWapper::setQB(SPH* obj, double value)
{
    Radioss::FITKPropSPHValue* propValue = getSPHValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setQB(value);
    return sendMessageToServer(tr("Set SPH QB successfully"));
}

QString PropSPHWapper::setACS(SPH* obj, double value)
{
    Radioss::FITKPropSPHValue* propValue = getSPHValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setACS(value);
    return sendMessageToServer(tr("Set SPH ACS successfully"));
}

// void PropSPHWapper::setSkewID(SPH* obj, int value)
// {
//     Radioss::FITKPropSPHValue* propValue = getSPHValue(obj);
//     if (propValue == nullptr)
//     {
//         AppFrame::FITKMessageError("Set SPH SkewID failed !");
//         return;
//     }

//     propValue->setSkewID(value);
// }

QString PropSPHWapper::setHID(SPH* obj, int value)
{
    Radioss::FITKPropSPHValue* propValue = getSPHValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setHID(value);
    return sendMessageToServer(tr("Set SPH HID successfully"));
}

QString PropSPHWapper::setOrderID(SPH* obj, int value)
{
    Radioss::FITKPropSPHValue* propValue = getSPHValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setOrderID(value);
    return sendMessageToServer(tr("Set SPH OrderID successfully"));
}

QString PropSPHWapper::setH(SPH* obj, double value)
{
    Radioss::FITKPropSPHValue* propValue = getSPHValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setH(value);
    return sendMessageToServer(tr("Set SPH H successfully"));
}

QString PropSPHWapper::setStab(SPH* obj, double value)
{
    Radioss::FITKPropSPHValue* propValue = getSPHValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setStab(value);
    return sendMessageToServer(tr("Set SPH Stab successfully"));
}

SPH::SPH(const QString& name)
    : PyInterface::PyPropertyBase(name)
{
}

SPH::SPH()
    : PyInterface::PyPropertyBase()
{
}