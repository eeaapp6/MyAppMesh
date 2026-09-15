/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFRadiationScatterModel.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFAbsRadiationScatterModel::FITKOFAbsRadiationScatterModel()
    {
        //初始化
        _radiationScatterModelParams = new Core::FITKParameter;
    }

    FITKOFAbsRadiationScatterModel::~FITKOFAbsRadiationScatterModel()
    {
        //销毁对象
        if (_radiationScatterModelParams) delete _radiationScatterModelParams;
    }

    Core::FITKParameter* FITKOFAbsRadiationScatterModel::getRadiationScatterModelPara()
    {
        //获取参数
        return _radiationScatterModelParams;
    }

    void FITKOFAbsRadiationScatterModel::initRadiationScatterModelParam()
    {
        //设置参数
    }

    FITKOFRadiationScatterModelNone::FITKOFRadiationScatterModelNone()
    {
        //初始化
        this->setDataObjectName("none");
        this->initRadiationScatterModelParam();
    }

    FITKOFRadiationScatterModelNone::~FITKOFRadiationScatterModelNone()
    {
        //销毁对象
    }

    void FITKOFRadiationScatterModelNone::initRadiationScatterModelParam()
    {
        //设置参数
        if (!_radiationScatterModelParams) return;

    }

    FITKOFRadiationScatterModelConstant::FITKOFRadiationScatterModelConstant()
    {
        //初始化
        this->setDataObjectName("Constant");
        this->initRadiationScatterModelParam();
    }

    FITKOFRadiationScatterModelConstant::~FITKOFRadiationScatterModelConstant()
    {
        //销毁对象
    }

    void FITKOFRadiationScatterModelConstant::initRadiationScatterModelParam()
    {
        //设置参数
        if (!_radiationScatterModelParams) return;
        _radiationScatterModelParams->createDoubleParameter("sigma [1/m]", 0.0);
        _radiationScatterModelParams->createDoubleParameter("C [-]", 0.0);
    }
}
