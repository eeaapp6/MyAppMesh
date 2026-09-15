/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "InterMultiUsageImpactWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionMultiUsageImpact.h"
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

Radioss::FITKInteractionMultiUsageImpact* InterMultiUsageImpactWapper::getInterMultiUsageImpact(InterMultiUsageImpact* obj)
{
    if (obj == nullptr)
    {
        return nullptr;
    }

    return dynamic_cast<Radioss::FITKInteractionMultiUsageImpact*>(obj->getInteraction());
}

Radioss::FITKInteractionMultiUsageImpactDefaultValue*
    InterMultiUsageImpactWapper::getInterMultiUsageImpactDefault(InterMultiUsageImpact* obj)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return nullptr;
    }

    return impact->getValues();
}

Radioss::MultiUsageImpactOptionalData*
    InterMultiUsageImpactWapper::getMultiUsageImpactOptionalData(InterMultiUsageImpact* obj)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return nullptr;
    }

    return &impact->getOptionalData();
}

InterMultiUsageImpact* InterMultiUsageImpactWapper::new_InterMultiUsageImpact()
{
    return new InterMultiUsageImpact();
}

void InterMultiUsageImpactWapper::delete_InterMultiUsageImpact(InterMultiUsageImpact* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

InterMultiUsageImpact* InterMultiUsageImpactWapper::static_InterMultiUsageImpact_GetInterMultiUsageImpact(const QString& name)
{
    return new InterMultiUsageImpact(name);
}

QString InterMultiUsageImpactWapper::getName(InterMultiUsageImpact* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString InterMultiUsageImpactWapper::setName(InterMultiUsageImpact* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, InterMultiUsageImpact with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString InterMultiUsageImpactWapper::createInteraction(InterMultiUsageImpact* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createInteraction(name, static_cast<int>(Radioss::FITKAbstractInteraction::IT_MultiUsageImpact)))
    {
        return sendMessageToServer(tr("Creation failed! InterMultiUsageImpact with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, InterMultiUsageImpact created with name %1").arg(obj->getName()));
}

QString InterMultiUsageImpactWapper::setMasterSurface(InterMultiUsageImpact* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setMasterSurface(name))
    {
        return sendMessageToServer(tr("Failed !Master surface with name %1 not found or interaction not created!").arg(name));
    }

    return sendMessageToServer(tr("Success, InterMultiUsageImpact Master Surface set to %1").arg(name));
}

QString InterMultiUsageImpactWapper::setNodeGroup(InterMultiUsageImpact* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setNodeGroup(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found or interaction not created!").arg(name));
    }

    return sendMessageToServer(tr("Success, InterMultiUsageImpact Node Group set to %1").arg(name));
}

QString InterMultiUsageImpactWapper::setIthe(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Ithe = (value != 0);
    return sendMessageToServer(tr("Success, Ithe set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIcurv(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Icurv = value;
    return sendMessageToServer(tr("Success, Icurv set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIadm(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Iadm = value;
    return sendMessageToServer(tr("Success, Iadm set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setFscaleGap(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Fscale_gap = value;
    return sendMessageToServer(tr("Success, Fscale_gap set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setGAPmax(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_GAPmax = value;
    return sendMessageToServer(tr("Success, GAPmax set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setFpenmax(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Fpenmax = value;
    return sendMessageToServer(tr("Success, Fpenmax set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setItied(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Itied = value;
    return sendMessageToServer(tr("Success, Itied set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setSTmin(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_STmin = value;
    return sendMessageToServer(tr("Success, STmin set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setSTmax(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_STmax = value;
    return sendMessageToServer(tr("Success, STmax set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setMeshSize(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_mesh_size = value;
    return sendMessageToServer(tr("Success, mesh_size set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setDtmin(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_dtmin = value;
    return sendMessageToServer(tr("Success, dtmin set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setStfac(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Stfac = value;
    return sendMessageToServer(tr("Success, Stfac set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setFric(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Fric = value;
    return sendMessageToServer(tr("Success, Fric set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setGapmin(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Gapmin = value;
    return sendMessageToServer(tr("Success, Gapmin set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setTstart(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Tstart = value;
    return sendMessageToServer(tr("Success, Tstart set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setTstop(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Tstop = value;
    return sendMessageToServer(tr("Success, Tstop set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIbc(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Ibc = value;
    return sendMessageToServer(tr("Success, Ibc set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setVisS(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_VisS = value;
    return sendMessageToServer(tr("Success, VisS set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setVisF(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_VisF = value;
    return sendMessageToServer(tr("Success, VisF set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setBumult(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Bumult = value;
    return sendMessageToServer(tr("Success, Bumult set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIfric(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Ifric = value;
    return sendMessageToServer(tr("Success, Ifric set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIfiltr(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Ifiltr = value;
    return sendMessageToServer(tr("Success, Ifiltr set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setXfreq(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_Xfreq = value;
    return sendMessageToServer(tr("Success, Xfreq set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setSensID(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_sens_ID = value;
    return sendMessageToServer(tr("Success, sens_ID set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setFctIDf(InterMultiUsageImpact* obj, const QString& curveName)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKNonLinerCurve* curve = getCurveByName(curveName);
    if (curve == nullptr)
    {
        return sendMessageToServer(tr("Failed !Curve with name %1 not found!").arg(curveName));
    }

    impact->_fct_IDf = curve->getDataObjectID();
    return sendMessageToServer(tr("Success, fct_IDf set to %1").arg(curveName));
}

QString InterMultiUsageImpactWapper::setAscaleF(InterMultiUsageImpact* obj, double value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_AscaleF = value;
    return sendMessageToServer(tr("Success, AscaleF set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setFricID(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpact* impact = getInterMultiUsageImpact(obj);
    if (impact == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    impact->_fric_ID = value;
    return sendMessageToServer(tr("Success, fric_ID set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIstf(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Istf = value;
    return sendMessageToServer(tr("Success, Istf set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIgap(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Igap = value;
    return sendMessageToServer(tr("Success, Igap set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIbag(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Ibag = value;
    return sendMessageToServer(tr("Success, Ibag set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIdel(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Idel = value;
    return sendMessageToServer(tr("Success, Idel set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIremGap(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Irem_gap = value;
    return sendMessageToServer(tr("Success, Irem_gap set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIremI2(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Irem_i2 = value;
    return sendMessageToServer(tr("Success, Irem_i2 set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setInacti(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Inacti = value;
    return sendMessageToServer(tr("Success, Inacti set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setIform(InterMultiUsageImpact* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Iform = value;
    return sendMessageToServer(tr("Success, Iform set to %1").arg(value));
}

// QString InterMultiUsageImpactWapper::setNodeID1(InterMultiUsageImpact* obj, int value)
// {
//     Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
//     if (data == nullptr)
//     {
//         return tr("Failed, object is null");
//     }

//     data->node_ID1 = value;
//     return tr("Success, node_ID1 set to %1").arg(value);
// }

// QString InterMultiUsageImpactWapper::setNodeID2(InterMultiUsageImpact* obj, int value)
// {
//     Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
//     if (data == nullptr)
//     {
//         return tr("Failed, object is null");
//     }

//     data->node_ID2 = value;
//     return tr("Success, node_ID2 set to %1").arg(value);
// }

QString InterMultiUsageImpactWapper::setC1(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->C1 = value;
    return sendMessageToServer(tr("Success, C1 set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setC2(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->C2 = value;
    return sendMessageToServer(tr("Success, C2 set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setC3(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->C3 = value;
    return sendMessageToServer(tr("Success, C3 set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setC4(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->C4 = value;
    return sendMessageToServer(tr("Success, C4 set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setC5(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->C5 = value;
    return sendMessageToServer(tr("Success, C5 set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setC6(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->C6 = value;
    return sendMessageToServer(tr("Success, C6 set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setNRadm(InterMultiUsageImpact* obj, int value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->NRadm = value;
    return sendMessageToServer(tr("Success, NRadm set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setPadm(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Padm = value;
    return sendMessageToServer(tr("Success, Padm set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setAngladm(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Angladm = value;
    return sendMessageToServer(tr("Success, Angladm set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setKthe(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Kthe = value;
    return sendMessageToServer(tr("Success, Kthe set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setFctIDK(InterMultiUsageImpact* obj, const QString& curveName)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKNonLinerCurve* curve = getCurveByName(curveName);
    if (curve == nullptr)
    {
        return sendMessageToServer(tr("Failed !Curve with name %1 not found!").arg(curveName));
    }

    data->fct_IDK = curve->getDataObjectID();
    return sendMessageToServer(tr("Success, fct_IDK set to %1").arg(curveName));
}

QString InterMultiUsageImpactWapper::setTint(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Tint = value;
    return sendMessageToServer(tr("Success, Tint set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setItheForm(InterMultiUsageImpact* obj, int value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Ithe_form = value;
    return sendMessageToServer(tr("Success, Ithe_form set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setAscaleK(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->AscaleK = value;
    return sendMessageToServer(tr("Success, AscaleK set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setFrad(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Frad = value;
    return sendMessageToServer(tr("Success, Frad set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setDrad(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Drad = value;
    return sendMessageToServer(tr("Success, Drad set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setFheats(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Fheats = value;
    return sendMessageToServer(tr("Success, Fheats set to %1").arg(value));
}

QString InterMultiUsageImpactWapper::setFheatm(InterMultiUsageImpact* obj, double value)
{
    Radioss::MultiUsageImpactOptionalData* data = getMultiUsageImpactOptionalData(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->Fheatm = value;
    return sendMessageToServer(tr("Success, Fheatm set to %1").arg(value));
}

InterMultiUsageImpact::InterMultiUsageImpact(const QString& name) :
    PyInterface::PyInteractionBase(name)
{
}

InterMultiUsageImpact::InterMultiUsageImpact() :
    PyInterface::PyInteractionBase()
{
}