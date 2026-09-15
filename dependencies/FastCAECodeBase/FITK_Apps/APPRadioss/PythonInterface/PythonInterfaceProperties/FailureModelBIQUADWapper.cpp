/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FailureModelBIQUADWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"
#include "FITK_Component/FITKRadiossData/FITKFailureModelBIQUAD.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
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

    Radioss::FITKNonLinerCurve* getCurveByName(const QString& curveName)
    {
        if (curveName.isEmpty())
        {
            return nullptr;
        }

        Radioss::FITKRadiossCase* radiossCase =
            FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr)
        {
            return nullptr;
        }

        Radioss::FITKNonLinerCurveManager* curveManager = radiossCase->getNonLinerCurveManager();
        if (curveManager == nullptr)
        {
            return nullptr;
        }

        return curveManager->getDataByName(curveName);
    }
}

BIQUAD::BIQUAD(const QString& name) :
    PyInterface::PyFailureModelBase(name)
{
}

BIQUAD::BIQUAD() :
    PyInterface::PyFailureModelBase()
{
}

Radioss::FITKFailureModelBIQUAD* BIQUAD::getFailureModelBIQUAD()
{
    return dynamic_cast<Radioss::FITKFailureModelBIQUAD*>(
        PyInterface::PyFailureModelBase::getFailureModel());
}

BIQUAD* FailureModelBIQUADWapper::new_BIQUAD()
{
    return new BIQUAD();
}

void FailureModelBIQUADWapper::delete_BIQUAD(BIQUAD* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

BIQUAD* FailureModelBIQUADWapper::static_BIQUAD_GetBIQUAD(const QString& name)
{
    return new BIQUAD(name);
}

QString FailureModelBIQUADWapper::getName(BIQUAD* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString FailureModelBIQUADWapper::setName(BIQUAD* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, Failure model BIQUAD with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString FailureModelBIQUADWapper::createFailureModel(BIQUAD* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createFailureModel(name, static_cast<int>(Radioss::FITKAbstractFailureModel::BIQUAD)))
    {
        return sendMessageToServer(tr("Creation failed! Failure model BIQUAD with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Failure model BIQUAD created with name %1").arg(obj->getName()));
}

QString FailureModelBIQUADWapper::addMaterial(BIQUAD* obj, const QString& materialName)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->addMaterial(materialName))
    {
        return sendMessageToServer(tr("Failed !Material with name %1 not found or failure model is invalid!").arg(materialName));
    }

    return sendMessageToServer(tr("Success, Add material %1 to failure model BIQUAD").arg(materialName));
}

QString FailureModelBIQUADWapper::removeMaterial(BIQUAD* obj, const QString& materialName)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->removeMaterial(materialName))
    {
        return sendMessageToServer(tr("Failed !Material with name %1 not found or failure model is invalid!").arg(materialName));
    }

    return sendMessageToServer(tr("Success, Remove material %1 from failure model BIQUAD").arg(materialName));
}

QString FailureModelBIQUADWapper::setC1(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setC1(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD C1 to %1").arg(value));
}

QString FailureModelBIQUADWapper::setC2(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setC2(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD C2 to %1").arg(value));
}

QString FailureModelBIQUADWapper::setC3(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setC3(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD C3 to %1").arg(value));
}

QString FailureModelBIQUADWapper::setC4(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setC4(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD C4 to %1").arg(value));
}

QString FailureModelBIQUADWapper::setC5(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setC5(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD C5 to %1").arg(value));
}

QString FailureModelBIQUADWapper::setPthickfail(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setPthickfail(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD Pthickfail to %1").arg(value));
}

QString FailureModelBIQUADWapper::setMFlag(BIQUAD* obj, int flag)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setMFlag(flag);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD MFlag to %1").arg(flag));
}

QString FailureModelBIQUADWapper::setSFlag(BIQUAD* obj, int flag)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setSFlag(flag);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD SFlag to %1").arg(flag));
}

QString FailureModelBIQUADWapper::setInststart(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setInststart(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD Inststart to %1").arg(value));
}

QString FailureModelBIQUADWapper::setFctIDel(BIQUAD* obj, const QString& curveName)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    if (curveName.isEmpty())
    {
        failureModel->setFctIDel(0);
        return sendMessageToServer(tr("Success, Clear failure model BIQUAD FctIDel"));
    }

    Radioss::FITKNonLinerCurve* curve = getCurveByName(curveName);
    if (curve == nullptr)
    {
        return sendMessageToServer(tr("Failed !Curve with name %1 not found!").arg(curveName));
    }

    failureModel->setFctIDel(curve->getDataObjectID());
    return sendMessageToServer(tr("Success, Set failure model BIQUAD FctIDel to %1").arg(curveName));
}

QString FailureModelBIQUADWapper::setElref(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setElref(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD Elref to %1").arg(value));
}

QString FailureModelBIQUADWapper::setR1(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setR1(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD R1 to %1").arg(value));
}

QString FailureModelBIQUADWapper::setR2(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setR2(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD R2 to %1").arg(value));
}

QString FailureModelBIQUADWapper::setR4(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setR4(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD R4 to %1").arg(value));
}

QString FailureModelBIQUADWapper::setR5(BIQUAD* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKFailureModelBIQUAD* failureModel = obj->getFailureModelBIQUAD();
    if (failureModel == nullptr)
    {
        return sendMessageToServer(tr("Failed !No failure model BIQUAD found!"));
    }

    failureModel->setR5(value);
    return sendMessageToServer(tr("Success, Set failure model BIQUAD R5 to %1").arg(value));
}