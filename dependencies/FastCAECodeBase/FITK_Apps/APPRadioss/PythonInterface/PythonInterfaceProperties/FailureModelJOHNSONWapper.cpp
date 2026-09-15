/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FailureModelJOHNSONWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"
#include "FITK_Component/FITKRadiossData/FITKFailureModelJOHNSON.h"

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

JOHNSON::JOHNSON(const QString& name) :
    PyInterface::PyFailureModelBase(name)
{
}

JOHNSON::JOHNSON() :
    PyInterface::PyFailureModelBase()
{
}

Radioss::FITKFailureModelJOHNSON* JOHNSON::getFailureModelJOHNSON()
{
    return dynamic_cast<Radioss::FITKFailureModelJOHNSON*>(
        PyInterface::PyFailureModelBase::getFailureModel());
}

JOHNSON* FailureModelJOHNSONWapper::new_JOHNSON()
{
    return new JOHNSON();
}

void FailureModelJOHNSONWapper::delete_JOHNSON(JOHNSON* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

JOHNSON* FailureModelJOHNSONWapper::static_JOHNSON_GetJOHNSON(const QString& name)
{
    return new JOHNSON(name);
}

QString FailureModelJOHNSONWapper::getName(JOHNSON* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString FailureModelJOHNSONWapper::setName(JOHNSON* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, Failure model JOHNSON with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString FailureModelJOHNSONWapper::createFailureModel(JOHNSON* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createFailureModel(name, static_cast<int>(Radioss::FITKAbstractFailureModel::JOHNSON)))
    {
        return sendMessageToServer(tr("Creation failed! Failure model JOHNSON with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Failure model JOHNSON created with name %1").arg(obj->getName()));
}

QString FailureModelJOHNSONWapper::addMaterial(JOHNSON* obj, const QString& materialName)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->addMaterial(materialName))
    {
        return sendMessageToServer(tr("Failed !Material with name %1 not found or failure model is invalid!").arg(materialName));
    }

    return sendMessageToServer(tr("Success, Add material %1 to failure model JOHNSON").arg(materialName));
}

QString FailureModelJOHNSONWapper::removeMaterial(JOHNSON* obj, const QString& materialName)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->removeMaterial(materialName))
    {
        return sendMessageToServer(tr("Failed !Material with name %1 not found or failure model is invalid!").arg(materialName));
    }

    return sendMessageToServer(tr("Success, Remove material %1 from failure model JOHNSON").arg(materialName));
}

QString FailureModelJOHNSONWapper::setD1(JOHNSON* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelJOHNSON* failureModel = obj->getFailureModelJOHNSON();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model JOHNSON found!"));
    }

    failureModel->setD1(value);
    return sendMessageToServer(tr("Success, Set failure model JOHNSON D1 to %1").arg(value));
}

QString FailureModelJOHNSONWapper::setD2(JOHNSON* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelJOHNSON* failureModel = obj->getFailureModelJOHNSON();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model JOHNSON found!"));
    }

    failureModel->setD2(value);
    return sendMessageToServer(tr("Success, Set failure model JOHNSON D2 to %1").arg(value));
}

QString FailureModelJOHNSONWapper::setD3(JOHNSON* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelJOHNSON* failureModel = obj->getFailureModelJOHNSON();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model JOHNSON found!"));
    }

    failureModel->setD3(value);
    return sendMessageToServer(tr("Success, Set failure model JOHNSON D3 to %1").arg(value));
}

QString FailureModelJOHNSONWapper::setD4(JOHNSON* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelJOHNSON* failureModel = obj->getFailureModelJOHNSON();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model JOHNSON found!"));
    }

    failureModel->setD4(value);
    return sendMessageToServer(tr("Success, Set failure model JOHNSON D4 to %1").arg(value));
}

QString FailureModelJOHNSONWapper::setD5(JOHNSON* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelJOHNSON* failureModel = obj->getFailureModelJOHNSON();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model JOHNSON found!"));
    }

    failureModel->setD5(value);
    return sendMessageToServer(tr("Success, Set failure model JOHNSON D5 to %1").arg(value));
}

QString FailureModelJOHNSONWapper::setEps0(JOHNSON* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelJOHNSON* failureModel = obj->getFailureModelJOHNSON();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model JOHNSON found!"));
    }

    failureModel->setEps0(value);
    return sendMessageToServer(tr("Success, Set failure model JOHNSON Eps0 to %1").arg(value));
}

QString FailureModelJOHNSONWapper::setIfailsh(JOHNSON* obj, int flag)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelJOHNSON* failureModel = obj->getFailureModelJOHNSON();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model JOHNSON found!"));
    }

    failureModel->setIfailsh(flag);
    return sendMessageToServer(tr("Success, Set failure model JOHNSON Ifailsh to %1").arg(flag));
}

QString FailureModelJOHNSONWapper::setIfailso(JOHNSON* obj, int flag)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelJOHNSON* failureModel = obj->getFailureModelJOHNSON();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model JOHNSON found!"));
    }

    failureModel->setIfailso(flag);
    return sendMessageToServer(tr("Success, Set failure model JOHNSON Ifailso to %1").arg(flag));
}

QString FailureModelJOHNSONWapper::setDadv(JOHNSON* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelJOHNSON* failureModel = obj->getFailureModelJOHNSON();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model JOHNSON found!"));
    }

    failureModel->setDadv(value);
    return sendMessageToServer(tr("Success, Set failure model JOHNSON Dadv to %1").arg(value));
}

QString FailureModelJOHNSONWapper::setIxfem(JOHNSON* obj, int flag)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelJOHNSON* failureModel = obj->getFailureModelJOHNSON();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model JOHNSON found!"));
    }

    failureModel->setIxfem(flag);
    return sendMessageToServer(tr("Success, Set failure model JOHNSON Ixfem to %1").arg(flag));
}