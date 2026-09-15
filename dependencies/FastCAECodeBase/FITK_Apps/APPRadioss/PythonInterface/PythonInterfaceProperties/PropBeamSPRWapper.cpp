/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PropBeamSPRWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeamSPR.h"
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

    Radioss::FITKPropBeamSPR* getBeamSPRProperty(BeamSPR* obj)
    {
        if (obj == nullptr)
        {
            return nullptr;
        }

        return dynamic_cast<Radioss::FITKPropBeamSPR*>(obj->getProperty());
    }

    Radioss::FITKPropValueBeamSPR* getBeamSPRValue(BeamSPR* obj)
    {
        Radioss::FITKPropBeamSPR* property = getBeamSPRProperty(obj);
        if (property == nullptr)
        {
            return nullptr;
        }

        return property->getValue();
    }

    Radioss::ValueDOF* getBeamSPRValueDOF(BeamSPR* obj, int dof)
    {
        Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
        if (propValue == nullptr)
        {
            return nullptr;
        }

        if (dof < static_cast<int>(Radioss::DOF_SPR::Tension_Compression) 
        || dof > static_cast<int>(Radioss::DOF_SPR::Bending_Z))
        {
            return nullptr;
        }
        Radioss::DOF_SPR beamSPRDof = static_cast<Radioss::DOF_SPR>(dof);

        return &propValue->getValueDOF(beamSPRDof);
    }

    Radioss::FITKNonLinerCurve* getBeamSPRCurve(const QString& curveName)
    {
        Radioss::FITKRadiossCase* caseData =
            FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr)
        {
            return nullptr;
        }

        Radioss::FITKNonLinerCurveManager* curveManager = caseData->getNonLinerCurveManager();
        if (curveManager == nullptr)
        {
            return nullptr;
        }

        return curveManager->getDataByName(curveName);
    }
}

BeamSPR* PropBeamSPRWapper::new_BeamSPR()
{
    return new BeamSPR();
}

void PropBeamSPRWapper::delete_BeamSPR(BeamSPR* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

BeamSPR* PropBeamSPRWapper::static_BeamSPR_GetBeamSPR(const QString& name)
{
    return new BeamSPR(name);
}

QString PropBeamSPRWapper::getName(BeamSPR* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString PropBeamSPRWapper::setName(BeamSPR* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    bool ok = obj->setName(name);
    if (!ok)
    {
        return sendMessageToServer(tr("Failed, Property with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString PropBeamSPRWapper::createProperty(BeamSPR* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    bool ok = obj->createProperty(name, static_cast<int>(Radioss::FITKPropAbstract::BeamSPR));
    if (!ok)
    {
        return sendMessageToServer(tr("Creation failed! Property with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, BeamSPR property created with name %1").arg(name));
}

QString PropBeamSPRWapper::setMass(BeamSPR* obj, double value)
{
    Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setMass(value);
    return sendMessageToServer(tr("Set BeamSPR Mass successfully"));
}

QString PropBeamSPRWapper::setInertia(BeamSPR* obj, double value)
{
    Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setInertia(value);
    return sendMessageToServer(tr("Set BeamSPR Inertia successfully"));
}

// void PropBeamSPRWapper::setSkewID(BeamSPR* obj, int value)
// {
//     Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
//     if (propValue == nullptr)
//     {
//         AppFrame::FITKMessageError("Set BeamSPR SkewID failed !");
//         return;
//     }

//     propValue->setSkewID(value);
// }

// QString PropBeamSPRWapper::setSensID(BeamSPR* obj, int value)
// {
//     Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
//     if (propValue == nullptr)
//     {
//         return tr("Failed !No property value found!");
//     }

//     propValue->setSensID(value);
//     return tr("Set BeamSPR SensID successfully");
// }

QString PropBeamSPRWapper::setIsflag(BeamSPR* obj, int value)
{
    Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIsflag(value);
    return sendMessageToServer(tr("Set BeamSPR Isflag successfully"));
}

QString PropBeamSPRWapper::setIfail(BeamSPR* obj, int value)
{
    Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIfail(value);
    return sendMessageToServer(tr("Set BeamSPR Ifail successfully"));
}

QString PropBeamSPRWapper::setIleng(BeamSPR* obj, int value)
{
    Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIleng(value);
    return sendMessageToServer(tr("Set BeamSPR Ileng successfully"));
}

QString PropBeamSPRWapper::setIfail2(BeamSPR* obj, int value)
{
    Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setIfail2(value);
    return sendMessageToServer(tr("Set BeamSPR Ifail2 successfully"));
}

QString PropBeamSPRWapper::setV0(BeamSPR* obj, double value)
{
    Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setV0(value);
    return sendMessageToServer(tr("Set BeamSPR V0 successfully"));
}

QString PropBeamSPRWapper::setW0(BeamSPR* obj, double value)
{
    Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setW0(value);
    return sendMessageToServer(tr("Set BeamSPR W0 successfully"));
}

QString PropBeamSPRWapper::setFcut(BeamSPR* obj, double value)
{
    Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setFcut(value);
    return sendMessageToServer(tr("Set BeamSPR Fcut successfully"));
}

QString PropBeamSPRWapper::setFsmooth(BeamSPR* obj, int value)
{
    Radioss::FITKPropValueBeamSPR* propValue = getBeamSPRValue(obj);
    if (propValue == nullptr)
    {
        return sendMessageToServer(tr("Failed !No property value found!"));
    }

    propValue->setFsmooth(value);
    return sendMessageToServer(tr("Set BeamSPR Fsmooth successfully"));
}

QString PropBeamSPRWapper::setK(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->K = value;
    return sendMessageToServer(tr("Set BeamSPR K successfully"));
}

QString PropBeamSPRWapper::setC(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->C = value;
    return sendMessageToServer(tr("Set BeamSPR C successfully"));
}

QString PropBeamSPRWapper::setA(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->A = value;
    return sendMessageToServer(tr("Set BeamSPR A successfully"));
}

QString PropBeamSPRWapper::setB(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->B = value;
    return sendMessageToServer(tr("Set BeamSPR B successfully"));
}

QString PropBeamSPRWapper::setD(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->D = value;
    return sendMessageToServer(tr("Set BeamSPR D successfully"));
}

QString PropBeamSPRWapper::setFunction1(BeamSPR* obj, int dof, const QString& curveName)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    Radioss::FITKNonLinerCurve* curve = getBeamSPRCurve(curveName);
    if (curve == nullptr)
    {
        return sendMessageToServer(tr("Failed !Curve with name %1 not found!").arg(curveName));
    }

    valueDOF->fct_ID1 = curve->getDataObjectID();
    return sendMessageToServer(tr("Set BeamSPR Function1 successfully"));
}

QString PropBeamSPRWapper::setH(BeamSPR* obj, int dof, int value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->H = value;
    return sendMessageToServer(tr("Set BeamSPR H successfully"));
}

QString PropBeamSPRWapper::setFunction2(BeamSPR* obj, int dof, const QString& curveName)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    Radioss::FITKNonLinerCurve* curve = getBeamSPRCurve(curveName);
    if (curve == nullptr)
    {
        return sendMessageToServer(tr("Failed !Curve with name %1 not found!").arg(curveName));
    }

    valueDOF->fct_ID2 = curve->getDataObjectID();
    return sendMessageToServer(tr("Set BeamSPR Function2 successfully"));
}

QString PropBeamSPRWapper::setFunction3(BeamSPR* obj, int dof, const QString& curveName)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    Radioss::FITKNonLinerCurve* curve = getBeamSPRCurve(curveName);
    if (curve == nullptr)
    {
        return sendMessageToServer(tr("Failed !Curve with name %1 not found!").arg(curveName));
    }

    valueDOF->fct_ID3 = curve->getDataObjectID();
    return sendMessageToServer(tr("Set BeamSPR Function3 successfully"));
}

QString PropBeamSPRWapper::setFunction4(BeamSPR* obj, int dof, const QString& curveName)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    Radioss::FITKNonLinerCurve* curve = getBeamSPRCurve(curveName);
    if (curve == nullptr)
    {
        return sendMessageToServer(tr("Failed !Curve with name %1 not found!").arg(curveName));
    }

    valueDOF->fct_ID4 = curve->getDataObjectID();
    return sendMessageToServer(tr("Set BeamSPR Function4 successfully"));
}

QString PropBeamSPRWapper::setMin(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->min = value;
    return sendMessageToServer(tr("Set BeamSPR Min successfully"));
}

QString PropBeamSPRWapper::setMax(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->max = value;
    return sendMessageToServer(tr("Set BeamSPR Max successfully"));
}

QString PropBeamSPRWapper::setF(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->F = value;
    return sendMessageToServer(tr("Set BeamSPR F successfully"));
}

QString PropBeamSPRWapper::setE(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->E = value;
    return sendMessageToServer(tr("Set BeamSPR E successfully"));
}

QString PropBeamSPRWapper::setAscale(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->Ascale = value;
    return sendMessageToServer(tr("Set BeamSPR Ascale successfully"));
}

QString PropBeamSPRWapper::setHscale(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->Hscale = value;
    return sendMessageToServer(tr("Set BeamSPR Hscale successfully"));
}

QString PropBeamSPRWapper::setVc(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->vc = value;
    return sendMessageToServer(tr("Set BeamSPR Vc successfully"));
}

QString PropBeamSPRWapper::setN(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->n = value;
    return sendMessageToServer(tr("Set BeamSPR N successfully"));
}

QString PropBeamSPRWapper::setAlpha(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->a = value;
    return sendMessageToServer(tr("Set BeamSPR Alpha successfully"));
}

QString PropBeamSPRWapper::setExponent(BeamSPR* obj, int dof, double value)
{
    Radioss::ValueDOF* valueDOF = getBeamSPRValueDOF(obj, dof);
    if (valueDOF == nullptr)
    {
        return sendMessageToServer(tr("Failed !Property value or DOF invalid!"));
    }

    valueDOF->e = value;
    return sendMessageToServer(tr("Set BeamSPR Exponent successfully"));
}

BeamSPR::BeamSPR(const QString& name)
    : PyInterface::PyPropertyBase(name)
{
}

BeamSPR::BeamSPR()
    : PyInterface::PyPropertyBase()
{
}