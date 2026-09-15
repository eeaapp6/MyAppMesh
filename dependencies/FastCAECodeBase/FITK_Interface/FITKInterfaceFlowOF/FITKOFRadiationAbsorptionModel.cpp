/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFRadiationAbsorptionModel.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFAbsRadiationAbsorptionModel::FITKOFAbsRadiationAbsorptionModel()
    {
        //初始化
        _radiationAbsorptionModelParams = new Core::FITKParameter;
    }

    FITKOFAbsRadiationAbsorptionModel::~FITKOFAbsRadiationAbsorptionModel()
    {
        //销毁对象
        if (_radiationAbsorptionModelParams) delete _radiationAbsorptionModelParams;
    }

    Core::FITKParameter* FITKOFAbsRadiationAbsorptionModel::getRadiationAbsorptionModelPara()
    {
        //获取参数
        return _radiationAbsorptionModelParams;
    }

    void FITKOFAbsRadiationAbsorptionModel::initRadiationAbsorptionModelParam()
    {
        //设置参数
    }

    FITKOFRadiationAbsorptionModelNone::FITKOFRadiationAbsorptionModelNone()
    {
        //初始化
        this->setDataObjectName("none");
        this->initRadiationAbsorptionModelParam();
    }

    FITKOFRadiationAbsorptionModelNone::~FITKOFRadiationAbsorptionModelNone()
    {
        //销毁对象
    }

    void FITKOFRadiationAbsorptionModelNone::initRadiationAbsorptionModelParam()
    {
        //设置参数
        if (!_radiationAbsorptionModelParams) return;

    }

    FITKOFRadiationAbsorptionModelConstant::FITKOFRadiationAbsorptionModelConstant()
    {
        //初始化
        this->setDataObjectName("Constant");
        this->initRadiationAbsorptionModelParam();
    }

    FITKOFRadiationAbsorptionModelConstant::~FITKOFRadiationAbsorptionModelConstant()
    {
        //销毁对象
    }

    void FITKOFRadiationAbsorptionModelConstant::initRadiationAbsorptionModelParam()
    {
        //设置参数
        if (!_radiationAbsorptionModelParams) return;
        _radiationAbsorptionModelParams->createDoubleParameter("emissivity [1/m]", 0.0);
        _radiationAbsorptionModelParams->createDoubleParameter("E [W/m3]", 0.0);
        _radiationAbsorptionModelParams->createDoubleParameter("absorptivity [1/m]", 0.0);
    }
}
