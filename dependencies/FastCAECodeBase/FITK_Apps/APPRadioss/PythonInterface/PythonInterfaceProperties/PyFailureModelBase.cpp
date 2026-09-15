/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyFailureModelBase.h"

#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"
#include "FITK_Component/FITKRadiossData/FITKFailureModelBIQUAD.h"
#include "FITK_Component/FITKRadiossData/FITKFailureModelJOHNSON.h"
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

    PyFailureModelBase::PyFailureModelBase(const QString& name)
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return;
        }

        _failureModelManager = dataCase->getFailureModelManager();
        if (_failureModelManager == nullptr)
        {
            return;
        }

        _failureModel = _failureModelManager->getDataByName(name);
    }

    PyFailureModelBase::PyFailureModelBase()
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return;
        }

        _failureModelManager = dataCase->getFailureModelManager();
    }

    QString PyFailureModelBase::getName()
    {
        if (_failureModel == nullptr)
        {
            return QString();
        }

        return _failureModel->getDataObjectName();
    }

    bool PyFailureModelBase::setName(const QString& name)
    {
        if (name.isEmpty() || _failureModel == nullptr || _failureModelManager == nullptr)
        {
            return false;
        }

        Radioss::FITKAbstractFailureModel* sameNameFailureModel =
            _failureModelManager->getDataByName(name);
        if (sameNameFailureModel != nullptr && sameNameFailureModel != _failureModel)
        {
            return false;
        }

        _failureModel->setDataObjectName(name);
        updateRender();
        return true;
    }

    bool PyFailureModelBase::createFailureModel(const QString& name, int type)
    {
        if (_failureModelManager == nullptr)
        {
            return false;
        }

        QString failureName = name;
        if (failureName.isEmpty())
        {
            failureName = _failureModelManager->checkName("Failure-1");
        }

        if (_failureModelManager->getDataByName(failureName) != nullptr)
        {
            return false;
        }

        switch (type)
        {
        case Radioss::FITKAbstractFailureModel::BIQUAD:
            _failureModel = new Radioss::FITKFailureModelBIQUAD();
            break;
        case Radioss::FITKAbstractFailureModel::JOHNSON:
            _failureModel = new Radioss::FITKFailureModelJOHNSON();
            break;
        default:
            _failureModel = nullptr;
            break;
        }

        if (_failureModel == nullptr)
        {
            return false;
        }

        _failureModel->setDataObjectName(failureName);
        _failureModelManager->appendDataObj(_failureModel);
        updateRender();
        return true;
    }

    bool PyFailureModelBase::addMaterial(const QString& materialName)
    {
        if (_failureModel == nullptr)
        {
            return false;
        }

        Interface::FITKAbstractMaterial* material = getMaterialByName(materialName);
        if (material == nullptr)
        {
            return false;
        }

        _failureModel->addMaterialID(material->getDataObjectID(), _failureModelManager);
        return true;
    }

    bool PyFailureModelBase::removeMaterial(const QString& materialName)
    {
        if (_failureModel == nullptr)
        {
            return false;
        }

        Interface::FITKAbstractMaterial* material = getMaterialByName(materialName);
        if (material == nullptr)
        {
            return false;
        }

        _failureModel->removeMaterialID(material->getDataObjectID());
        return true;
    }

    Radioss::FITKAbstractFailureModel* PyFailureModelBase::getFailureModel()
    {
        return _failureModel;
    }

    void PyFailureModelBase::updateRender()
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