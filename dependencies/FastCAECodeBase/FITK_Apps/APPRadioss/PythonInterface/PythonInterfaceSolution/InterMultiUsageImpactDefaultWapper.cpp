/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "InterMultiUsageImpactDefaultWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "SolutionWapper.h"

#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionMultiUsageImpact.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"

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

Radioss::FITKInteractionMultiUsageImpactDefaultValue* InterMultiUsageImpactDefault::getInterMultiUsageImpactDefault() const
{
    Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
    if (solution == nullptr)
    {
        return nullptr;
    }

    Radioss::FITKRadiossInteractionDefaultManager* manager = solution->getDefaultInteractionManager();
    if (manager == nullptr)
    {
        return nullptr;
    }

    if (manager->getDataCount() == 0)
    {
        Radioss::FITKInteractionMultiUsageImpactDefaultValue* value =
            new Radioss::FITKInteractionMultiUsageImpactDefaultValue();
        manager->appendDataObj(value);
        return value;
    }

    for (int index = 0; index < manager->getDataCount(); ++index)
    {
        Radioss::FITKInteractionDefaultAbstract* value = manager->getDataByIndex(index);
        if (value == nullptr)
        {
            continue;
        }

        if (value->getDefaultInteractionType()
            == Radioss::FITKInteractionDefaultAbstract::DefaultInteractionType::DIT_MultiUsageImpact)
        {
            return dynamic_cast<Radioss::FITKInteractionMultiUsageImpactDefaultValue*>(value);
        }
    }

    Radioss::FITKInteractionMultiUsageImpactDefaultValue* value =
        new Radioss::FITKInteractionMultiUsageImpactDefaultValue();
    manager->appendDataObj(value);
    return value;
}

Radioss::FITKInteractionMultiUsageImpactDefaultValue*
    InterMultiUsageImpactDefaultWapper::getInterMultiUsageImpactDefault(InterMultiUsageImpactDefault* obj)
{
    if (obj == nullptr)
    {
        return nullptr;
    }

    return obj->getInterMultiUsageImpactDefault();
}

InterMultiUsageImpactDefault* InterMultiUsageImpactDefaultWapper::new_InterMultiUsageImpactDefault()
{
    return new InterMultiUsageImpactDefault();
}

void InterMultiUsageImpactDefaultWapper::delete_InterMultiUsageImpactDefault(InterMultiUsageImpactDefault* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

QString InterMultiUsageImpactDefaultWapper::setIstf(InterMultiUsageImpactDefault* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Istf = value;
    return sendMessageToServer(tr("Success, Istf set to %1").arg(value));
}

QString InterMultiUsageImpactDefaultWapper::setIgap(InterMultiUsageImpactDefault* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Igap = value;
    return sendMessageToServer(tr("Success, Igap set to %1").arg(value));
}

QString InterMultiUsageImpactDefaultWapper::setIbag(InterMultiUsageImpactDefault* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Ibag = value;
    return sendMessageToServer(tr("Success, Ibag set to %1").arg(value));
}

QString InterMultiUsageImpactDefaultWapper::setIdel(InterMultiUsageImpactDefault* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Idel = value;
    return sendMessageToServer(tr("Success, Idel set to %1").arg(value));
}

QString InterMultiUsageImpactDefaultWapper::setIremGap(InterMultiUsageImpactDefault* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Irem_gap = value;
    return sendMessageToServer(tr("Success, Irem_gap set to %1").arg(value));
}

QString InterMultiUsageImpactDefaultWapper::setIremI2(InterMultiUsageImpactDefault* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Irem_i2 = value;
    return sendMessageToServer(tr("Success, Irem_i2 set to %1").arg(value));
}

QString InterMultiUsageImpactDefaultWapper::setInacti(InterMultiUsageImpactDefault* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Inacti = value;
    return sendMessageToServer(tr("Success, Inacti set to %1").arg(value));
}

QString InterMultiUsageImpactDefaultWapper::setIform(InterMultiUsageImpactDefault* obj, int value)
{
    Radioss::FITKInteractionMultiUsageImpactDefaultValue* data = getInterMultiUsageImpactDefault(obj);
    if (data == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    data->_Iform = value;
    return sendMessageToServer(tr("Success, Iform set to %1").arg(value));
}