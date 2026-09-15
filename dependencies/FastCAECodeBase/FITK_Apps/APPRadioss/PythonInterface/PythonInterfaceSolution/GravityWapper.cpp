/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GravityWapper.h"

#include "SolutionWapper.h"

#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"

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

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

Gravity::Gravity(const QString& name)
{
    Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
    if (solution == nullptr)
    {
        return;
    }

    _gravityManager = solution->getGravityManager();
    if (_gravityManager == nullptr)
    {
        return;
    }

    _gravity = _gravityManager->getDataByName(name);
}

Gravity::Gravity()
{
    Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
    if (solution == nullptr)
    {
        return;
    }

    _gravityManager = solution->getGravityManager();
}

QString Gravity::getName()
{
    if (_gravity == nullptr)
    {
        return QString();
    }

    return _gravity->getDataObjectName();
}

bool Gravity::setName(const QString& name)
{
    if (name == "" || _gravity == nullptr || _gravityManager == nullptr)
    {
        return false;
    }

    if (_gravityManager->getDataByName(name) != nullptr && _gravity->getDataObjectName() != name)
    {
        return false;
    }

    _gravity->setDataObjectName(name);
    updateRender();
    return true;
}

bool Gravity::createGravity(const QString& name)
{
    if (_gravityManager == nullptr)
    {
        return false;
    }

    QString g_name = name;
    if (g_name.isEmpty())
    {
        g_name = _gravityManager->checkName("Gravity-1");
    }

    if (_gravityManager->getDataByName(g_name) != nullptr)
    {
        return false;
    }

    _gravity = new Radioss::FITKGravity();
    _gravity->setDataObjectName(g_name);
    _gravityManager->appendDataObj(_gravity);
    updateRender();
    return true;
}

bool Gravity::setTimeFunction(const QString& name)
{
    if (_gravity == nullptr || name == "")
    {
        return false;
    }

    Radioss::FITKRadiossCase* radiossCase =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (radiossCase == nullptr)
    {
        return false;
    }

    Radioss::FITKNonLinerCurveManager* curveManager = radiossCase->getNonLinerCurveManager();
    if (curveManager == nullptr)
    {
        return false;
    }

    Radioss::FITKNonLinerCurve* curve = curveManager->getDataByName(name);
    if (curve == nullptr)
    {
        return false;
    }

    _gravity->setTimeFunctionID(curve->getDataObjectID());
    return true;
}

bool Gravity::setGravityDirection(const QString& direction)
{
    if (_gravity == nullptr)
    {
        return false;
    }

    QString directionText = direction.trimmed().toLower();
    Radioss::FITKGravity::GravityDirection gravityDirection = Radioss::FITKGravity::GRAVITY_Z;

    if (directionText == "x")
    {
        gravityDirection = Radioss::FITKGravity::GRAVITY_X;
    }
    else if (directionText == "y")
    {
        gravityDirection = Radioss::FITKGravity::GRAVITY_Y;
    }
    else if (directionText == "z")
    {
        gravityDirection = Radioss::FITKGravity::GRAVITY_Z;
    }
    else
    {
        return false;
    }

    _gravity->setGravityDirection(gravityDirection);
    updateRender();
    return true;
}

// bool Gravity::setSystemID(int id)
// {
//     if (_gravity == nullptr)
//     {
//         return false;
//     }
//
//     _gravity->setSystemID(id);
//     return true;
// }

// bool Gravity::setSensorID(int id)
// {
//     if (_gravity == nullptr)
//     {
//         return false;
//     }
//
//     _gravity->setSensorID(id);
//     return true;
// }

bool Gravity::setNodeGroup(const QString& name)
{
    if (_gravity == nullptr || name == "")
    {
        return false;
    }
    if (name == "Global")
    {
        _gravity->setNodeGroupID(0);
        return true;
    }

    Radioss::FITKRadiossCase* radiossCase =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (radiossCase == nullptr)
    {
        return false;
    }

    Radioss::FITKRadiossMeshModel* radMeshModel = radiossCase->getMeshModel();
    if (radMeshModel == nullptr)
    {
        return false;
    }

    Interface::FITKComponentManager* componentManager = radMeshModel->getComponentManager();
    if (componentManager == nullptr)
    {
        return false;
    }

    Interface::FITKAbstractModelComponent* component = componentManager->getDataByName(name);
    if (component == nullptr)
    {
        return false;
    }

    _gravity->setNodeGroupID(component->getDataObjectID());
    updateRender();
    return true;
}

bool Gravity::setAScaleX(double scale)
{
    if (_gravity == nullptr)
    {
        return false;
    }

    _gravity->setAScaleX(scale);
    return true;
}

bool Gravity::setFScaleY(double scale)
{
    if (_gravity == nullptr)
    {
        return false;
    }

    _gravity->setFScaleY(scale);
    updateRender();
    return true;
}

Radioss::FITKGravity* Gravity::getGravity()
{
    return _gravity;
}

void Gravity::updateRender()
{
    QStringList listOpertor = { "TreeSolutionEvent" };
    for (int i = 0; i < listOpertor.size(); ++i)
    {
        EventOper::TreeEventOperator* oper =
            dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
        if (oper != nullptr)
        {
            oper->updateTree();
        }
    }
    //更新渲染
    EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
    if (operGraph)
    {
        operGraph->updateGraph(_gravity->getDataObjectID(),true);
    }
    return ;
}

Gravity* GravityWapper::new_Gravity()
{
    return new Gravity();
}

void GravityWapper::delete_Gravity(Gravity* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

Gravity* GravityWapper::static_Gravity_GetGravity(const QString& name)
{
    return new Gravity(name);
}

QString GravityWapper::getName(Gravity* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString GravityWapper::setName(Gravity* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, Gravity with name %1 may already exist or name is invalid").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString GravityWapper::createGravity(Gravity* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createGravity(name))
    {
        return sendMessageToServer(tr("Creation failed! Gravity with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Gravity created with name %1").arg(obj->_gravity->getDataObjectName()));
}

QString GravityWapper::setTimeFunction(Gravity* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setTimeFunction(name))
    {
        return sendMessageToServer(tr("Failed !Time function with name %1 not found or gravity not created!").arg(name));
    }

    return sendMessageToServer(tr("Success, Gravity Time Function set to %1").arg(name));
}

QString GravityWapper::setGravityDirection(Gravity* obj, const QString& direction)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setGravityDirection(direction))
    {
        return sendMessageToServer(tr("Failed !Gravity direction invalid, use x/y/z"));
    }

    return sendMessageToServer(tr("Success, Gravity Direction set to %1").arg(direction));
}

// void GravityWapper::setSystemID(Gravity* obj, int id)
// {
//     if (obj == nullptr || !obj->setSystemID(id))
//     {
//         AppFrame::FITKMessageError("Set Gravity System ID failed!");
//     }
// }

// void GravityWapper::setSensorID(Gravity* obj, int id)
// {
//     if (obj == nullptr || !obj->setSensorID(id))
//     {
//         AppFrame::FITKMessageError("Set Gravity Sensor ID failed!");
//     }
// }

QString GravityWapper::setNodeGroup(Gravity* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setNodeGroup(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found or gravity not created!").arg(name));
    }

    return sendMessageToServer(tr("Success, Gravity Node Group set to %1").arg(name));
}

QString GravityWapper::setAScaleX(Gravity* obj, double scale)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setAScaleX(scale))
    {
        return sendMessageToServer(tr("Failed !No gravity found!"));
    }

    return sendMessageToServer(tr("Success, Gravity AScaleX set to %1").arg(scale));
}

QString GravityWapper::setFScaleY(Gravity* obj, double scale)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setFScaleY(scale))
    {
        return sendMessageToServer(tr("Failed !No gravity found!"));
    }

    return sendMessageToServer(tr("Success, Gravity FScaleY set to %1").arg(scale));
}