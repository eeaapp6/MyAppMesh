/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "RigidBodyWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"

#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKConnectionRBODY.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

RigidBody::RigidBody(const QString& name)
{
    Radioss::FITKRadiossCase* radiossCase =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (radiossCase == nullptr)
    {
        return;
    }

    _connectionManager = radiossCase->getConnectionManager();
    if (_connectionManager == nullptr)
    {
        return;
    }

    Radioss::FITKAbstractConnection* connection = _connectionManager->getDataByName(name);
    _rbody = dynamic_cast<Radioss::FITKConnectionRBODY*>(connection);
}

RigidBody::RigidBody()
{
    Radioss::FITKRadiossCase* radiossCase =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (radiossCase == nullptr)
    {
        return;
    }

    _connectionManager = radiossCase->getConnectionManager();
}

QString RigidBody::getName() const
{
    if (_rbody == nullptr)
    {
        return QString();
    }

    return _rbody->getDataObjectName();
}

bool RigidBody::setName(const QString& name)
{
    if (name.isEmpty() || _rbody == nullptr || _connectionManager == nullptr)
    {
        return false;
    }

    Radioss::FITKAbstractConnection* sameNameConnection = _connectionManager->getDataByName(name);
    if (sameNameConnection != nullptr && sameNameConnection != _rbody)
    {
        return false;
    }

    _rbody->setDataObjectName(name);
    updateRender();
    return true;
}

bool RigidBody::createRigidBody(const QString& name)
{
    if (_connectionManager == nullptr)
    {
        return false;
    }

    QString rigidBodyName = name;
    if (rigidBodyName.isEmpty())
    {
        rigidBodyName = _connectionManager->checkName("RigidBody-1");
    }

    if (_connectionManager->getDataByName(rigidBodyName) != nullptr)
    {
        return false;
    }

    _rbody = new Radioss::FITKConnectionRBODY();
    _rbody->setDataObjectName(rigidBodyName);
    _connectionManager->appendDataObj(_rbody);
    updateRender();
    return true;
}

bool RigidBody::setSecondaryNodesGroup(const QString& name)
{
    if (_rbody == nullptr || name.isEmpty())
    {
        return false;
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

    _rbody->setSecondaryNodesGroupID(component->getDataObjectID());
    updateRender();
    return true;
}

bool RigidBody::setEnvelopeSurface(const QString& name)
{
    if (_rbody == nullptr || name.isEmpty())
    {
        return false;
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

    _rbody->setEnvelopeSurface(component->getDataObjectID());
    updateRender();
    return true;
}

Radioss::FITKConnectionRBODY* RigidBody::getRigidBody()
{
    return _rbody;
}

void RigidBody::updateRender()
{
    QStringList operatorList = { "TreeSolutionEvent" };
    for (int index = 0; index < operatorList.size(); ++index)
    {
        EventOper::TreeEventOperator* oper =
            dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(operatorList[index]));
        if (oper != nullptr)
        {
            oper->updateTree();
        }
    }

    if (_rbody == nullptr)
    {
        return;
    }

    EventOper::GraphEventOperator* operGraph =
        FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
    if (operGraph != nullptr)
    {
        operGraph->updateGraph(_rbody->getDataObjectID(), true);
    }
}

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

Radioss::FITKConnectionRBODY* RigidBodyWapper::getRigidBody(RigidBody* obj)
{
    if (obj == nullptr)
    {
        return nullptr;
    }

    return obj->getRigidBody();
}

RigidBody* RigidBodyWapper::new_RigidBody()
{
    return new RigidBody();
}

void RigidBodyWapper::delete_RigidBody(RigidBody* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

RigidBody* RigidBodyWapper::static_RigidBody_GetRigidBody(const QString& name)
{
    return new RigidBody(name);
}

QString RigidBodyWapper::getName(RigidBody* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString RigidBodyWapper::setName(RigidBody* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, RigidBody with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString RigidBodyWapper::createRigidBody(RigidBody* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createRigidBody(name))
    {
        return sendMessageToServer(tr("Creation failed! RigidBody with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, RigidBody created with name %1").arg(obj->getName()));
}

QString RigidBodyWapper::setMainNodeID(RigidBody* obj, int nodeID)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    rbody->setMainNodeID(nodeID);
    obj->updateRender();
    return sendMessageToServer(tr("Success, Main node ID set to %1").arg(nodeID));
}

// QString RigidBodyWapper::setSensorID(RigidBody* obj, int sensorID)
// {
//     Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
//     if (rbody == nullptr)
//     {
//         return tr("Failed, object is null");
//     }

//     rbody->setSensorID(sensorID);
//     obj->updateRender();
//     return tr("Success, Sensor ID set to %1").arg(sensorID);
// }

// QString RigidBodyWapper::setSystemID(RigidBody* obj, int systemID)
// {
//     Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
//     if (rbody == nullptr)
//     {
//         return tr("Failed, object is null");
//     }

//     rbody->setSystemID(systemID);
//     obj->updateRender();
//     return tr("Success, System ID set to %1").arg(systemID);
// }

QString RigidBodyWapper::setInertiaType(RigidBody* obj, int type)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    rbody->setInertiaType(static_cast<Radioss::FITKConnectionRBODY::RbodyIspherType>(type));
    obj->updateRender();
    return sendMessageToServer(tr("Success, Inertia type set to %1").arg(type));
}

QString RigidBodyWapper::setMass(RigidBody* obj, double mass)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    rbody->setMass(mass);
    obj->updateRender();
    return sendMessageToServer(tr("Success, Mass set to %1").arg(mass));
}

QString RigidBodyWapper::setSecondaryNodesGroup(RigidBody* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setSecondaryNodesGroup(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found or rigid body not created!").arg(name));
    }

    return sendMessageToServer(tr("Success, Secondary Nodes Group set to %1").arg(name));
}

QString RigidBodyWapper::setDeactivationRigidWall(RigidBody* obj, bool isDeact)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    rbody->setDeactivationRigidWall(isDeact);
    obj->updateRender();
    return sendMessageToServer(tr("Success, Ikrem set to %1").arg(isDeact ? 1 : 0));
}

QString RigidBodyWapper::setGravityCenterComputation(RigidBody* obj, int type)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    rbody->setGravityCenterComputation(static_cast<Radioss::FITKConnectionRBODY::GravityCenterComputationType>(type));
    obj->updateRender();
    return sendMessageToServer(tr("Success, ICoG set to %1").arg(type));
}

QString RigidBodyWapper::setEnvelopeSurface(RigidBody* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setEnvelopeSurface(name))
    {
        return sendMessageToServer(tr("Failed !Surface with name %1 not found or rigid body not created!").arg(name));
    }

    return sendMessageToServer(tr("Success, Envelope Surface set to %1").arg(name));
}

QString RigidBodyWapper::setInertiaValue(RigidBody* obj, int index, double value)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (index < 0 || index >= 6)
    {
        return sendMessageToServer(tr("Failed, index out of range"));
    }

    double inertiaValue[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    rbody->getInertiaValue(inertiaValue);
    inertiaValue[index] = value;
    rbody->setInertiaValue(inertiaValue);
    obj->updateRender();
    return sendMessageToServer(tr("Success, Inertia value %1 set to %2").arg(index).arg(value));
}

QString RigidBodyWapper::setDomainDecomposition(RigidBody* obj, bool isDom)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    rbody->setDomainDecomposition(isDom);
    obj->updateRender();
    return sendMessageToServer(tr("Success, Ioptoff set to %1").arg(isDom ? 1 : 0));
}

QString RigidBodyWapper::setIfail(RigidBody* obj, bool isFail)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    rbody->setIfail(isFail);
    obj->updateRender();
    return sendMessageToServer(tr("Success, Ifail set to %1").arg(isFail ? 1 : 0));
}

QString RigidBodyWapper::setFN(RigidBody* obj, double value)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    rbody->getOptionData().FN = value;
    return sendMessageToServer(tr("Success, FN set to %1").arg(value));
}

QString RigidBodyWapper::setFT(RigidBody* obj, double value)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    rbody->getOptionData().FT = value;
    return sendMessageToServer(tr("Success, FT set to %1").arg(value));
}

QString RigidBodyWapper::setExpN(RigidBody* obj, int value)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    rbody->getOptionData().expN = value;
    return sendMessageToServer(tr("Success, expN set to %1").arg(value));
}

QString RigidBodyWapper::setExpT(RigidBody* obj, int value)
{
    Radioss::FITKConnectionRBODY* rbody = getRigidBody(obj);
    if (rbody == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    rbody->getOptionData().expT = value;
    return sendMessageToServer(tr("Success, expT set to %1").arg(value));
}