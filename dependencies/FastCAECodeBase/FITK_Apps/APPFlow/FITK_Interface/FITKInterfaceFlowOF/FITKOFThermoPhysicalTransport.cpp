/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFThermoPhysicalTransport.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFAbsThermoPhysicalTransport::FITKOFAbsThermoPhysicalTransport()
    {
        //初始化
        _transportPara = new Core::FITKParameter;
    }

    FITKOFAbsThermoPhysicalTransport::~FITKOFAbsThermoPhysicalTransport()
    {
        //销毁对象
        if (_transportPara) delete _transportPara;
    }


    Core::FITKParameter * FITKOFAbsThermoPhysicalTransport::getTransportPara()
    {
        //获取Transport参数
        return _transportPara;
    }

    void FITKOFAbsThermoPhysicalTransport::initTransportParam()
    {
        //设置参数
    }

    FITKOFThermoPhysicalTransportConstant::FITKOFThermoPhysicalTransportConstant()
    {
        //初始化
        this->setDataObjectName("Constant");
        this->initTransportParam();
    }

    FITKOFThermoPhysicalTransportConstant::~FITKOFThermoPhysicalTransportConstant()
    {
    }

    void FITKOFThermoPhysicalTransportConstant::initTransportParam()
    {
        //设置参数
        if (!_transportPara) return;
        _transportPara->createDoubleParameter("mu", 1.8E-5);
        _transportPara->createDoubleParameter("Pr", 0.7);
    }

    FITKOFThermoPhysicalTransportSutherland::FITKOFThermoPhysicalTransportSutherland()
    {
        //初始化
        this->setDataObjectName("Sutherland");
        this->initTransportParam();
    }

    FITKOFThermoPhysicalTransportSutherland::~FITKOFThermoPhysicalTransportSutherland()
    {
    }

    void FITKOFThermoPhysicalTransportSutherland::initTransportParam()
    {
        //设置参数
        if (!_transportPara) return;
        _transportPara->createDoubleParameter("As", 1.67212E-6);
        _transportPara->createDoubleParameter("Ts", 170.672);
    }

    FITKOFThermoPhysicalTransportConstantIsotropic::FITKOFThermoPhysicalTransportConstantIsotropic()
    {
        //初始化
        this->setDataObjectName("Constant Isotropic");
        this->initTransportParam();
    }

    FITKOFThermoPhysicalTransportConstantIsotropic::~FITKOFThermoPhysicalTransportConstantIsotropic()
    {
    }

    void FITKOFThermoPhysicalTransportConstantIsotropic::initTransportParam()
    {
        //设置参数
        if (!_transportPara) return;
        _transportPara->createDoubleParameter("kappa", 1.17);
    }

}
