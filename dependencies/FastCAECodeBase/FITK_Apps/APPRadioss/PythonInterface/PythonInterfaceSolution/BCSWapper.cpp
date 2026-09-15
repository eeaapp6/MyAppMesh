/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "BCSWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"

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

BCS::BCS(const QString& name) :
    PyInterface::PyBCSBase(name)
{
}

BCS::BCS() :
    PyInterface::PyBCSBase()
{
}

Radioss::FITKRadiossBCS* BCS::getBCS()
{
    return dynamic_cast<Radioss::FITKRadiossBCS*>(PyInterface::PyBCSBase::getBCS());
}

BCS* BCSWapper::new_BCS()
{
    return new BCS();
}

void BCSWapper::delete_BCS(BCS* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

BCS* BCSWapper::static_BCS_GetBCS(const QString& name)
{
    return new BCS(name);
}

QString BCSWapper::getName(BCS* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString BCSWapper::setName(BCS* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, BCS with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString BCSWapper::createBCS(BCS* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createBCS(name, static_cast<int>(Radioss::FITKAbstractBCS::BCS)))
    {
        return sendMessageToServer(tr("Creation failed! BCS with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, BCS created with name %1").arg(obj->getName()));
}

// QString BCSWapper::setCoordinateSystemID(BCS* obj, int id)
// {
//     if (obj == nullptr)
//     {
//         return tr("Failed, object is null");
//     }

//     if (!obj->setCoordinateSystemID(id))
//     {
//         return tr("Failed, No BCS found!");
//     }

//     return tr("Success, Set BCS coordinate system ID to %1").arg(id);
// }

QString BCSWapper::setNodeGroup(BCS* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setNodeGroup1(name))
    {
        return sendMessageToServer(tr("Failed, Node group with name %1 not found or BCS not created!").arg(name));
    }

    return sendMessageToServer(tr("Success, Set BCS Node Group to %1").arg(name));
}

QString BCSWapper::setFreedomFixed(BCS* obj, int index, bool fixed)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKRadiossBCS* bcs = obj->getBCS();
    if (bcs == nullptr)
    {
        return sendMessageToServer(tr("Failed, No BCS found!"));
    }

    if (index < 1 || index > 6)
    {
        return sendMessageToServer(tr("Failed, BCS not found or freedom index %1 is invalid!").arg(index));
    }

    bcs->setFreedomFixed(index, fixed);
    obj->updateRender();

    return sendMessageToServer(tr("Success, Set BCS freedom %1 to %2").arg(index).arg(fixed));
}