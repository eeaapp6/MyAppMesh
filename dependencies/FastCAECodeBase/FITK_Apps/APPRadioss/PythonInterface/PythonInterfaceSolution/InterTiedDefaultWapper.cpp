/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "InterTiedDefaultWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "SolutionWapper.h"

#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"
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

Radioss::FITKInteractionKinematicTiedDefault* InterTiedDefault::getInterTiedDefault() const
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
        Radioss::FITKInteractionKinematicTiedDefault* value = new Radioss::FITKInteractionKinematicTiedDefault();
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
            == Radioss::FITKInteractionDefaultAbstract::DefaultInteractionType::DIT_KinematicTied)
        {
            return dynamic_cast<Radioss::FITKInteractionKinematicTiedDefault*>(value);
        }
    }

    Radioss::FITKInteractionKinematicTiedDefault* value = new Radioss::FITKInteractionKinematicTiedDefault();
    manager->appendDataObj(value);
    return value;
}
Radioss::FITKInteractionKinematicTiedDefault* InterTiedDefaultWapper::getInterTiedDefault(InterTiedDefault* obj)
{
    if (obj == nullptr)
    {
        return nullptr;
    }

    return obj->getInterTiedDefault();
}

InterTiedDefault* InterTiedDefaultWapper::new_InterTiedDefault()
{
    return new InterTiedDefault();
}

void InterTiedDefaultWapper::delete_InterTiedDefault(InterTiedDefault* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

QString InterTiedDefaultWapper::setIgnoreFlag(InterTiedDefault* obj, int flag)
{
    Radioss::FITKInteractionKinematicTiedDefault* value = getInterTiedDefault(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    value->setIgnoreFlag(flag);

    return sendMessageToServer(tr("Success, Ignore flag set to %1").arg(flag));
}

QString InterTiedDefaultWapper::setSpotWeldFormulationFlag(InterTiedDefault* obj, int flag)
{
    Radioss::FITKInteractionKinematicTiedDefault* value = getInterTiedDefault(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    value->setSpotWeldFormulationFlag(flag);

    return sendMessageToServer(tr("Success, Spotflag set to %1").arg(flag));
}

QString InterTiedDefaultWapper::setSearchFormulationFlag(InterTiedDefault* obj, int flag)
{
    Radioss::FITKInteractionKinematicTiedDefault* value = getInterTiedDefault(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    value->setSearchFormulationFlag(flag);

    return sendMessageToServer(tr("Success, Isearch set to %1").arg(flag));
}

QString InterTiedDefaultWapper::setNodeDeletionFlag(InterTiedDefault* obj, int flag)
{
    Radioss::FITKInteractionKinematicTiedDefault* value = getInterTiedDefault(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    value->setNodeDeletionFlag(flag);

    return sendMessageToServer(tr("Success, Idel2 set to %1").arg(flag));
}

QString InterTiedDefaultWapper::setIstfFlagDefault(InterTiedDefault* obj, int flag)
{
    Radioss::FITKInteractionKinematicTiedDefault* value = getInterTiedDefault(obj);
    if (value == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    value->setIstfFlagDefault(flag);

    return sendMessageToServer(tr("Success, Istf set to %1").arg(flag));
}