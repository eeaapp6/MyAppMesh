/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTransportModel.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKAbsOFTransportModel::FITKAbsOFTransportModel()
    {
        _transModelPara = new Core::FITKParameter;
    }

    FITKAbsOFTransportModel::~FITKAbsOFTransportModel()
    {
        if (_transModelPara) delete _transModelPara;
    }

    Core::FITKParameter* FITKAbsOFTransportModel::getTransportModelPara()
    {
        return _transModelPara;
    }

    void FITKAbsOFTransportModel::initTransportModelParam()
    {

    }

    FITKOFTransModelNewtonian::FITKOFTransModelNewtonian()
    {
        this->setDataObjectName("Newtonian");
        this->initTransportModelParam();
    }

    FITKOFTransModelNewtonian::~FITKOFTransModelNewtonian()
    {
    }

    void FITKOFTransModelNewtonian::initTransportModelParam()
    {
        if (!_transModelPara) return;
        _transModelPara->createDoubleParameter("v [m2/s]", 1.5e-05);
    }
}
