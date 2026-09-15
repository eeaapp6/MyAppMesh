/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyPropertyBase.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "FITK_Component/FITKRadiossData/FITKPropSolid.h"
#include "FITK_Component/FITKRadiossData/FITKPropShell.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeam.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeamSPR.h"
#include "FITK_Component/FITKRadiossData/FITKPropSandwichShell.h"
#include "FITK_Component/FITKRadiossData/FITKPropSPH.h"

#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace PyInterface
{
    PyPropertyBase::PyPropertyBase(const QString& name)
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return;
        }

        _propertyManager = dataCase->getSectionManager();
        if (_propertyManager == nullptr)
        {
            return;
        }

        _property = static_cast<Radioss::FITKPropAbstract*>(_propertyManager->getDataByName(name));
    }

    PyPropertyBase::PyPropertyBase()
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return;
        }

        _propertyManager = dataCase->getSectionManager();
    }

    QString PyPropertyBase::getName()
    {
        if (_property != nullptr)
        {
            return _property->getDataObjectName();
        }

        return QString();
    }

    bool PyPropertyBase::setName(const QString& name)
    {
        if (name == "" || _property == nullptr || _propertyManager == nullptr)
        {
            return false;
        }

        if (_propertyManager->getDataByName(name) != nullptr && _property->getDataObjectName() != name)
        {
            return false;
        }

        _property->setDataObjectName(name);
        return true;
    }

    bool PyPropertyBase::createProperty(const QString& name, int type)
    {
        if (!_propertyManager)
            return false;

        //生成默认名称
        QString propertyName = name;
        if (propertyName == "")
        {
            propertyName = _propertyManager->checkName("Property-1");
        }
        if(_propertyManager->getDataByName(propertyName))
            return false;

        switch(type)
        {
        case Radioss::FITKPropAbstract::Solid:
            _property = new Radioss::FITKPropSolid();
            break;
        case Radioss::FITKPropAbstract::Shell:
            _property = new Radioss::FITKPropShell();
            break;
        case Radioss::FITKPropAbstract::Beam:
            _property = new Radioss::FITKPropBeam();
            break;
        case Radioss::FITKPropAbstract::BeamSPR:
            _property = new Radioss::FITKPropBeamSPR();
            break;
        case Radioss::FITKPropAbstract::Sandwich:
            _property = new Radioss::FITKPropSandwichShell();
            break;
        case Radioss::FITKPropAbstract::SPH:
            _property = new Radioss::FITKPropSPH();
            break;
        }
        if(!_property)
        {
            return false;
        }
        _property->setDataObjectName(propertyName);
        _propertyManager->appendDataObj(_property);

        updateRender();
        return true;
    }

    Radioss::FITKPropAbstract* PyPropertyBase::getProperty()
    {
        return _property;
    }

    void PyPropertyBase::updateRender()
    {
        QStringList listOpertor = { "TreePropertyEvent" };
        for (int i = 0; i < listOpertor.size(); ++i)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr)
            {
                oper->updateTree();
            }
        }
    }
}