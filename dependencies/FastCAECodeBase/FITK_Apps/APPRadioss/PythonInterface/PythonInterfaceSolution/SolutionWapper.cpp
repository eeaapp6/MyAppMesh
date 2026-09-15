/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SolutionWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

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

Radioss::FITKRadiossSolution* Solution::_solution = nullptr;

Solution::Solution(const QString& name)
{
    setCurrentSolution(name);
}

Solution::Solution()
{
}

QString Solution::getName()
{
    if (_solution == nullptr)
    {
        return QString();
    }

    return _solution->getDataObjectName();
}

bool Solution::setName(const QString& name)
{
    if (name == "")
        return false;
    if (_solution == nullptr)
        return false;

    Radioss::FITKRadiossCase* caseData =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (caseData == nullptr)
        return false;

    Radioss::FITKRadiossSolutionManager* solutionManager = caseData->getSolutionManager();
    if (solutionManager == nullptr)
        return false;

    if (solutionManager->getDataByName(name) != nullptr && _solution->getDataObjectName() != name)
        return false;

    _solution->setDataObjectName(name);
    updateRender();
    return true;
}

bool Solution::createSolution(const QString& name)
{
    if (name == "")
        return false;

    Radioss::FITKRadiossCase* caseData =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (caseData == nullptr)
        return false;

    Radioss::FITKRadiossSolutionManager* solutionManager = caseData->getSolutionManager();
    if (solutionManager == nullptr)
        return false;

    if (solutionManager->getDataByName(name) != nullptr)
        return false;

    Radioss::FITKRadiossSolution* solution = new Radioss::FITKRadiossSolution();
    solution->setDataObjectName(name);
    solutionManager->appendDataObj(solution);
    solutionManager->setCurrentSolution(solution->getDataObjectID());
    _solution = solution;
    updateRender();
    return true;
}

bool Solution::setCurrentSolution(const QString& name)
{     
    Radioss::FITKRadiossCase* caseData =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (caseData == nullptr)
    {
        return false;
    }

    Radioss::FITKRadiossSolutionManager* solutionManager = caseData->getSolutionManager();
    if (solutionManager == nullptr)
    {
        return false;
    }
    _solution = solutionManager->getDataByName(name);
    if (_solution != nullptr)
    {
        solutionManager->setCurrentSolution(_solution->getDataObjectID());
    }
    QStringList listOpertor = { "TreeSolutionEvent" };
    for (int i = 0; i < listOpertor.size(); ++i)
    {
        EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
        if (oper != nullptr)
        {
            oper->updateTree();
        }
    }
    return _solution != nullptr;
}

Radioss::FITKRadiossSolution *Solution::getCurrentSolution()
{
    if(_solution == nullptr)
    {
        Radioss::FITKRadiossCase* caseData =
            FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData != nullptr)
        {
            Radioss::FITKRadiossSolutionManager* solutionManager = caseData->getSolutionManager();
            if (solutionManager != nullptr)
            {
                _solution = solutionManager->getCurrentSolution();
            }
        }
    }
    return _solution;
}

void Solution::updateRender()
{
    QStringList listOpertor = { "TreeSolutionEvent" };
    for (int i = 0; i < listOpertor.size(); ++i)
    {
        EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
        if (oper != nullptr)
        {
            oper->updateTree();
        }
    }
}

Solution* SolutionWapper::new_Solution()
{
    return new Solution();
}

void SolutionWapper::delete_Solution(Solution* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

Solution* SolutionWapper::static_Solution_GetSolution(const QString& name)
{
    return new Solution(name);
}

QString SolutionWapper::getName(Solution* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

void SolutionWapper::setName(Solution* obj, const QString& name)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("Set Solution Name failed!");
        return; 
    }

    obj->setName(name);
}

void SolutionWapper::createSolution(Solution* obj, const QString& name)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("Create Solution failed!");
        return;
    }

    obj->createSolution(name);
}

QString SolutionWapper::static_Solution_setCurrentSolution(const QString& name)
{
    if (!Solution::setCurrentSolution(name))
    {
        return sendMessageToServer("Set Current Solution failed!");
    }
    else
    {
        return sendMessageToServer("Set Current Solution succeeded!");
    }
}