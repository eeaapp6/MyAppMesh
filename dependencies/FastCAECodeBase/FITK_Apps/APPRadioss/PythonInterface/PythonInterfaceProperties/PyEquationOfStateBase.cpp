/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyEquationOfStateBase.h"

#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"
#include "FITK_Component/FITKRadiossData/FITKEquationOfStatePolynomial.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/TreeEventOperator.h"

namespace PyInterface
{
    namespace
    {
        Interface::FITKAbstractMaterial* getMaterialByName(const QString& materialName)
        {
            if (materialName.isEmpty())
            {
                return nullptr;
            }

            Radioss::FITKRadiossCase* dataCase =
                FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
            if (dataCase == nullptr)
            {
                return nullptr;
            }

            Interface::FITKMaterialManager* materialManager = dataCase->getMaterialManager();
            if (materialManager == nullptr)
            {
                return nullptr;
            }

            return materialManager->getDataByName(materialName);
        }
    }

    PyEquationOfStateBase::PyEquationOfStateBase(const QString& name)
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return;
        }

        _equationOfStateManager = dataCase->getEquationOfState();
        if (_equationOfStateManager == nullptr)
        {
            return;
        }

        _equationOfState = _equationOfStateManager->getDataByName(name);
    }

    PyEquationOfStateBase::PyEquationOfStateBase()
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return;
        }

        _equationOfStateManager = dataCase->getEquationOfState();
    }

    QString PyEquationOfStateBase::getName()
    {
        if (_equationOfState == nullptr)
        {
            return QString();
        }

        return _equationOfState->getDataObjectName();
    }

    bool PyEquationOfStateBase::setName(const QString& name)
    {
        if (name.isEmpty() || _equationOfState == nullptr || _equationOfStateManager == nullptr)
        {
            return false;
        }

        Radioss::FITKAbstractEquationOfState* sameNameEquationOfState =
            _equationOfStateManager->getDataByName(name);
        if (sameNameEquationOfState != nullptr && sameNameEquationOfState != _equationOfState)
        {
            return false;
        }

        _equationOfState->setDataObjectName(name);
        updateRender();
        return true;
    }

    bool PyEquationOfStateBase::createEquationOfState(const QString& name, int type)
    {
        if (_equationOfStateManager == nullptr)
        {
            return false;
        }

        QString equationName = name;
        if (equationName.isEmpty())
        {
            equationName = _equationOfStateManager->checkName("EOS-1");
        }

        if (_equationOfStateManager->getDataByName(equationName) != nullptr)
        {
            return false;
        }

        switch (type)
        {
        case Radioss::FITKAbstractEquationOfState::Polynomial:
            _equationOfState = new Radioss::FITKEquationOfStatePolynomial();
            break;
        default:
            _equationOfState = nullptr;
            break;
        }

        if (_equationOfState == nullptr)
        {
            return false;
        }

        _equationOfState->setDataObjectName(equationName);
        _equationOfStateManager->appendDataObj(_equationOfState);
        updateRender();
        return true;
    }

    bool PyEquationOfStateBase::addMaterial(const QString& materialName)
    {
        if (_equationOfState == nullptr)
        {
            return false;
        }

        Interface::FITKAbstractMaterial* material = getMaterialByName(materialName);
        if (material == nullptr)
        {
            return false;
        }

        _equationOfState->addMaterialID(material->getDataObjectID(), _equationOfStateManager);
        return true;
    }

    bool PyEquationOfStateBase::removeMaterial(const QString& materialName)
    {
        if (_equationOfState == nullptr)
        {
            return false;
        }

        Interface::FITKAbstractMaterial* material = getMaterialByName(materialName);
        if (material == nullptr)
        {
            return false;
        }

        _equationOfState->removeMaterialID(material->getDataObjectID());
        return true;
    }

    bool PyEquationOfStateBase::setP0(double value)
    {
        if (_equationOfState == nullptr)
        {
            return false;
        }

        _equationOfState->setP0(value);
        return true;
    }

    Radioss::FITKAbstractEquationOfState* PyEquationOfStateBase::getEquationOfState()
    {
        return _equationOfState;
    }

    void PyEquationOfStateBase::updateRender()
    {
        QStringList listOpertor = { "TreePropertyEvent" };
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
}