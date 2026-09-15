/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "InterTiedWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"
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

Radioss::FITKInteractionKinematicTied* InterTiedWapper::getInterTied(InterTied* obj)
{
    if (obj == nullptr)
    {
        return nullptr;
    }

    return dynamic_cast<Radioss::FITKInteractionKinematicTied*>(obj->getInteraction());
}

Radioss::FITKInteractionKinematicTiedDefault* InterTiedWapper::getInterTiedDefault(InterTied* obj)
{
    Radioss::FITKInteractionKinematicTied* tied = getInterTied(obj);
    if (tied == nullptr)
    {
        return nullptr;
    }

    return tied->getValue();
}

Radioss::TiedOptionalData* InterTiedWapper::getTiedOptionalData(InterTied* obj)
{
    Radioss::FITKInteractionKinematicTied* tied = getInterTied(obj);
    if (tied == nullptr)
    {
        return nullptr;
    }

    return &tied->getOptionalData();
}

InterTied* InterTiedWapper::new_InterTied()
{
    return new InterTied();
}

void InterTiedWapper::delete_InterTied(InterTied* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

InterTied* InterTiedWapper::static_InterTied_GetInterTied(const QString& name)
{
    return new InterTied(name);
}

QString InterTiedWapper::getName(InterTied* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString InterTiedWapper::setName(InterTied* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, InterTied with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString InterTiedWapper::createInteraction(InterTied* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createInteraction(name, static_cast<int>(Radioss::FITKAbstractInteraction::IT_KinematicTied)))
    {
        return sendMessageToServer(tr("Creation failed! InterTied with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, InterTied created with name %1").arg(obj->getName()));
}

QString InterTiedWapper::setMasterSurface(InterTied* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setMasterSurface(name))
    {
        return sendMessageToServer(tr("Failed !Master surface with name %1 not found or interaction not created!").arg(name));
    }

    return sendMessageToServer(tr("Success, InterTied Master Surface set to %1").arg(name));
}

QString InterTiedWapper::setNodeGroup(InterTied* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setNodeGroup(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found or interaction not created!").arg(name));
    }

    return sendMessageToServer(tr("Success, InterTied Node Group set to %1").arg(name));
}

QString InterTiedWapper::setLevel(InterTied* obj, int level)
{
    Radioss::FITKInteractionKinematicTied* tied = getInterTied(obj);
    if (tied == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    tied->setLevel(level);
    obj->updateRender();
    return sendMessageToServer(tr("Set InterTied Level successfully"));
}

QString InterTiedWapper::setSearchDistance(InterTied* obj, double value)
{
    Radioss::FITKInteractionKinematicTied* tied = getInterTied(obj);
    if (tied == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    tied->setSearchDistance(value);
    obj->updateRender();
    return sendMessageToServer(tr("Set InterTied Search Distance successfully"));
}

QString InterTiedWapper::setIgnoreFlag(InterTied* obj, int flag)
{
    Radioss::FITKInteractionKinematicTiedDefault* value = getInterTiedDefault(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    value->setIgnoreFlag(flag);
    return sendMessageToServer(tr("Success, Ignore flag set to %1").arg(flag));
}

QString InterTiedWapper::setSpotWeldFormulationFlag(InterTied* obj, int flag)
{
    Radioss::FITKInteractionKinematicTiedDefault* value = getInterTiedDefault(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    value->setSpotWeldFormulationFlag(flag);
    return sendMessageToServer(tr("Success, Spotflag set to %1").arg(flag));
}

QString InterTiedWapper::setSearchFormulationFlag(InterTied* obj, int flag)
{
    Radioss::FITKInteractionKinematicTiedDefault* value = getInterTiedDefault(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    value->setSearchFormulationFlag(flag);
    return sendMessageToServer(tr("Success, Isearch set to %1").arg(flag));
}

QString InterTiedWapper::setNodeDeletionFlag(InterTied* obj, int flag)
{
    Radioss::FITKInteractionKinematicTiedDefault* value = getInterTiedDefault(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    value->setNodeDeletionFlag(flag);
    return sendMessageToServer(tr("Success, Idel2 set to %1").arg(flag));
}

//QString InterTiedWapper::setIstfFlagDefault(InterTied* obj, int flag)
//{
//    Radioss::FITKInteractionKinematicTiedDefault* value = getInterTiedDefault(obj);
//    if (value == nullptr)
//    {
//        return tr("Failed, object is null");
//    }
//
//    value->setIstfFlagDefault(flag);
//    return tr("Success, Istf set to %1").arg(flag);
//}

QString InterTiedWapper::setRupt(InterTied* obj, int value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Rupt = value;
    return sendMessageToServer(tr("Success, Rupt set to %1").arg(value));
}

QString InterTiedWapper::setIfilter(InterTied* obj, int value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Ifilter = value;
    return sendMessageToServer(tr("Success, Ifilter set to %1").arg(value));
}

QString InterTiedWapper::setFctIDsr(InterTied* obj, const QString& curveName)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKNonLinerCurve* curve = getCurveByName(curveName);
    if (curve == nullptr)
    {
        return sendMessageToServer(tr("Failed !Curve with name %1 not found!").arg(curveName));
    }

    data->fct_IDsr = curve->getDataObjectID();
    return sendMessageToServer(tr("Success, fct_IDsr set to %1").arg(curveName));
}

QString InterTiedWapper::setFctIDsn(InterTied* obj, const QString& curveName)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKNonLinerCurve* curve = getCurveByName(curveName);
    if (curve == nullptr)
    {
        return sendMessageToServer(tr("Failed !Curve with name %1 not found!").arg(curveName));
    }

    data->fct_IDsn = curve->getDataObjectID();
    return sendMessageToServer(tr("Success, fct_IDsn set to %1").arg(curveName));
}

QString InterTiedWapper::setFctIDst(InterTied* obj, const QString& curveName)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKNonLinerCurve* curve = getCurveByName(curveName);
    if (curve == nullptr)
    {
        return sendMessageToServer(tr("Failed !Curve with name %1 not found!").arg(curveName));
    }

    data->fct_IDst = curve->getDataObjectID();
    return sendMessageToServer(tr("Success, fct_IDst set to %1").arg(curveName));
}

QString InterTiedWapper::setIsym(InterTied* obj, int value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Isym = value;
    return sendMessageToServer(tr("Success, Isym set to %1").arg(value));
}

QString InterTiedWapper::setMaxNDist(InterTied* obj, double value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Max_N_Dist = value;
    return sendMessageToServer(tr("Success, Max_N_Dist set to %1").arg(value));
}

QString InterTiedWapper::setMaxTDist(InterTied* obj, double value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Max_T_Dist = value;
    return sendMessageToServer(tr("Success, Max_T_Dist set to %1").arg(value));
}

QString InterTiedWapper::setFscaleStress(InterTied* obj, double value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Fscale_stress = value;
    return sendMessageToServer(tr("Success, Fscale_stress set to %1").arg(value));
}

QString InterTiedWapper::setFscaleStrRate(InterTied* obj, double value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Fscale_str_rate = value;
    return sendMessageToServer(tr("Success, Fscale_str_rate set to %1").arg(value));
}

QString InterTiedWapper::setFscaleDist(InterTied* obj, double value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Fscale_dist = value;
    return sendMessageToServer(tr("Success, Fscale_dist set to %1").arg(value));
}

QString InterTiedWapper::setAlpha(InterTied* obj, double value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Alpha = value;
    return sendMessageToServer(tr("Success, Alpha set to %1").arg(value));
}

QString InterTiedWapper::setArea(InterTied* obj, double value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Area = value;
    return sendMessageToServer(tr("Success, Area set to %1").arg(value));
}

QString InterTiedWapper::setStfac(InterTied* obj, double value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Stfac = value;
    return sendMessageToServer(tr("Success, Stfac set to %1").arg(value));
}

QString InterTiedWapper::setVisc(InterTied* obj, double value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Visc = value;
    return sendMessageToServer(tr("Success, Visc set to %1").arg(value));
}

QString InterTiedWapper::setIstf(InterTied* obj, int value)
{
    Radioss::TiedOptionalData* data = getTiedOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Istf = value;
    return sendMessageToServer(tr("Success, Istf set to %1").arg(value));
}

InterTied::InterTied(const QString& name) :
    PyInterface::PyInteractionBase(name)
{
}

InterTied::InterTied() :
    PyInterface::PyInteractionBase()
{
}