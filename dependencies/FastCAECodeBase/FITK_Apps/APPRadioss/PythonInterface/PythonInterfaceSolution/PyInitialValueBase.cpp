/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyInitialValueBase.h"

#include "SolutionWapper.h"

#include "FITK_Component/FITKRadiossData/FITKAbstractInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace PyInterface
{
    PyInitialValueBase::PyInitialValueBase(const QString& name)
    {
        Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
        if (solution == nullptr)
        {
            return;
        }

        _initialValueManager = solution->getInitialFieldManager();
        if (_initialValueManager == nullptr)
        {
            return;
        }

        _initialValue = _initialValueManager->getDataByName(name);
    }

    PyInitialValueBase::PyInitialValueBase()
    {
        Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
        if (solution == nullptr)
        {
            return;
        }

        _initialValueManager = solution->getInitialFieldManager();
    }

    QString PyInitialValueBase::getName()
    {
        if (_initialValue == nullptr)
        {
            return QString();
        }

        return _initialValue->getDataObjectName();
    }

    bool PyInitialValueBase::setName(const QString& name)
    {
        if (name == "" || _initialValue == nullptr || _initialValueManager == nullptr)
        {
            return false;
        }

        if (_initialValueManager->getDataByName(name) != nullptr && _initialValue->getDataObjectName() != name)
        {
            return false;
        }

        _initialValue->setDataObjectName(name);
        updateRender();
        return true;
    }

    bool PyInitialValueBase::createInitialValue(const QString& name, int type)
    {
        if (_initialValueManager == nullptr)
        {
            return false;
        }
        QString initV_name = name;
        if (initV_name.isEmpty())
        {
            initV_name = _initialValueManager->checkName("InitVal-1");
        }

        if (_initialValueManager->getDataByName(initV_name) != nullptr)
        {
            return false;
        }

        switch (type)
        {
        case Radioss::FITKAbstractInitialField::TRA:
            _initialValue = new Radioss::FITKInitialFieldTra();
            break;
        case Radioss::FITKAbstractInitialField::AXIS:
            _initialValue = new Radioss::FITKInitialFieldAxis();
            break;
        default:
            _initialValue = nullptr;
            break;
        }

        if (_initialValue == nullptr)
        {
            return false;
        }

        _initialValue->setDataObjectName(initV_name);
        _initialValueManager->appendDataObj(_initialValue);
        updateRender();
        return true;
    }

    bool PyInitialValueBase::setValue(double x, double y, double z)
    {
        if (_initialValue == nullptr)
        {
            return false;
        }

        double value[3] = { x, y, z };
        _initialValue->setValue(value);
        return true;
    }

    bool PyInitialValueBase::setNodeGroup(const QString& name)
    {
        if (_initialValue == nullptr || name == "")
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

        _initialValue->setSetID(component->getDataObjectID());
        updateRender();
        return true;
    }

    // bool PyInitialValueBase::setSystemID(int systemID)
    // {
    //     if (_initialValue == nullptr)
    //     {
    //         return false;
    //     }

    //     _initialValue->setSystemID(systemID);
    //     return true;
    // }

    Radioss::FITKAbstractInitialField* PyInitialValueBase::getInitialValue()
    {
        return _initialValue;
    }

    void PyInitialValueBase::updateRender()
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

        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph != nullptr)
        {
            operGraph->updateGraph(_initialValue->getDataObjectID(), true);
        }
    }
}