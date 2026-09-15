/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "RWallCylinderWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"

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

RWallCylinder* RWallCylinderWapper::new_RWallCylinder()
{
    return new RWallCylinder();
}

void RWallCylinderWapper::delete_RWallCylinder(RWallCylinder* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

RWallCylinder* RWallCylinderWapper::static_RWallCylinder_GetRWallCylinder(const QString& name)
{
    return new RWallCylinder(name);
}

QString RWallCylinderWapper::getName(RWallCylinder* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString RWallCylinderWapper::setName(RWallCylinder* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, RWall cylinder with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString RWallCylinderWapper::createRWall(RWallCylinder* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createRWall(name, static_cast<int>(Radioss::FITKAbstractRWall::RWallCYL)))
    {
        return sendMessageToServer(tr("Creation failed! RWall cylinder with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, RWall cylinder created with name %1").arg(obj->getName()));
}

// void RWallCylinderWapper::setNodeId(RWallCylinder* obj, int value)
// {
//     if (obj == nullptr || obj->getRWall() == nullptr)
//     {
//         AppFrame::FITKMessageError("Set RWall Cylinder Node Id failed!");
//         return;
//     }

//     obj->getRWall()->setNodeId(value);
//     obj->updateRender();
// }

QString RWallCylinderWapper::setSlidingType(RWallCylinder* obj, int type)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setSlidingType(type))
    {
        return sendMessageToServer(tr("Failed !Sliding type invalid or rwall not created!"));
    }

    return sendMessageToServer(tr("Set RWall Cylinder Sliding Type successfully"));
}

QString RWallCylinderWapper::setDSearch(RWallCylinder* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setDSearch(value))
    {
        return sendMessageToServer(tr("Failed !DSearch invalid or rwall not created!"));
    }

    return sendMessageToServer(tr("Set RWall Cylinder DSearch successfully"));
}

QString RWallCylinderWapper::setFrictionCoef(RWallCylinder* obj, double value)
{
    if (obj == nullptr || obj->getRWall() == nullptr)
    {
        return sendMessageToServer(tr("Failed !No rwall found!"));
    }

    obj->getRWall()->setFrictionCoef(value);

    return sendMessageToServer(tr("Set RWall Cylinder Friction Coef successfully"));
}

QString RWallCylinderWapper::setDiameter(RWallCylinder* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKRWallCylinder* rWall = dynamic_cast<Radioss::FITKRWallCylinder*>(obj->getRWall());
    if (rWall == nullptr)
    {
        return sendMessageToServer(tr("Failed !No rwall cylinder found!"));
    }

    rWall->setDiameter(value);
    obj->updateRender();

    return sendMessageToServer(tr("Set RWall Cylinder Diameter successfully"));
}

QString RWallCylinderWapper::setFilteringFactor(RWallCylinder* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setFilteringFactor(value))
    {
        return sendMessageToServer(tr("Failed !Filtering factor invalid or rwall not created!"));
    }

    return sendMessageToServer(tr("Set RWall Cylinder Filtering Factor successfully"));
}

QString RWallCylinderWapper::setFilteringFlag(RWallCylinder* obj, int value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setFilteringFlag(value))
    {
        return sendMessageToServer(tr("Failed !Filtering flag invalid or rwall not created!"));
    }

    return sendMessageToServer(tr("Set RWall Cylinder Filtering Flag successfully"));
}

QString RWallCylinderWapper::setNodeGroup1(RWallCylinder* obj, const QString& name)
{
    if (obj == nullptr || obj->getRWall() == nullptr)
    {
        return sendMessageToServer(tr("Failed !No rwall found!"));
    }

    if (name.isEmpty() || name == "None")
    {
        obj->getRWall()->setNodeGroupId1(0);
        obj->updateRender();
        return sendMessageToServer(tr("Clear RWall Cylinder Node Group1 successfully"));
    }

    if (!obj->setNodeGroup1(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found!").arg(name));
    }

    return sendMessageToServer(tr("Set RWall Cylinder Node Group1 successfully"));
}

QString RWallCylinderWapper::setNodeGroup2(RWallCylinder* obj, const QString& name)
{
    if (obj == nullptr || obj->getRWall() == nullptr)
    {
        return sendMessageToServer(tr("Failed !No rwall found!"));
    }

    if (name.isEmpty() || name == "None")
    {
        obj->getRWall()->setNodeGroupId2(0);
        obj->updateRender();
        return sendMessageToServer(tr("Clear RWall Cylinder Node Group2 successfully"));
    }

    if (!obj->setNodeGroup2(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found!").arg(name));
    }

    return sendMessageToServer(tr("Set RWall Cylinder Node Group2 successfully"));
}

QString RWallCylinderWapper::setBasicPoint(RWallCylinder* obj, double x, double y, double z)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKRWallCylinder* rWall = dynamic_cast<Radioss::FITKRWallCylinder*>(obj->getRWall());
    if (rWall == nullptr)
    {
        return sendMessageToServer(tr("Failed !No rwall cylinder found!"));
    }

    rWall->setBasicPoint(x, y, z);
    obj->updateRender();

    return sendMessageToServer(tr("Set RWall Cylinder Basic Point successfully"));
}

QString RWallCylinderWapper::setAxisVector(RWallCylinder* obj, double nx, double ny, double nz)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKRWallCylinder* rWall = dynamic_cast<Radioss::FITKRWallCylinder*>(obj->getRWall());
    if (rWall == nullptr)
    {
        return sendMessageToServer(tr("Failed !No rwall cylinder found!"));
    }

    rWall->setAxisVector(nx, ny, nz);
    obj->updateRender();

    return sendMessageToServer(tr("Set RWall Cylinder Axis Vector successfully"));
}

RWallCylinder::RWallCylinder(const QString& name) :
    PyInterface::PyRWallBase(name)
{
}

RWallCylinder::RWallCylinder() :
    PyInterface::PyRWallBase()
{
}