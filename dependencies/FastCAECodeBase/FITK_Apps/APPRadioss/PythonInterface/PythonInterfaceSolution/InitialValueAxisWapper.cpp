/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "InitialValueAxisWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"

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


InitialValueAxis* InitialValueAxisWapper::new_InitialValueAxis()
{
    return new InitialValueAxis();
}

void InitialValueAxisWapper::delete_InitialValueAxis(InitialValueAxis* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

InitialValueAxis* InitialValueAxisWapper::static_InitialValueAxis_GetInitialValueAxis(const QString& name)
{
    return new InitialValueAxis(name);
}

QString InitialValueAxisWapper::getName(InitialValueAxis* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString InitialValueAxisWapper::setName(InitialValueAxis* obj, const QString& name)
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

QString InitialValueAxisWapper::createInitialValue(InitialValueAxis* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createInitialValue(name, (int)Radioss::FITKAbstractInitialField::AXIS))
    {
        return sendMessageToServer(tr("Creation failed! Initial value with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Initial Value Axis created with name %1").arg(obj->_initialValue->getDataObjectName()));
}

QString InitialValueAxisWapper::setValue(InitialValueAxis* obj, double x, double y, double z)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setValue(x, y, z))
    {
        return sendMessageToServer(tr("Failed !No initial value found!"));
    }

    return sendMessageToServer(tr("Success, Set Initial Value Axis translation velocity to (%1, %2, %3)").arg(x).arg(y).arg(z));
}

QString InitialValueAxisWapper::setNodeGroup(InitialValueAxis* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setNodeGroup(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found or initial value not created!").arg(name));
    }

    obj->updateRender();
    return sendMessageToServer(tr("Success, Set Initial Value Axis Node Group to %1").arg(name));
}

QString InitialValueAxisWapper::setDir(InitialValueAxis* obj, const QString& dir)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKInitialFieldAxis* axis = obj->getInitialValueAxis();
    if (axis == nullptr)
    {
        return sendMessageToServer(tr("Failed, Direction must be X, Y or Z and initial value must be AXIS type"));
    }

    QString upperDir = dir.trimmed().toUpper();
    if (upperDir != "X" && upperDir != "Y" && upperDir != "Z")
    {
        return sendMessageToServer(tr("Failed, Direction must be X, Y or Z and initial value must be AXIS type"));
    }

    axis->setDir(upperDir);

    return sendMessageToServer(tr("Success, Set Initial Value Axis direction to %1").arg(upperDir));
}

// QString InitialValueAxisWapper::setFrameID(InitialValueAxis* obj, int id)
// {
//     if (obj == nullptr)
//     {
//         return tr("Failed, object is null");
//     }

//     Radioss::FITKInitialFieldAxis* axis = obj->getInitialValueAxis();
//     if (axis == nullptr)
//     {
//         return tr("Failed, No AXIS initial value found!");
//     }

//     axis->setFrameID(id);

//     return tr("Success, Set Initial Value Axis Frame ID to %1").arg(id);
// }

QString InitialValueAxisWapper::setVr(InitialValueAxis* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKInitialFieldAxis* axis = obj->getInitialValueAxis();
    if (axis == nullptr)
    {
        return sendMessageToServer(tr("Failed, No AXIS initial value found!"));
    }

    axis->setVr(value);

    return sendMessageToServer(tr("Success, Set Initial Value Axis rotational velocity to %1").arg(value));
}

InitialValueAxis::InitialValueAxis(const QString& name) :
    PyInterface::PyInitialValueBase(name)
{
}

InitialValueAxis::InitialValueAxis() :
    PyInterface::PyInitialValueBase()
{
}

Radioss::FITKInitialFieldAxis* InitialValueAxis::getInitialValueAxis()
{
    return dynamic_cast<Radioss::FITKInitialFieldAxis*>(getInitialValue());
}