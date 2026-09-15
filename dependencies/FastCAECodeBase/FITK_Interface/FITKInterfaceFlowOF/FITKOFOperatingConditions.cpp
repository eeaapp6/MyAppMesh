/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFOperatingConditions.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamRadioGroup.h"

namespace Interface
{
    FITKOFOperatingConditions::FITKOFOperatingConditions()
    {
        _gravitationalAcceleration = new Core::FITKParameter;
        _referencePressure = new Core::FITKParameter;
    }

    FITKOFOperatingConditions::~FITKOFOperatingConditions()
    {
        if (_gravitationalAcceleration) delete _gravitationalAcceleration;
        if (_referencePressure) delete _referencePressure;
    }

    Core::FITKParameter* FITKOFOperatingConditions::getGravitationalAcceleration()
    {
        return _gravitationalAcceleration;
    }

    Core::FITKParameter* FITKOFOperatingConditions::getReferencePressure()
    {
        return _referencePressure;
    }

    void FITKOFOperatingConditions::initGravitationalAcceleration()
    {
        _gravitationalAcceleration->createDoubleListParameter("g [m/s2]", QList<double>() << 0 << 0 << -9.81);
        _gravitationalAcceleration->createDoubleParameter("href [m]", 0);
    }

    void FITKOFOperatingConditions::initReferencePressure()
    {
        Core::FITKParamRadioGroup* location = _referencePressure->createRadioGroupParameter("location");
        Core::FITKParameter* sub = new Core::FITKParameter;
        sub->createIntParameter("Value", 0);
        location->setSubValue(sub);
        Core::FITKParameter* cell = new Core::FITKParameter;
        cell->createIntParameter("Cell", 0);
        location->appendValue("Cell", cell, QList<bool>() << true);
        Core::FITKParameter* point = new Core::FITKParameter;
        point->createDoubleParameter("Pointx", 0);
        point->createDoubleParameter("Pointy", 0);
        point->createDoubleParameter("Pointz", 0);
        location->appendValue("Point", point, QList<bool>() << true);
    }
}
