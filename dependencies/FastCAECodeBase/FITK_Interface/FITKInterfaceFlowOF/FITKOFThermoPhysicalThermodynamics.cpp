/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFThermoPhysicalThermodynamics.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFAbsThermoPhysicalThermodynamics::FITKOFAbsThermoPhysicalThermodynamics()
    {
        //初始化
        _thermodynamicsPara = new Core::FITKParameter;
    }

    FITKOFAbsThermoPhysicalThermodynamics::~FITKOFAbsThermoPhysicalThermodynamics()
    {
        //销毁对象
        if (_thermodynamicsPara) delete _thermodynamicsPara;
    }

    Core::FITKParameter * FITKOFAbsThermoPhysicalThermodynamics::getThermodynamicsPara()
    {
        //获取Thermodynamics参数
        return _thermodynamicsPara;
    }

    void FITKOFAbsThermoPhysicalThermodynamics::initThermodynamicsParam()
    {
        //设置参数
    }

    FITKOFThermoPhysicalThermodynamicsConstant::FITKOFThermoPhysicalThermodynamicsConstant()
    {
        //初始化
        this->setDataObjectName("Constant");
        this->initThermodynamicsParam();
    }

    FITKOFThermoPhysicalThermodynamicsConstant::~FITKOFThermoPhysicalThermodynamicsConstant()
    {
    }

    void FITKOFThermoPhysicalThermodynamicsConstant::initThermodynamicsParam()
    {
        //设置参数
        if (!_thermodynamicsPara) return;
        _thermodynamicsPara->createDoubleParameter("Cp", 1005.0);
        _thermodynamicsPara->createDoubleParameter("Hf", 0.0);
    }

    FITKOFThermoPhysicalThermodynamicsEConstant::FITKOFThermoPhysicalThermodynamicsEConstant()
    {
        //初始化
        this->setDataObjectName("eConstant");
        this->initThermodynamicsParam();
    }

    FITKOFThermoPhysicalThermodynamicsEConstant::~FITKOFThermoPhysicalThermodynamicsEConstant()
    {
    }

    void FITKOFThermoPhysicalThermodynamicsEConstant::initThermodynamicsParam()
    {
        //设置参数
        if (!_thermodynamicsPara) return;
        _thermodynamicsPara->createDoubleParameter("Cv", 1005.0);
        _thermodynamicsPara->createDoubleParameter("Hf", 0.0);
    }

}
