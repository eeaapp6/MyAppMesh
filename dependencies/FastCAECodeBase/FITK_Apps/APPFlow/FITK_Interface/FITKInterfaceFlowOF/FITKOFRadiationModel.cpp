/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFRadiationModel.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFAbsRadiationModel::FITKOFAbsRadiationModel()
    {
        //初始化
        _radiationModelParams = new Core::FITKParameter;
    }

    FITKOFAbsRadiationModel::~FITKOFAbsRadiationModel()
    {
        //销毁对象
        if (_radiationModelParams) delete _radiationModelParams;
    }

    Core::FITKParameter* FITKOFAbsRadiationModel::getRadiationModelPara()
    {
        //获取参数
        return _radiationModelParams;
    }

    void FITKOFAbsRadiationModel::initRadiationModelParam()
    {
        //设置参数
    }

    FITKOFRadiationModelNone::FITKOFRadiationModelNone()
    {
        //初始化
        this->setDataObjectName("none");
        this->initRadiationModelParam();
    }

    FITKOFRadiationModelNone::~FITKOFRadiationModelNone()
    {
        //销毁对象
    }

    void FITKOFRadiationModelNone::initRadiationModelParam()
    {
        //设置参数
        if (!_radiationModelParams) return;

    }

    FITKOFRadiationModelP1::FITKOFRadiationModelP1()
    {
        //初始化
        this->setDataObjectName("P1");
        this->initRadiationModelParam();
    }

    FITKOFRadiationModelP1::~FITKOFRadiationModelP1()
    {
        //销毁对象
    }

    void FITKOFRadiationModelP1::initRadiationModelParam()
    {
        //设置参数
        if (!_radiationModelParams) return;
    }

    FITKOFRadiationModelDiscreteOrdinates::FITKOFRadiationModelDiscreteOrdinates()
    {
        //初始化
        this->setDataObjectName("Discrete Ordinates");
        this->initRadiationModelParam();
    }

    FITKOFRadiationModelDiscreteOrdinates::~FITKOFRadiationModelDiscreteOrdinates()
    {
        //销毁对象
    }

    void FITKOFRadiationModelDiscreteOrdinates::initRadiationModelParam()
    {
        //设置参数
        if (!_radiationModelParams) return;
        _radiationModelParams->createIntParameter("nPhi", 3);
        _radiationModelParams->createIntParameter("nTheta", 5);
        _radiationModelParams->createDoubleParameter("convergence", 0.001);
        _radiationModelParams->createIntParameter("maxIter", 10);
    }

    FITKOFRadiationModelSurfToSurf::FITKOFRadiationModelSurfToSurf()
    {
        //初始化
        this->setDataObjectName("Surface to Surface");
        this->initRadiationModelParam();
    }

    FITKOFRadiationModelSurfToSurf::~FITKOFRadiationModelSurfToSurf()
    {
        //销毁对象
    }

    void FITKOFRadiationModelSurfToSurf::initRadiationModelParam()
    {
        //设置参数
        if (!_radiationModelParams) return;
        _radiationModelParams->createBoolParameter("smoothing", true);
        _radiationModelParams->createBoolParameter("constantEmissivity", true);
    }
}
