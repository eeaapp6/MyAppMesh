/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFBoundaryType.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBoolGroup.h"

namespace Interface
{

    FITKOFAbsBoundaryType::FITKOFAbsBoundaryType()
    {
        _boundaryTypeParams = new Core::FITKParameter;
    }

    FITKOFAbsBoundaryType::~FITKOFAbsBoundaryType()
    {
        if (_boundaryTypeParams) delete _boundaryTypeParams;
    }

    Core::FITKParameter* FITKOFAbsBoundaryType::getBoundaryTypePara()
    {
        return _boundaryTypeParams;
    }

    void FITKOFAbsBoundaryType::initBoundaryTypeParam()
    {
    }

    FITKOFBTySymmetry::FITKOFBTySymmetry()
    {
        this->setDataObjectName("Symmetry");
        this->initBoundaryTypeParam();
    }
    FITKOFBTySymmetry::~FITKOFBTySymmetry()
    {
    }
    void FITKOFBTySymmetry::initBoundaryTypeParam()
    {
    }

    FITKOFBTyWedge::FITKOFBTyWedge()
    {
        this->setDataObjectName("Wedge");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyWedge::~FITKOFBTyWedge()
    {
    }
    void FITKOFBTyWedge::initBoundaryTypeParam()
    {
    }

    FITKOFBTyZeroGradient::FITKOFBTyZeroGradient()
    {
        this->setDataObjectName("Zero Gradient");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyZeroGradient::~FITKOFBTyZeroGradient()
    {
    }
    void FITKOFBTyZeroGradient::initBoundaryTypeParam()
    {
    }

    FITKOFBTyFixedFluxPressure::FITKOFBTyFixedFluxPressure()
    {
        this->setDataObjectName("Fixed Flux Pressure");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyFixedFluxPressure::~FITKOFBTyFixedFluxPressure()
    {
    }
    void FITKOFBTyFixedFluxPressure::initBoundaryTypeParam()
    {
    }

    FITKOFBTyTKEFixedValue::FITKOFBTyTKEFixedValue()
    {
        this->setDataObjectName("Fixed Value");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyTKEFixedValue::~FITKOFBTyTKEFixedValue()
    {
    }
    void FITKOFBTyTKEFixedValue::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("Value [m2/s2]", 0);
    }

    FITKOFBTyVFixedValue::FITKOFBTyVFixedValue()
    {
        this->setDataObjectName("Fixed Value");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyVFixedValue::~FITKOFBTyVFixedValue()
    {
    }
    void FITKOFBTyVFixedValue::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleListParameter("Value [m/s]", QList<double>() << 0.0 << 0.0 << 0.0);
    }

    FITKOFBTyFixedValue::FITKOFBTyFixedValue()
    {
        this->setDataObjectName("Fixed Value");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyFixedValue::~FITKOFBTyFixedValue()
    {
    }
    void FITKOFBTyFixedValue::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("Value [-]", 0.0);
    }

    FITKOFBTyPFixedValue::FITKOFBTyPFixedValue()
    {
        this->setDataObjectName("Fixed Value");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyPFixedValue::~FITKOFBTyPFixedValue()
    {
    }
    void FITKOFBTyPFixedValue::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("Value [Pa]", 0.0);
    }

    FITKOFBTyStandardWallFunction::FITKOFBTyStandardWallFunction()
    {
        this->setDataObjectName("Standard Wall Function");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyStandardWallFunction::~FITKOFBTyStandardWallFunction()
    {
    }
    void FITKOFBTyStandardWallFunction::initBoundaryTypeParam()
    {
    }

    FITKOFBTyVLowReWallFunction::FITKOFBTyVLowReWallFunction()
    {
        this->setDataObjectName("Low Re Wall Function");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyVLowReWallFunction::~FITKOFBTyVLowReWallFunction()
    {
    }
    void FITKOFBTyVLowReWallFunction::initBoundaryTypeParam()
    {
    }

    FITKOFBTyNoSlip::FITKOFBTyNoSlip()
    {
        this->setDataObjectName("No-Slip");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyNoSlip::~FITKOFBTyNoSlip()
    {
    }
    void FITKOFBTyNoSlip::initBoundaryTypeParam()
    {
    }

    FITKOFBTyTurbulentIntensityInlet::FITKOFBTyTurbulentIntensityInlet()
    {
        this->setDataObjectName("Turbulent Intensity Inlet");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyTurbulentIntensityInlet::~FITKOFBTyTurbulentIntensityInlet()
    {
    }
    void FITKOFBTyTurbulentIntensityInlet::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("Intensity [-]", 0.05);
    }

    FITKOFBTyTurbulentMixingLengthInlet::FITKOFBTyTurbulentMixingLengthInlet()
    {
        this->setDataObjectName("Turbulent Mixing Length Inlet");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyTurbulentMixingLengthInlet::~FITKOFBTyTurbulentMixingLengthInlet()
    {
    }
    void FITKOFBTyTurbulentMixingLengthInlet::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("Mixing Length [m]", 1e-03);
    }

    FITKOFBTyTKETotalPressure::FITKOFBTyTKETotalPressure()
    {
        this->setDataObjectName("Total Pressure");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyTKETotalPressure::~FITKOFBTyTKETotalPressure()
    {
    }
    void FITKOFBTyTKETotalPressure::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("p0 [m2/s2]", 0);
    }

    FITKOFBTyPTotalPressure::FITKOFBTyPTotalPressure()
    {
        this->setDataObjectName("Total Pressure");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyPTotalPressure::~FITKOFBTyPTotalPressure()
    {
    }
    void FITKOFBTyPTotalPressure::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("p0 [Pa]", 0);
    }

    FITKOFBTyPressureIOVelocity::FITKOFBTyPressureIOVelocity()
    {
        this->setDataObjectName("Pressure Inlet-Outlet Velocity");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyPressureIOVelocity::~FITKOFBTyPressureIOVelocity()
    {
    }
    void FITKOFBTyPressureIOVelocity::initBoundaryTypeParam()
    {
    }

    FITKOFBTySurNorFixedValue::FITKOFBTySurNorFixedValue()
    {
        this->setDataObjectName("Surface Normal Fixed Value");
        this->initBoundaryTypeParam();
    }
    FITKOFBTySurNorFixedValue::~FITKOFBTySurNorFixedValue()
    {
    }
    void FITKOFBTySurNorFixedValue::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("Reference Value [m/s]", 0);
    }

    FITKOFBTyInletOutlet::FITKOFBTyInletOutlet()
    {
        this->setDataObjectName("Inlet-Outlet");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyInletOutlet::~FITKOFBTyInletOutlet()
    {
    }
    void FITKOFBTyInletOutlet::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("Inlet Value [-]", 0);
    }

    FITKOFBTyOutletPhaseMeanVelocity::FITKOFBTyOutletPhaseMeanVelocity()
    {
        this->setDataObjectName("Outlet Phase Mean Velocity");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyOutletPhaseMeanVelocity::~FITKOFBTyOutletPhaseMeanVelocity()
    {
    }
    void FITKOFBTyOutletPhaseMeanVelocity::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("Umean [m/s]", 0);
    }

    FITKOFBTyVariableHeight::FITKOFBTyVariableHeight()
    {
        this->setDataObjectName("Variable Height");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyVariableHeight::~FITKOFBTyVariableHeight()
    {
    }
    void FITKOFBTyVariableHeight::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("Lower Bound [-]", 0);
        _boundaryTypeParams->createDoubleParameter("Upper Bound [-]", 0.9);
    }

    FITKOFBTyCoupledTemperatureAndRadiation::FITKOFBTyCoupledTemperatureAndRadiation()
    {
        this->setDataObjectName("Coupled Temperature and Radiation");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyCoupledTemperatureAndRadiation::~FITKOFBTyCoupledTemperatureAndRadiation()
    {
    }
    void FITKOFBTyCoupledTemperatureAndRadiation::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createBoolGroupParameter("Resistance", false);
        _boundaryTypeParams->createDoubleParameter("thicknessLayers", 1);
        _boundaryTypeParams->createDoubleParameter("kappaLayers", 1);
    }

    FITKOFBTyEmpty::FITKOFBTyEmpty()
    {
        this->setDataObjectName("Empty");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyEmpty::~FITKOFBTyEmpty()
    {
    }
    void FITKOFBTyEmpty::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
    }

    FITKOFBTyTotalTemperature::FITKOFBTyTotalTemperature()
    {
        this->setDataObjectName("Total Temperature");
        this->initBoundaryTypeParam();
    }
    FITKOFBTyTotalTemperature::~FITKOFBTyTotalTemperature()
    {
    }
    void FITKOFBTyTotalTemperature::initBoundaryTypeParam()
    {
        if (!_boundaryTypeParams)return;
        _boundaryTypeParams->createDoubleParameter("T0[K]", 300);
    }

}

