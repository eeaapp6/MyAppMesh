/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyProbeBase.h"

#include "SolutionWapper.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include "FITK_Component/FITKRadiossData/FITKProbeAcceleration.h"
#include "FITK_Component/FITKRadiossData/FITKProbeSection.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/TreeEventOperator.h"

namespace PyInterface
{
    PyProbeBase::PyProbeBase(const QString& name)
    {
        Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
        if (solution == nullptr)
        {
            return;
        }

        _probeManager = solution->getProbeManager();
        if (_probeManager == nullptr)
        {
            return;
        }

        _probe = _probeManager->getDataByName(name);
    }

    PyProbeBase::PyProbeBase()
    {
        Radioss::FITKRadiossSolution* solution = Solution::getCurrentSolution();
        if (solution == nullptr)
        {
            return;
        }

        _probeManager = solution->getProbeManager();
    }

    QString PyProbeBase::getName()
    {
        if (_probe == nullptr)
        {
            return QString();
        }

        return _probe->getDataObjectName();
    }

    bool PyProbeBase::setName(const QString& name)
    {
        if (name.isEmpty() || _probe == nullptr || _probeManager == nullptr)
        {
            return false;
        }

        Radioss::FITKAbstractProbe* sameNameProbe = _probeManager->getDataByName(name);
        if (sameNameProbe != nullptr && sameNameProbe != _probe)
        {
            return false;
        }

        _probe->setDataObjectName(name);
        updateRender();
        return true;
    }

    bool PyProbeBase::createProbe(const QString& name, int type)
    {
        if (_probeManager == nullptr)
        {
            return false;
        }

        QString probeName = name;
        if (probeName.isEmpty())
        {
            probeName = _probeManager->checkName("Probe-1");
        }

        if (_probeManager->getDataByName(probeName) != nullptr)
        {
            return false;
        }

        switch (type)
        {
        case Radioss::FITKAbstractProbe::Probe_Acceleration:
            _probe = new Radioss::FITKProbeAcceleration();
            break;
        case Radioss::FITKAbstractProbe::Probe_Section:
            _probe = new Radioss::FITKProbeSection();
            break;
        default:
            _probe = nullptr;
            break;
        }

        if (_probe == nullptr)
        {
            return false;
        }

        _probe->setDataObjectName(probeName);
        _probeManager->appendDataObj(_probe);
        updateRender();
        return true;
    }

    Radioss::FITKAbstractProbe* PyProbeBase::getProbe()
    {
        return _probe;
    }

    void PyProbeBase::updateRender()
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
    }
}