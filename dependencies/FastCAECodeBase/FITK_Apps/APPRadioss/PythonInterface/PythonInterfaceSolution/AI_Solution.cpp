/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file AI_Solution.cpp
 * @brief AI工具封装实现，组合求解设置基础能力
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-20
 */

#include "AI_Solution.h"

#include "FITK_Component/FITKRadiossData/FITKAbstractInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "SolutionWapper.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"

namespace
{
    QString rWallTypeToString(Radioss::FITKAbstractRWall* rWall)
    {
        if (dynamic_cast<Radioss::FITKRWallPlane*>(rWall) != nullptr)
        {
            return "RWallPlane";
        }

        if (dynamic_cast<Radioss::FITKRWallCylinder*>(rWall) != nullptr)
        {
            return "RWallCylinder";
        }

        if (dynamic_cast<Radioss::FITKRWallSpherical*>(rWall) != nullptr)
        {
            return "RWallSpherical";
        }

        return QString();
    }

    QString initialValueTypeToString(Radioss::FITKAbstractInitialField* initialValue)
    {
        if (initialValue == nullptr)
        {
            return QString();
        }

        switch (initialValue->getType())
        {
        case Radioss::FITKAbstractInitialField::TRA:
            return "InitialValueTra";
        case Radioss::FITKAbstractInitialField::AXIS:
            return "InitialValueAxis";
        default:
            return QString();
        }
    }

    QString interactionTypeToString(Radioss::FITKAbstractInteraction* interaction)
    {
        if (interaction == nullptr)
        {
            return QString();
        }

        switch (interaction->getInteractionType())
        {
        case Radioss::FITKAbstractInteraction::IT_KinematicTied:
            return "KinematicTied";
        case Radioss::FITKAbstractInteraction::IT_MultiUsageImpact:
            return "MultiUsageImpact";
        default:
            return QString();
        }
    }

    QString probeTypeToString(Radioss::FITKAbstractProbe* probe)
    {
        if (probe == nullptr)
        {
            return QString();
        }

        switch (probe->getType())
        {
        case Radioss::FITKAbstractProbe::Probe_Acceleration:
            return "ProbeAcceleration";
        case Radioss::FITKAbstractProbe::Probe_Section:
            return "ProbeSection";
        default:
            return QString();
        }
    }

    QString bcsTypeToString(Radioss::FITKAbstractBCS* bcs)
    {
        if (bcs == nullptr)
        {
            return QString();
        }

        switch (bcs->getBCSType())
        {
        case Radioss::FITKAbstractBCS::BCS:
            return "BCS";
        case Radioss::FITKAbstractBCS::BSSCyclic:
            return "BSSCyclic";
        case Radioss::FITKAbstractBCS::BCSLAGMUL:
            return "BCSLAGMUL";
        default:
            return QString();
        }
    }

    QString sendMessageToServer(const QString& message, bool isToServer = true)
    {
        if (isToServer)
        {
            HttpPython::FITKHttpCommons::sendMessageToServer(message);
        }
        return message;
    }
}

QString AI_Solution::getRWallType(const QString& name)
{
    Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
    if (solution == nullptr)
    {
        return QString();
    }

    Radioss::FITKRadiossRWallManager* rWallManager = solution->getRWallManager();
    if (rWallManager == nullptr)
    {
        return QString();
    }

    Radioss::FITKAbstractRWall* rWall = rWallManager->getDataByName(name);
    if (rWall == nullptr)
    {
        return QString();
    }

    return rWallTypeToString(rWall);
}

QString AI_Solution::getInitialValueType(const QString& name)
{
    Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
    if (solution == nullptr)
    {
        return QString();
    }

    Radioss::FITKInitialFieldManager* initialFieldManager = solution->getInitialFieldManager();
    if (initialFieldManager == nullptr)
    {
        return QString();
    }

    Radioss::FITKAbstractInitialField* initialValue = initialFieldManager->getDataByName(name);
    if (initialValue == nullptr)
    {
        return QString();
    }

    return initialValueTypeToString(initialValue);
}

QString AI_Solution::getInteractionType(const QString& name)
{
    Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
    if (solution == nullptr)
    {
        return QString();
    }

    Radioss::FITKInteractionManager* interactionManager = solution->getInteractionManager();
    if (interactionManager == nullptr)
    {
        return QString();
    }

    Radioss::FITKAbstractInteraction* interaction = interactionManager->getDataByName(name);
    if (interaction == nullptr)
    {
        return QString();
    }

    return interactionTypeToString(interaction);
}

QString AI_Solution::getProbeType(const QString& name)
{
    Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
    if (solution == nullptr)
    {
        return QString();
    }

    Radioss::FITKProbeManager* probeManager = solution->getProbeManager();
    if (probeManager == nullptr)
    {
        return QString();
    }

    Radioss::FITKAbstractProbe* probe = probeManager->getDataByName(name);
    if (probe == nullptr)
    {
        return QString();
    }

    return probeTypeToString(probe);
}

QString AI_Solution::getBCSType(const QString& name)
{
    Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
    if (solution == nullptr)
    {
        return QString();
    }

    Radioss::FITKRadiossBCSManager* bcsManager = solution->getBCSManager();
    if (bcsManager == nullptr)
    {
        return QString();
    }

    Radioss::FITKAbstractBCS* bcs = bcsManager->getDataByName(name);
    if (bcs == nullptr)
    {
        return QString();
    }

    return bcsTypeToString(bcs);
}

AI_Solution* AI_SolutionWapper::new_AI_Solution()
{
    return new AI_Solution();
}

void AI_SolutionWapper::delete_AI_Solution(AI_Solution* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

QString AI_SolutionWapper::getRWallType(AI_Solution* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Error! Object destroyed.");
    }

    if (name.isEmpty())
    {
        return sendMessageToServer("Error! Name is empty.");
    }

    QString result = obj->getRWallType(name);
    if (result.isEmpty())
    {
        return sendMessageToServer("Error! RWall not found!");
    }

    return sendMessageToServer(QString("The rigid wall exists, and the rigid wall type is %1").arg(result));
}

QString AI_SolutionWapper::getInitialValueType(AI_Solution* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Error! Object destroyed.");
    }

    if (name.isEmpty())
    {
        return sendMessageToServer("Error! Name is empty.");
    }

    QString result = obj->getInitialValueType(name);
    if (result.isEmpty())
    {
        return sendMessageToServer("Error! Initial value not found!");
    }

    return sendMessageToServer(QString("The initial value exists, and the initial value type is %1").arg(result));
}

QString AI_SolutionWapper::getInteractionType(AI_Solution* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Error! Object destroyed.");
    }

    if (name.isEmpty())
    {
        return sendMessageToServer("Error! Name is empty.");
    }

    QString result = obj->getInteractionType(name);
    if (result.isEmpty())
    {
        return sendMessageToServer("Error! Interaction not found!");
    }

    return sendMessageToServer(QString("The interaction exists, and the interaction type is %1").arg(result));
}

QString AI_SolutionWapper::getProbeType(AI_Solution* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Error! Object destroyed.");
    }

    if (name.isEmpty())
    {
        return sendMessageToServer("Error! Name is empty.");
    }

    QString result = obj->getProbeType(name);
    if (result.isEmpty())
    {
        return sendMessageToServer("Error! Probe not found!");
    }

    return sendMessageToServer(QString("The probe exists, and the probe type is %1").arg(result));
}

QString AI_SolutionWapper::getBCSType(AI_Solution* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Error! Object destroyed.");
    }

    if (name.isEmpty())
    {
        return sendMessageToServer("Error! Name is empty.");
    }

    QString result = obj->getBCSType(name);
    if (result.isEmpty())
    {
        return sendMessageToServer("Error! BCS not found!");
    }

    return sendMessageToServer(QString("The boundary condition exists, and the boundary condition type is %1").arg(result));
}