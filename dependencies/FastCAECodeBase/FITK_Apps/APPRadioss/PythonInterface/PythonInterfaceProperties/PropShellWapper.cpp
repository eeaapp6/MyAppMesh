/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PropShellWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKPropShell.h"

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

    Radioss::FITKPropShell* getShellProperty(Shell* obj)
    {
        if (obj == nullptr)
        {
            return nullptr;
        }

        return dynamic_cast<Radioss::FITKPropShell*>(obj->getProperty());
    }

    Radioss::FITKPropShellValue* getShellValue(Shell* obj)
    {
        Radioss::FITKPropShell* property = getShellProperty(obj);
        if (property == nullptr)
        {
            return nullptr;
        }

        return property->getValue();
    }
}

Shell* PropShellWapper::new_Shell()
{
    return new Shell();
}

void PropShellWapper::delete_Shell(Shell* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

Shell* PropShellWapper::static_Shell_GetShell(const QString& name)
{
    return new Shell(name);
}

QString PropShellWapper::getName(Shell* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString PropShellWapper::setName(Shell* obj, const QString& name)
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

QString PropShellWapper::createProperty(Shell* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    bool ok = obj->createProperty(name, static_cast<int>(Radioss::FITKPropAbstract::Shell));
    if (!ok)
    {
        return sendMessageToServer(tr("Creation failed! Property with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Shell property created with name %1").arg(name));
}

QString PropShellWapper::setIpinch(Shell* obj, int value)
{
    Radioss::FITKPropShell* property = getShellProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setIpinch(value);

    return sendMessageToServer(tr("Set Shell Ipinch successfully"));
}

QString PropShellWapper::setPthickFail(Shell* obj, double value)
{
    Radioss::FITKPropShell* property = getShellProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setPthickFail(value);

    return sendMessageToServer(tr("Set Shell PthickFail successfully"));
}

QString PropShellWapper::setHm(Shell* obj, double value)
{
    Radioss::FITKPropShell* property = getShellProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setHm(value);

    return sendMessageToServer(tr("Set Shell Hm successfully"));
}

QString PropShellWapper::setHf(Shell* obj, double value)
{
    Radioss::FITKPropShell* property = getShellProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setHf(value);

    return sendMessageToServer(tr("Set Shell Hf successfully"));
}

QString PropShellWapper::setHr(Shell* obj, double value)
{
    Radioss::FITKPropShell* property = getShellProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setHr(value);

    return sendMessageToServer(tr("Set Shell Hr successfully"));
}

QString PropShellWapper::setDm(Shell* obj, double value)
{
    Radioss::FITKPropShell* property = getShellProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setDm(value);

    return sendMessageToServer(tr("Set Shell Dm successfully"));
}

QString PropShellWapper::setDn(Shell* obj, double value)
{
    Radioss::FITKPropShell* property = getShellProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setDn(value);

    return sendMessageToServer(tr("Set Shell Dn successfully"));
}

QString PropShellWapper::setN(Shell* obj, int value)
{
    Radioss::FITKPropShell* property = getShellProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setN(value);

    return sendMessageToServer(tr("Set Shell N successfully"));
}

QString PropShellWapper::setThick(Shell* obj, double value)
{
    Radioss::FITKPropShell* property = getShellProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setThick(value);

    return sendMessageToServer(tr("Set Shell Thick successfully"));
}

QString PropShellWapper::setAshear(Shell* obj, double value)
{
    Radioss::FITKPropShell* property = getShellProperty(obj);
    if (property == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property found!"));
    }

    property->setAshear(value);

    return sendMessageToServer(tr("Set Shell Ashear successfully"));
}

QString PropShellWapper::setIshell(Shell* obj, int value)
{
    Radioss::FITKPropShellValue* propValue = getShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIshell(value);

    return sendMessageToServer(tr("Set Shell Ishell successfully"));
}

QString PropShellWapper::setIsmstr(Shell* obj, int value)
{
    Radioss::FITKPropShellValue* propValue = getShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIsmstr(value);

    return sendMessageToServer(tr("Set Shell Ismstr successfully"));
}

QString PropShellWapper::setIthick(Shell* obj, int value)
{
    Radioss::FITKPropShellValue* propValue = getShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIthick(value);

    return sendMessageToServer(tr("Set Shell Ithick successfully"));
}

QString PropShellWapper::setIplas(Shell* obj, int value)
{
    Radioss::FITKPropShellValue* propValue = getShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIplas(value);

    return sendMessageToServer(tr("Set Shell Iplas successfully"));
}

QString PropShellWapper::setIsh3n(Shell* obj, int value)
{
    Radioss::FITKPropShellValue* propValue = getShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIsh3n(value);

    return sendMessageToServer(tr("Set Shell Ish3n successfully"));
}

QString PropShellWapper::setIdril(Shell* obj, int value)
{
    Radioss::FITKPropShellValue* propValue = getShellValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIdril(value);

    return sendMessageToServer(tr("Set Shell Idril successfully"));
}

Shell::Shell(const QString& name)
    : PyInterface::PyPropertyBase(name)
{
}

Shell::Shell()
    : PyInterface::PyPropertyBase()
{
}