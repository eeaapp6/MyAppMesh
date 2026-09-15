/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "InitialValueTraWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInitialField.h"

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
}

InitialValueTra* InitialValueTraWapper::new_InitialValueTra()
{
    return new InitialValueTra();
}

void InitialValueTraWapper::delete_InitialValueTra(InitialValueTra* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

InitialValueTra* InitialValueTraWapper::static_InitialValueTra_GetInitialValueTra(const QString& name)
{
    return new InitialValueTra(name);
}

QString InitialValueTraWapper::getName(InitialValueTra* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString InitialValueTraWapper::setName(InitialValueTra* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, Initial value with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(obj->_initialValue->getDataObjectName()));
}

QString InitialValueTraWapper::createInitialValue(InitialValueTra* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createInitialValue(name, (int)Radioss::FITKAbstractInitialField::TRA))
    {
        return sendMessageToServer(tr("Creation failed! Initial value with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Initial Value Tra created with name %1").arg(obj->_initialValue->getDataObjectName()));
}

QString InitialValueTraWapper::setValue(InitialValueTra* obj, double x, double y, double z)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setValue(x, y, z))
    {
        return sendMessageToServer(tr("Failed !No initial value found!"));
    }

    return sendMessageToServer(tr("Success, Set Initial Value Tra to (%1, %2, %3)").arg(x).arg(y).arg(z));
}

QString InitialValueTraWapper::setNodeGroup(InitialValueTra* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setNodeGroup(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found or initial value not created!").arg(name));
    }

    return sendMessageToServer(tr("Success, Set Initial Value Tra Node Group to %1").arg(name));
}

InitialValueTra::InitialValueTra(const QString& name) :
    PyInterface::PyInitialValueBase(name)
{
}

InitialValueTra::InitialValueTra() :
    PyInterface::PyInitialValueBase()
{
}