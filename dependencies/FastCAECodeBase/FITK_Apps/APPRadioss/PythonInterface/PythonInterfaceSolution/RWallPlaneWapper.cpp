/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "RWallPlaneWapper.h"

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

RWallPlane* RWallPlaneWapper::new_RWallPlane()
{
    return new RWallPlane();
}

void RWallPlaneWapper::delete_RWallPlane(RWallPlane* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

RWallPlane* RWallPlaneWapper::static_RWallPlane_GetRWallPlane(const QString& name)
{
    return new RWallPlane(name);
}

QString RWallPlaneWapper::getName(RWallPlane* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString RWallPlaneWapper::setName(RWallPlane* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, RWall plane with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString RWallPlaneWapper::createRWall(RWallPlane* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createRWall(name, static_cast<int>(Radioss::FITKAbstractRWall::RWallPLANE)))
    {
        return sendMessageToServer(tr("Creation failed! RWall plane with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, RWall plane created with name %1").arg(obj->getName()));
}

QString RWallPlaneWapper::setSlidingType(RWallPlane* obj, int type)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setSlidingType(type))
    {
        return sendMessageToServer(tr("Failed !Sliding type invalid or rwall not created!"));
    }

    return sendMessageToServer(tr("Set RWall Plane Sliding Type successfully"));
}

QString RWallPlaneWapper::setDSearch(RWallPlane* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setDSearch(value))
    {
        return sendMessageToServer(tr("Failed !DSearch invalid or rwall not created!"));
    }

    return sendMessageToServer(tr("Set RWall Plane DSearch successfully"));
}

QString RWallPlaneWapper::setFrictionCoef(RWallPlane* obj, double value)
{
    if (obj == nullptr || obj->getRWall() == nullptr)
    {
        return sendMessageToServer(tr("Failed !No rwall found!"));
    }

    obj->getRWall()->setFrictionCoef(value);

    return sendMessageToServer(tr("Set RWall Plane Friction Coef successfully"));
}

QString RWallPlaneWapper::setFilteringFactor(RWallPlane* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setFilteringFactor(value))
    {
        return sendMessageToServer(tr("Failed !Filtering factor invalid or rwall not created!"));
    }

    return sendMessageToServer(tr("Set RWall Plane Filtering Factor successfully"));
}

QString RWallPlaneWapper::setFilteringFlag(RWallPlane* obj, int value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setFilteringFlag(value))
    {
        return sendMessageToServer(tr("Failed !Filtering flag invalid or rwall not created!"));
    }

    return sendMessageToServer(tr("Set RWall Plane Filtering Flag successfully"));
}

QString RWallPlaneWapper::setNodeGroup1(RWallPlane* obj, const QString& name)
{
    if (obj == nullptr || obj->getRWall() == nullptr)
    {
        return sendMessageToServer(tr("Failed !No rwall found!"));
    }

    if (name.isEmpty() || name == "None")
    {
        obj->getRWall()->setNodeGroupId1(0);
        obj->updateRender();
        return sendMessageToServer(tr("Clear RWall Plane Node Group1 successfully"));
    }

    if (!obj->setNodeGroup1(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found!").arg(name));
    }

    return sendMessageToServer(tr("Set RWall Plane Node Group1 successfully"));
}

QString RWallPlaneWapper::setNodeGroup2(RWallPlane* obj, const QString& name)
{
    if (obj == nullptr || obj->getRWall() == nullptr)
    {
        return sendMessageToServer(tr("Failed !No rwall found!"));
    }

    if (name.isEmpty() || name == "None")
    {
        obj->getRWall()->setNodeGroupId2(0);
        obj->updateRender();
        return sendMessageToServer(tr("Clear RWall Plane Node Group2 successfully"));
    }

    if (!obj->setNodeGroup2(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found!").arg(name));
    }

    return sendMessageToServer(tr("Set RWall Plane Node Group2 successfully"));
}

QString RWallPlaneWapper::setBasicPoint(RWallPlane* obj, double x, double y, double z)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKRWallPlane* rWall = dynamic_cast<Radioss::FITKRWallPlane*>(obj->getRWall());
    if (rWall == nullptr)
    {
        return sendMessageToServer(tr("Failed !No rwall plane found!"));
    }

    rWall->setBasicPoint(x, y, z);
    obj->updateRender();

    return sendMessageToServer(tr("Set RWall Plane Basic Point successfully"));
}

QString RWallPlaneWapper::setNormalVector(RWallPlane* obj, double nx, double ny, double nz)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKRWallPlane* rWall = dynamic_cast<Radioss::FITKRWallPlane*>(obj->getRWall());
    if (rWall == nullptr)
    {
        return sendMessageToServer(tr("Failed !No rwall plane found!"));
    }

    rWall->setNormalVector(nx, ny, nz);
    obj->updateRender();

    return sendMessageToServer(tr("Set RWall Plane Normal Vector successfully"));
}

RWallPlane::RWallPlane(const QString& name) :
    PyInterface::PyRWallBase(name)
{
}

RWallPlane::RWallPlane() :
    PyInterface::PyRWallBase()
{
}