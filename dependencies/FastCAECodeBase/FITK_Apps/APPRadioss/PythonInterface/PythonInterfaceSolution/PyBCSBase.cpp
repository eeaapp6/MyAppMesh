/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyBCSBase.h"

#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

namespace PyInterface
{
    PyBCSBase::PyBCSBase(const QString& name)
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return;
        }

        _bcsManager = dataCase->getBCSManager();
        if (_bcsManager == nullptr)
        {
            return;
        }

        _bcs = _bcsManager->getDataByName(name);
    }

    PyBCSBase::PyBCSBase()
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return;
        }

        _bcsManager = dataCase->getBCSManager();
    }

    QString PyBCSBase::getName()
    {
        if (_bcs == nullptr)
        {
            return QString();
        }

        return _bcs->getDataObjectName();
    }

    bool PyBCSBase::setName(const QString& name)
    {
        if (name.isEmpty() || _bcs == nullptr || _bcsManager == nullptr)
        {
            return false;
        }

        Radioss::FITKAbstractBCS* sameNameBCS = _bcsManager->getDataByName(name);
        if (sameNameBCS != nullptr && sameNameBCS != _bcs)
        {
            return false;
        }

        _bcs->setDataObjectName(name);
        updateRender();
        return true;
    }

    bool PyBCSBase::createBCS(const QString& name, int type)
    {
        if (_bcsManager == nullptr)
        {
            return false;
        }

        QString bcsName = name;
        if (bcsName.isEmpty())
        {
            bcsName = _bcsManager->checkName("BCS-1");
        }

        if (_bcsManager->getDataByName(bcsName) != nullptr)
        {
            return false;
        }

        switch (type)
        {
        case Radioss::FITKAbstractBCS::BCS:
            _bcs = new Radioss::FITKRadiossBCS();
            break;
        case Radioss::FITKAbstractBCS::BSSCyclic:
            _bcs = new Radioss::FITKRadiossBCSCyclic();
            break;
        case Radioss::FITKAbstractBCS::BCSLAGMUL:
            _bcs = new Radioss::FITKRadiossBCSLagMul();
            break;
        default:
            _bcs = nullptr;
            break;
        }

        if (_bcs == nullptr)
        {
            return false;
        }

        _bcs->setDataObjectName(bcsName);
        _bcsManager->appendDataObj(_bcs);
        updateRender();
        return true;
    }

    // bool PyBCSBase::setCoordinateSystemID(int id)
    // {
    //     if (_bcs == nullptr)
    //     {
    //         return false;
    //     }

    //     _bcs->setCoordinateSystemID(id);
    //     updateRender();
    //     return true;
    // }

    bool PyBCSBase::setNodeGroup1(const QString& name)
    {
        if (_bcs == nullptr || name.isEmpty())
        {
            return false;
        }

        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return false;
        }

        Radioss::FITKRadiossMeshModel* meshModel = dataCase->getMeshModel();
        if (meshModel == nullptr)
        {
            return false;
        }

        Interface::FITKComponentManager* componentManager = meshModel->getComponentManager();
        if (componentManager == nullptr)
        {
            return false;
        }

        Interface::FITKAbstractModelComponent* component = componentManager->getDataByName(name);
        if (component == nullptr)
        {
            return false;
        }

        _bcs->setGroupID(component->getDataObjectID(), _bcs->getGroupID2());
        updateRender();
        return true;
    }

    bool PyBCSBase::setNodeGroup2(const QString& name)
    {
        if (_bcs == nullptr || name.isEmpty())
        {
            return false;
        }

        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return false;
        }

        Radioss::FITKRadiossMeshModel* meshModel = dataCase->getMeshModel();
        if (meshModel == nullptr)
        {
            return false;
        }

        Interface::FITKComponentManager* componentManager = meshModel->getComponentManager();
        if (componentManager == nullptr)
        {
            return false;
        }

        Interface::FITKAbstractModelComponent* component = componentManager->getDataByName(name);
        if (component == nullptr)
        {
            return false;
        }

        _bcs->setGroupID(_bcs->getGroupID1(), component->getDataObjectID());
        updateRender();
        return true;
    }

    Radioss::FITKAbstractBCS* PyBCSBase::getBCS()
    {
        return _bcs;
    }

    void PyBCSBase::updateRender()
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

        if (_bcs == nullptr)
        {
            return;
        }

        EventOper::GraphEventOperator* operGraph =
            FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph != nullptr)
        {
            operGraph->updateGraph(_bcs->getDataObjectID(), true);
        }
    }
}