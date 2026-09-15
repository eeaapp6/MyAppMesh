/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyRWallBase.h"
#include "SolutionWapper.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

#include "OperatorsInterface/TreeEventOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace PyInterface
{
    PyRWallBase::PyRWallBase(const QString & name)
    {
        Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
        if (solution == nullptr)
        {
            return;
        }
        _rWallManager = solution->getRWallManager();
        if (_rWallManager == nullptr)
        {
            return;
        }
        _rWall = _rWallManager->getDataByName(name);
    }

    PyRWallBase::PyRWallBase()
    {
        Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
        if (solution == nullptr)
        {
            return;
        }
        _rWallManager = solution->getRWallManager();
    }

    QString PyRWallBase::getName()
    {
        if (_rWall == nullptr)
        {
            return QString();
        }

        return _rWall->getDataObjectName();
    }

    bool PyRWallBase::setName(const QString & name)
    {
        if (name == "" || _rWall == nullptr || _rWallManager == nullptr)
        {
            return false;
        }

        if (_rWallManager->getDataByName(name) != nullptr && _rWall->getDataObjectName() != name)
        {
            return false;
        }

        _rWall->setDataObjectName(name);
        updateRender();
        return true;
    }

    bool PyRWallBase::createRWall(const QString & name, int type)
    {
        if (!_rWallManager)
        {
            return false;
        }

        //生成默认名称
        QString rWallName = name;
        if (rWallName == "")
        {
            rWallName = _rWallManager->checkName("RWall-1");
        }
        if(_rWallManager->getDataByName(rWallName))
            return false;

        switch (type)
        {
        case Radioss::FITKAbstractRWall::RWallPLANE:
            _rWall = new Radioss::FITKRWallPlane();
            break;
        case Radioss::FITKAbstractRWall::RWallCYL:
            _rWall = new Radioss::FITKRWallCylinder();
            break;
        case Radioss::FITKAbstractRWall::RWallSPHER:
            _rWall = new Radioss::FITKRWallSpherical();
            break;
        default:
            _rWall = nullptr;
            break;
        }

        if (_rWall == nullptr)
        {
            return false;
        }

        _rWall->setDataObjectName(rWallName);
        _rWallManager->appendDataObj(_rWall);
        updateRender();
        return true;
    }

    //  bool PyRWallBase::setNodeId(int value)
    // {
    //     if (_rWall == nullptr)
    //     {
    //         return false;
    //     }
    //     _rWall->setNodeId(value);
    //     return true;
    // }

    bool PyRWallBase::setSlidingType(int type)
    {
        if (_rWall == nullptr)
        {
            return false;
        }
        if(type < Radioss::FITKAbstractRWall::Sliding ||
           type > Radioss::FITKAbstractRWall::SlidingWithFriction)
        {
            return false;
        }

        _rWall->setSlidingType(static_cast<Radioss::FITKAbstractRWall::RWallSliding>(type));
        return true;
    }

    bool PyRWallBase::setDSearch(double value)
    {
        if (_rWall == nullptr)
        {
            return false;
        }

        _rWall->setDSearch(value);
        return true;
    }

    bool PyRWallBase::setFilteringFactor(double value)
    {
        if (_rWall == nullptr)
        {
            return false;
        }

        _rWall->setFilteringFactor(value);
        return true;
    }

    bool PyRWallBase::setFilteringFlag(int value)
    {
        if (_rWall == nullptr)
        {
            return false;
        }

        _rWall->setFilteringFlag(value);
        return true;
    }

    bool PyRWallBase::setNodeGroup1(const QString& name)
    {
        if (_rWall == nullptr || name == "")
            return false;

        Radioss::FITKRadiossCase* radiossCase =
            FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr)
            return false;

        Radioss::FITKRadiossMeshModel* radMeshModel = radiossCase->getMeshModel();
        if (radMeshModel == nullptr)
            return false;
        Interface::FITKComponentManager* setSurfManager = radMeshModel->getComponentManager();
        if (setSurfManager == nullptr)
            return false;

        Interface::FITKAbstractModelComponent* component = setSurfManager->getDataByName(name);
        if (component == nullptr)
            return false;

        _rWall->setNodeGroupId1(component->getDataObjectID());
        return true;
    }

    bool PyRWallBase::setNodeGroup2(const QString& name)
    {
        if (_rWall == nullptr || name == "")
            return false;

        Radioss::FITKRadiossCase* radiossCase =
            FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr)
            return false;

        Radioss::FITKRadiossMeshModel* radMeshModel = radiossCase->getMeshModel();
        if (radMeshModel == nullptr)
            return false;

        Interface::FITKComponentManager* setSurfManager = radMeshModel->getComponentManager();
        if (setSurfManager == nullptr)
            return false;

        Interface::FITKAbstractModelComponent* component = setSurfManager->getDataByName(name);
        if (component == nullptr)
            return false;

        _rWall->setNodeGroupId2(component->getDataObjectID());
        return true;
    }

    Radioss::FITKAbstractRWall * PyRWallBase::getRWall()
    {
        return _rWall;
    }

    void PyRWallBase::updateRender()
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
         //更新渲染
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateGraph(_rWall->getDataObjectID(),true);
        }
    }
}