/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFRadiationSunDirection.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFAbsRadiationSunDirection::FITKOFAbsRadiationSunDirection()
    {
        //初始化
        _radiationSunDirectionParams = new Core::FITKParameter;
    }

    FITKOFAbsRadiationSunDirection::~FITKOFAbsRadiationSunDirection()
    {
        //销毁对象
        if (_radiationSunDirectionParams) delete _radiationSunDirectionParams;
    }

    Core::FITKParameter* FITKOFAbsRadiationSunDirection::getRadiationSunDirectionPara()
    {
        //获取参数
        return _radiationSunDirectionParams;
    }

    void FITKOFAbsRadiationSunDirection::initRadiationSunDirectionParam()
    {
        //设置参数
    }

    FITKOFRadiationSunDirectionConstant::FITKOFRadiationSunDirectionConstant()
    {
        //初始化
        this->setDataObjectName("Constant");
        this->initRadiationSunDirectionParam();
    }

    FITKOFRadiationSunDirectionConstant::~FITKOFRadiationSunDirectionConstant()
    {
        //销毁对象
    }

    void FITKOFRadiationSunDirectionConstant::initRadiationSunDirectionParam()
    {
        //设置参数
        if (!_radiationSunDirectionParams) return;
        _radiationSunDirectionParams->createDoubleListParameter("Direction", QList<double>() << 0 << 0 << -1);
    }

    FITKOFRadiationSunDirectionSunTracking::FITKOFRadiationSunDirectionSunTracking()
    {
        //初始化
        this->setDataObjectName("Sun Tracking");
        this->initRadiationSunDirectionParam();
    }

    FITKOFRadiationSunDirectionSunTracking::~FITKOFRadiationSunDirectionSunTracking()
    {
        //销毁对象
    }

    void FITKOFRadiationSunDirectionSunTracking::initRadiationSunDirectionParam()
    {
        //设置参数
        if (!_radiationSunDirectionParams) return;
        _radiationSunDirectionParams->createDoubleParameter("Local Meridian [h]", 0);
        _radiationSunDirectionParams->createDoubleParameter("Longitude [deg]", 0);
        _radiationSunDirectionParams->createDoubleParameter("Latitude [deg]", 0);
        _radiationSunDirectionParams->createDoubleListParameter("Up Direction [-]", QList<double>() << 0 << 0 << 1);
        _radiationSunDirectionParams->createDoubleListParameter("East Direction [-]", QList<double>() << 1 << 0 << 0);
        _radiationSunDirectionParams->createDoubleParameter("Start Day [day]", 0);
        _radiationSunDirectionParams->createDoubleParameter("Start Time [h]", 0);
        _radiationSunDirectionParams->createDoubleParameter("Tracking Update interval [h]", 1);
    }

}
