/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ProbeAccelerometerWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKProbeAcceleration.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

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

ProbeAccelerometer::ProbeAccelerometer(const QString& name) :
    PyInterface::PyProbeBase(name)
{
}

ProbeAccelerometer::ProbeAccelerometer() :
    PyInterface::PyProbeBase()
{
}

Radioss::FITKProbeAcceleration* ProbeAccelerometer::getProbeAccelerometer()
{
    return dynamic_cast<Radioss::FITKProbeAcceleration*>(PyInterface::PyProbeBase::getProbe());
}

bool ProbeAccelerometer::setSingleNodeGroup(const QString& name)
{
    Radioss::FITKProbeAcceleration* probe = getProbeAccelerometer();
    if (probe == nullptr || name.isEmpty())
    {
        return false;
    }

    Radioss::FITKRadiossCase* radiossCase =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (radiossCase == nullptr)
    {
        return false;
    }

    Radioss::FITKRadiossMeshModel* meshModel = radiossCase->getMeshModel();
    if (meshModel == nullptr)
    {
        return false;
    }

    Interface::FITKComponentManager* componentManager = meshModel->getComponentManager();
    if (componentManager == nullptr)
    {
        return false;
    }

    Interface::FITKModelSet* modelSet =
        dynamic_cast<Interface::FITKModelSet*>(componentManager->getDataByName(name));
    if (modelSet == nullptr)
    {
        return false;
    }

    probe->setSetID(modelSet->getDataObjectID());
    updateRender();
    return true;
}

ProbeAccelerometer* ProbeAccelerometerWapper::new_ProbeAccelerometer()
{
    return new ProbeAccelerometer();
}

void ProbeAccelerometerWapper::delete_ProbeAccelerometer(ProbeAccelerometer* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

ProbeAccelerometer* ProbeAccelerometerWapper::static_ProbeAccelerometer_GetProbeAccelerometer(const QString& name)
{
    return new ProbeAccelerometer(name);
}

QString ProbeAccelerometerWapper::getName(ProbeAccelerometer* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString ProbeAccelerometerWapper::setName(ProbeAccelerometer* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, Probe accelerometer with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString ProbeAccelerometerWapper::createProbe(ProbeAccelerometer* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createProbe(name, static_cast<int>(Radioss::FITKAbstractProbe::Probe_Acceleration)))
    {
        return sendMessageToServer(tr("Creation failed! Probe accelerometer with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Probe accelerometer created with name %1").arg(obj->getName()));
}

QString ProbeAccelerometerWapper::setSystemID(ProbeAccelerometer* obj, int systemID)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeAcceleration* probe = obj->getProbeAccelerometer();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe accelerometer found!"));
    }

    probe->setSystemID(systemID);
    obj->updateRender();
    return sendMessageToServer(tr("Success, Set probe accelerometer system ID to %1").arg(systemID));
}

QString ProbeAccelerometerWapper::setSingleNodeGroup(ProbeAccelerometer* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeAcceleration* probe = obj->getProbeAccelerometer();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe accelerometer found!"));
    }

    if (name.isEmpty() || name == "None")
    {
        probe->setSetID(-1);
        obj->updateRender();
        return sendMessageToServer(tr("Clear probe accelerometer node group successfully"));
    }

    if (!obj->setSingleNodeGroup(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found!").arg(name));
    }

    return sendMessageToServer(tr("Set probe accelerometer node group successfully"));
}

int ProbeAccelerometerWapper::getNodeID(ProbeAccelerometer* obj)
{
    if (obj == nullptr)
    {
        return -1;
    }

    Radioss::FITKProbeAcceleration* probe = obj->getProbeAccelerometer();
    if (probe == nullptr)
    {
        return -1;
    }

    return probe->getNodeID();
}

QString ProbeAccelerometerWapper::setCutOffFreq(ProbeAccelerometer* obj, double freq)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeAcceleration* probe = obj->getProbeAccelerometer();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe accelerometer found!"));
    }

    probe->setCutOffFreq(freq);
    obj->updateRender();
    return sendMessageToServer(tr("Success, Set probe accelerometer cut off frequency to %1").arg(freq));
}
