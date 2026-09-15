/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "CurveWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/TreeEventOperator.h"

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

Curve::Curve(const QString& name)
{
    Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (caseData == nullptr)
    {
        return;
    }

    _curveManager = caseData->getNonLinerCurveManager();
    if (_curveManager == nullptr)
    {
        return;
    }

    _curve = _curveManager->getDataByName(name);
}

Curve::Curve()
{
    Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (caseData == nullptr)
    {
        return;
    }

    _curveManager = caseData->getNonLinerCurveManager();
}

QString Curve::getName()
{
    if (_curve == nullptr)
    {
        return QString();
    }

    return _curve->getDataObjectName();
}

bool Curve::setName(const QString& name)
{
    if (name.isEmpty() || _curve == nullptr || _curveManager == nullptr)
    {
        return false;
    }

    Radioss::FITKNonLinerCurve* sameNameCurve = _curveManager->getDataByName(name);
    if (sameNameCurve != nullptr && sameNameCurve != _curve)
    {
        return false;
    }

    _curve->setDataObjectName(name);
    updateRender();
    return true;
}

bool Curve::createCurve(const QString& name)
{
    if (_curveManager == nullptr)
    {
        return false;
    }

    QString curveName = name;
    if (curveName.isEmpty())
    {
        curveName = _curveManager->checkName("Curve-1");
    }

    if (_curveManager->getDataByName(curveName) != nullptr)
    {
        return false;
    }

    _curve = new Radioss::FITKNonLinerCurve();
    _curve->setDataObjectName(curveName);
    _curveManager->appendDataObj(_curve);
    updateRender();
    return true;
}

bool Curve::addPoint(double x, double y)
{
    if (_curve == nullptr)
    {
        return false;
    }

    Radioss::FITKNonLinerCurve::FITKNonLinerCurvePoint point;
    point.x = x;
    point.y = y;
    _curve->addPoint(point);
    return true;
}

bool Curve::removePoint(int index)
{
    if (_curve == nullptr)
    {
        return false;
    }

    if (!_curve->removePoint(index))
    {
        return false;
    }

    return true;
}

bool Curve::updatePoint(int index, double x, double y)
{
    if (_curve == nullptr)
    {
        return false;
    }

    Radioss::FITKNonLinerCurve::FITKNonLinerCurvePoint point;
    point.x = x;
    point.y = y;
    if (!_curve->updatePoint(index, point))
    {
        return false;
    }

    return true;
}

bool Curve::removeAllPoints()
{
    if (_curve == nullptr)
    {
        return false;
    }

    _curve->removeAllPoints();
    return true;
}

int Curve::getPointCount()
{
    if (_curve == nullptr)
    {
        return 0;
    }

    return _curve->getPointCount();
}

Radioss::FITKNonLinerCurve* Curve::getCurve()
{
    return _curve;
}

void Curve::updateRender()
{
    QStringList listOpertor = {  "TreePropertyEvent" };
    for (int i = 0; i < listOpertor.size(); ++i)
    {
        EventOper::TreeEventOperator* oper =
            dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
        if (oper != nullptr)
        {
            oper->updateTree();
        }
    }
}

Curve* CurveWapper::new_Curve()
{
    return new Curve();
}

void CurveWapper::delete_Curve(Curve* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

Curve* CurveWapper::static_Curve_GetCurve(const QString& name)
{
    return new Curve(name);
}

QString CurveWapper::getName(Curve* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString CurveWapper::setName(Curve* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, Curve with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString CurveWapper::createCurve(Curve* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createCurve(name))
    {
        return sendMessageToServer(tr("Creation failed! Curve with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Curve created with name %1").arg(obj->getName()));
}

QString CurveWapper::addPoint(Curve* obj, double x, double y)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->addPoint(x, y))
    {
        return sendMessageToServer(tr("Failed, No curve found!"));
    }

    return sendMessageToServer(tr("Success, Add curve point (%1, %2)").arg(x).arg(y));
}

QString CurveWapper::removePoint(Curve* obj, int index)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->removePoint(index))
    {
        return sendMessageToServer(tr("Failed, Index %1 is invalid or curve not found!").arg(index));
    }

    return sendMessageToServer(tr("Success, Remove curve point at index %1").arg(index));
}

QString CurveWapper::updatePoint(Curve* obj, int index, double x, double y)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->updatePoint(index, x, y))
    {
        return sendMessageToServer(tr("Failed, Index %1 is invalid or curve not found!").arg(index));
    }

    return sendMessageToServer(tr("Success, Update curve point %1 to (%2, %3)").arg(index).arg(x).arg(y));
}

QString CurveWapper::removeAllPoints(Curve* obj)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->removeAllPoints())
    {
        return sendMessageToServer(tr("Failed, No curve found!"));
    }

    return sendMessageToServer(tr("Success, Remove all curve points"));
}

int CurveWapper::getPointCount(Curve* obj)
{
    if (obj == nullptr)
    {
        return 0;
    }

    return obj->getPointCount();
}