/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyInteractionBase.h"

#include "SolutionWapper.h"

#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionMultiUsageImpact.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

namespace PyInterface
{
    PyInteractionBase::PyInteractionBase(const QString& name)
    {
        Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
        if (solution == nullptr)
        {
            return;
        }

        _interactionManager = solution->getInteractionManager();
        if (_interactionManager == nullptr)
        {
            return;
        }

        _interaction = _interactionManager->getDataByName(name);
    }

    PyInteractionBase::PyInteractionBase()
    {
        Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
        if (solution == nullptr)
        {
            return;
        }

        _interactionManager = solution->getInteractionManager();
    }

    QString PyInteractionBase::getName()
    {
        if (_interaction == nullptr)
        {
            return QString();
        }

        return _interaction->getDataObjectName();
    }

    bool PyInteractionBase::setName(const QString& name)
    {
        if (name.isEmpty() || _interaction == nullptr || _interactionManager == nullptr)
        {
            return false;
        }

        Radioss::FITKAbstractInteraction* sameNameInteraction = _interactionManager->getDataByName(name);
        if (sameNameInteraction != nullptr && sameNameInteraction != _interaction)
        {
            return false;
        }

        _interaction->setDataObjectName(name);
        updateRender();
        return true;
    }

    bool PyInteractionBase::createInteraction(const QString& name, int type)
    {
        if (_interactionManager == nullptr)
        {
            return false;
        }

        QString interactionName = name;
        if (interactionName.isEmpty())
        {
            interactionName = _interactionManager->checkName("Interaction-1");
        }

        if (_interactionManager->getDataByName(interactionName) != nullptr)
        {
            return false;
        }

        switch (type)
        {
        case Radioss::FITKAbstractInteraction::IT_KinematicTied:
            _interaction = new Radioss::FITKInteractionKinematicTied();
            break;
        case Radioss::FITKAbstractInteraction::IT_MultiUsageImpact:
            _interaction = new Radioss::FITKInteractionMultiUsageImpact();
            break;
        default:
            _interaction = nullptr;
            break;
        }

        if (_interaction == nullptr)
        {
            return false;
        }

        _interaction->setDataObjectName(interactionName);
        _interactionManager->appendDataObj(_interaction);
        updateRender();
        return true;
    }

    bool PyInteractionBase::setMasterSurface(const QString& name)
    {
        if (_interaction == nullptr || name.isEmpty())
        {
            return false;
        }

        Radioss::FITKRadiossCase* radiossCase =
            FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr)
        {
            return false;
        }

        Radioss::FITKRadiossMeshModel* radMeshModel = radiossCase->getMeshModel();
        if (radMeshModel == nullptr)
        {
            return false;
        }

        Interface::FITKComponentManager* componentManager = radMeshModel->getComponentManager();
        if (componentManager == nullptr)
        {
            return false;
        }

        Interface::FITKAbstractModelComponent* component = componentManager->getDataByName(name);
        if (component == nullptr)
        {
            return false;
        }

        _interaction->setMasterSurfaceID(component->getDataObjectID());
        updateRender();
        return true;
    }

    bool PyInteractionBase::setNodeGroup(const QString& name)
    {
        if (_interaction == nullptr || name.isEmpty())
        {
            return false;
        }

        Radioss::FITKRadiossCase* radiossCase =
            FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr)
        {
            return false;
        }

        Radioss::FITKRadiossMeshModel* radMeshModel = radiossCase->getMeshModel();
        if (radMeshModel == nullptr)
        {
            return false;
        }

        Interface::FITKComponentManager* componentManager = radMeshModel->getComponentManager();
        if (componentManager == nullptr)
        {
            return false;
        }

        Interface::FITKAbstractModelComponent* component = componentManager->getDataByName(name);
        if (component == nullptr)
        {
            return false;
        }

        _interaction->setNodeGroupID(component->getDataObjectID());
        updateRender();
        return true;
    }

    Radioss::FITKAbstractInteraction* PyInteractionBase::getInteraction()
    {
        return _interaction;
    }

    void PyInteractionBase::updateRender()
    {
        QStringList listOpertor = { "TreeSolutionEvent" };
        for (int i = 0; i < listOpertor.size(); ++i)
        {
            EventOper::TreeEventOperator* oper =
                dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr)
            {
                oper->updateTree();
            }
        }

        if (_interaction == nullptr)
        {
            return;
        }

        EventOper::GraphEventOperator* operGraph =
            FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph != nullptr)
        {
            operGraph->updateGraph(_interaction->getDataObjectID(), true);
        }
    }
}