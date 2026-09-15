/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFThermoPhysicalEquationOfState.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFAbsThermoPhysicalEquationOfState::FITKOFAbsThermoPhysicalEquationOfState()
    {
        //初始化
        _equationOfStatePara = new Core::FITKParameter;
    }

    FITKOFAbsThermoPhysicalEquationOfState::~FITKOFAbsThermoPhysicalEquationOfState()
    {
        //销毁对象
        if (_equationOfStatePara) delete _equationOfStatePara;
    }

    Core::FITKParameter * FITKOFAbsThermoPhysicalEquationOfState::getEquationOfStatePara()
    {
        //获取Equation Of State参数
        return _equationOfStatePara;
    }

    void FITKOFAbsThermoPhysicalEquationOfState::initEquationOfStateParam()
    {
        //设置参数
    }

    FITKOFThermoPhysicalEquationOfStatePerfectGas::FITKOFThermoPhysicalEquationOfStatePerfectGas()
    {
        //初始化
        this->setDataObjectName("Perfect Gas");
        this->initEquationOfStateParam();
    }

    FITKOFThermoPhysicalEquationOfStatePerfectGas::~FITKOFThermoPhysicalEquationOfStatePerfectGas()
    {
    }

    void FITKOFThermoPhysicalEquationOfStatePerfectGas::initEquationOfStateParam()
    {
        //设置参数
        if (!_equationOfStatePara) return;
    }

    FITKOFThermoPhysicalEquationOfStateBoussinesq::FITKOFThermoPhysicalEquationOfStateBoussinesq()
    {
        //初始化
        this->setDataObjectName("Boussinesq");
        this->initEquationOfStateParam();
    }

    FITKOFThermoPhysicalEquationOfStateBoussinesq::~FITKOFThermoPhysicalEquationOfStateBoussinesq()
    {
    }

    void FITKOFThermoPhysicalEquationOfStateBoussinesq::initEquationOfStateParam()
    {
        //设置参数
        if (!_equationOfStatePara) return;
        _equationOfStatePara->createDoubleParameter("rho0", 1.2);
        _equationOfStatePara->createDoubleParameter("T0", 300.0);
        _equationOfStatePara->createDoubleParameter("beta", 0.003);
    }

    FITKOFThermoPhysicalEquationOfStateIncompressiblePerfectGas::FITKOFThermoPhysicalEquationOfStateIncompressiblePerfectGas()
    {
        //初始化
        this->setDataObjectName("Incompressible Perfect Gas");
        this->initEquationOfStateParam();
    }

    FITKOFThermoPhysicalEquationOfStateIncompressiblePerfectGas::~FITKOFThermoPhysicalEquationOfStateIncompressiblePerfectGas()
    {
    }

    void FITKOFThermoPhysicalEquationOfStateIncompressiblePerfectGas::initEquationOfStateParam()
    {
        //设置参数
        if (!_equationOfStatePara) return;
        _equationOfStatePara->createDoubleParameter("pRef", 100000);
    }

    FITKOFThermoPhysicalEquationOfStateConstantDensity::FITKOFThermoPhysicalEquationOfStateConstantDensity()
    {
        //初始化
        this->setDataObjectName("Constant Density");
        this->initEquationOfStateParam();
    }

    FITKOFThermoPhysicalEquationOfStateConstantDensity::~FITKOFThermoPhysicalEquationOfStateConstantDensity()
    {
    }

    void FITKOFThermoPhysicalEquationOfStateConstantDensity::initEquationOfStateParam()
    {
        //设置参数
        if (!_equationOfStatePara) return;
        _equationOfStatePara->createDoubleParameter("rho", 1.17);
    }

    FITKOFThermoPhysicalEquationOfStatePerfectFluid::FITKOFThermoPhysicalEquationOfStatePerfectFluid()
    {
        //初始化
        this->setDataObjectName("Perfect Fluid");
        this->initEquationOfStateParam();
    }

    FITKOFThermoPhysicalEquationOfStatePerfectFluid::~FITKOFThermoPhysicalEquationOfStatePerfectFluid()
    {
    }

    void FITKOFThermoPhysicalEquationOfStatePerfectFluid::initEquationOfStateParam()
    {
        //设置参数
        if (!_equationOfStatePara) return;
        _equationOfStatePara->createDoubleParameter("R", 3000.0);
        _equationOfStatePara->createDoubleParameter("rho0", 1027.0);
    }

    FITKOFThermoPhysicalEquationOfStateAdiabaticPerfectFluid::FITKOFThermoPhysicalEquationOfStateAdiabaticPerfectFluid()
    {
        //初始化
        this->setDataObjectName("Adiabatic Perfect Fluid");
        this->initEquationOfStateParam();
    }

    FITKOFThermoPhysicalEquationOfStateAdiabaticPerfectFluid::~FITKOFThermoPhysicalEquationOfStateAdiabaticPerfectFluid()
    {
    }

    void FITKOFThermoPhysicalEquationOfStateAdiabaticPerfectFluid::initEquationOfStateParam()
    {
        //设置参数
        if (!_equationOfStatePara) return;
        _equationOfStatePara->createDoubleParameter("p0", 0.0);
        _equationOfStatePara->createDoubleParameter("rho0", 1000.0);
        _equationOfStatePara->createDoubleParameter("gamma", 1.0);
        _equationOfStatePara->createDoubleParameter("B", 0.0);
    }

    FITKOFThermoPhysicalEquationOfStatePengRobinson::FITKOFThermoPhysicalEquationOfStatePengRobinson()
    {
        //初始化
        this->setDataObjectName("Peng-Robinson");
        this->initEquationOfStateParam();
    }

    FITKOFThermoPhysicalEquationOfStatePengRobinson::~FITKOFThermoPhysicalEquationOfStatePengRobinson()
    {
    }

    void FITKOFThermoPhysicalEquationOfStatePengRobinson::initEquationOfStateParam()
    {
        //设置参数
        if (!_equationOfStatePara) return;
        _equationOfStatePara->createDoubleParameter("Tc", 1.0);
        _equationOfStatePara->createDoubleParameter("Pc", 1.0);
        _equationOfStatePara->createDoubleParameter("Vc", 1.0);
        _equationOfStatePara->createDoubleParameter("omega", 1.0);
    }

}
