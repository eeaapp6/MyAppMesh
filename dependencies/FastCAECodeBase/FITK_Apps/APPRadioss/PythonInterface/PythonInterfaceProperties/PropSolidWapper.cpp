/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PropSolidWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKPropSolid.h"

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

    Radioss::FITKPropSolid* getSolidProperty(Solid* obj)
    {
        if (obj == nullptr)
        {
            return nullptr;
        }

        return dynamic_cast<Radioss::FITKPropSolid*>(obj->getProperty());
    }

    Radioss::FITKPropSolidValue* getSolidValue(Solid* obj)
    {
        Radioss::FITKPropSolid* property = getSolidProperty(obj);
        if (property == nullptr)
        {
            return nullptr;
        }

        return property->getValue();
    }
}

Solid* PropSolidWapper::new_Solid()
{
    return new Solid();
}

void PropSolidWapper::delete_Solid(Solid* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

Solid* PropSolidWapper::static_Solid_GetSolid(const QString& name)
{
    return new Solid(name);
}

QString PropSolidWapper::getName(Solid* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString PropSolidWapper::setName(Solid* obj, const QString& name)
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

QString PropSolidWapper::createProperty(Solid* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    bool ok = obj->createProperty(name, static_cast<int>(Radioss::FITKPropAbstract::Solid));
    if (!ok)
    {
        return sendMessageToServer(tr("Creation failed! Property with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Solid property created with name %1").arg(name));
}

QString PropSolidWapper::setDn(Solid* obj, double val)
{
    Radioss::FITKPropSolid* property = getSolidProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setDn(val);
    return sendMessageToServer(tr("Set Solid Dn successfully"));
}

QString PropSolidWapper::setQa(Solid* obj, double val)
{
    Radioss::FITKPropSolid* property = getSolidProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setQa(val);
    return sendMessageToServer(tr("Set Solid Qa successfully"));
}

QString PropSolidWapper::setQb(Solid* obj, double val)
{
    Radioss::FITKPropSolid* property = getSolidProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setQb(val);
    return sendMessageToServer(tr("Set Solid Qb successfully"));
}

QString PropSolidWapper::setH(Solid* obj, double val)
{
    Radioss::FITKPropSolid* property = getSolidProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setH(val);
    return sendMessageToServer(tr("Set Solid H successfully"));
}

QString PropSolidWapper::setLambdaV(Solid* obj, double val)
{
    Radioss::FITKPropSolid* property = getSolidProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setLambdaV(val);
    return sendMessageToServer(tr("Set Solid LambdaV successfully"));
}

QString PropSolidWapper::setMuV(Solid* obj, double val)
{
    Radioss::FITKPropSolid* property = getSolidProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setMuV(val);
    return sendMessageToServer(tr("Set Solid MuV successfully"));
}

QString PropSolidWapper::setMinTimeStep(Solid* obj, double val)
{
    Radioss::FITKPropSolid* property = getSolidProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setMinTimeStep(val);
    return sendMessageToServer(tr("Set Solid MinTimeStep successfully"));
}

QString PropSolidWapper::setNdir(Solid* obj, int val)
{
    Radioss::FITKPropSolid* property = getSolidProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setNdir(val);
    return sendMessageToServer(tr("Set Solid Ndir successfully"));
}

// void PropSolidWapper::setSPHPartID(Solid* obj, int val)
// {
//     Radioss::FITKPropSolid* property = getSolidProperty(obj);
//     if (property == nullptr)
//     {
//         AppFrame::FITKMessageError("Set Solid SPHPartID failed !");
//         return;
//     }

//     property->setSPHPartID(val);
// }

QString PropSolidWapper::setIsolid(Solid* obj, int val)
{
    Radioss::FITKPropSolidValue* value = getSolidValue(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    value->setIsolid(val);
    return sendMessageToServer(tr("Set Solid Isolid successfully"));
}

QString PropSolidWapper::setIsmstr(Solid* obj, int val)
{
    Radioss::FITKPropSolidValue* value = getSolidValue(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    value->setIsmstr(val);
    return sendMessageToServer(tr("Set Solid Ismstr successfully"));
}

QString PropSolidWapper::setIcpre(Solid* obj, int val)
{
    Radioss::FITKPropSolidValue* value = getSolidValue(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    value->setIcpre(val);
    return sendMessageToServer(tr("Set Solid Icpre successfully"));
}

QString PropSolidWapper::setItetra4(Solid* obj, int val)
{
    Radioss::FITKPropSolidValue* value = getSolidValue(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    value->setItetra4(val);
    return sendMessageToServer(tr("Set Solid Itetra4 successfully"));
}

QString PropSolidWapper::setItetra10(Solid* obj, int val)
{
    Radioss::FITKPropSolidValue* value = getSolidValue(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    value->setItetra10(val);
    return sendMessageToServer(tr("Set Solid Itetra10 successfully"));
}

QString PropSolidWapper::setImas(Solid* obj, int val)
{
    Radioss::FITKPropSolidValue* value = getSolidValue(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    value->setImas(val);
    return sendMessageToServer(tr("Set Solid Imas successfully"));
}

QString PropSolidWapper::setIFrame(Solid* obj, int val)
{
    Radioss::FITKPropSolidValue* value = getSolidValue(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    value->setIFrame(val);
    return sendMessageToServer(tr("Set Solid IFrame successfully"));
}

Solid::Solid(const QString& name)
    : PyInterface::PyPropertyBase(name)
{
}

Solid::Solid()
    : PyInterface::PyPropertyBase()
{
}