/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFRadiationSunLoad.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFAbsRadiationSunLoad::FITKOFAbsRadiationSunLoad()
    {
        //初始化
        _radiationSunLoadParams = new Core::FITKParameter;
    }

    FITKOFAbsRadiationSunLoad::~FITKOFAbsRadiationSunLoad()
    {
        //销毁对象
        if (_radiationSunLoadParams) delete _radiationSunLoadParams;
    }

    Core::FITKParameter* FITKOFAbsRadiationSunLoad::getRadiationSunLoadPara()
    {
        //获取参数
        return _radiationSunLoadParams;
    }

    void FITKOFAbsRadiationSunLoad::initRadiationSunLoadParam()
    {
        //设置参数
    }

    FITKOFRadiationSunLoadConstant::FITKOFRadiationSunLoadConstant()
    {
        //初始化
        this->setDataObjectName("Constant");
        this->initRadiationSunLoadParam();
    }

    FITKOFRadiationSunLoadConstant::~FITKOFRadiationSunLoadConstant()
    {
        //销毁对象
    }

    void FITKOFRadiationSunLoadConstant::initRadiationSunLoadParam()
    {
        //设置参数
        if (!_radiationSunLoadParams) return;
        _radiationSunLoadParams->createDoubleParameter("Direct [W/m2]", 1000);
        _radiationSunLoadParams->createDoubleParameter("Diffuse [W/m2]", 0);
    }
}
