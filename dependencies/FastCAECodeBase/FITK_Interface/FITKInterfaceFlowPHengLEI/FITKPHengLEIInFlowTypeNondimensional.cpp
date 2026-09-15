/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIInFlowTypeNondimensional.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITKPHengLEIAbsSolver.h"

namespace Interface
{
    FITKPHengLEIInFlowTypeNondimensional::FITKPHengLEIInFlowTypeNondimensional(FITKPHengLEIAbsSolver::PHengLEISolverType type)
		:FITKPHengLEIAbstractInFlowType(type)
    {
        //初始化
        this->init();
        this->setDataObjectName("Nondimensional");
    }


    void FITKPHengLEIInFlowTypeNondimensional::initInFlowTypeParam()
    {
		if (_solverType == Interface::FITKPHengLEIAbsSolver::PLHypersonic)
		{
			_inFlowTypeParams->createDoubleParameter("RefReNumber", 4.0e6);
			_inFlowTypeParams->createDoubleParameter("Dimensional Temperature", 46.3);
			_inFlowTypeParams->createDoubleParameter("Vibration Temperature", 10000.0);
		}

		if (_solverType == Interface::FITKPHengLEIAbsSolver::PLSubsonic)
		{
			_inFlowTypeParams->createDoubleParameter("RefReNumber", 6.5e6);
			_inFlowTypeParams->createDoubleParameter("Dimensional Temperature", 288.15);
		}

		if (_solverType == Interface::FITKPHengLEIAbsSolver::PLSupersonic)
		{
			_inFlowTypeParams->createDoubleParameter("RefReNumber", 1.0e5);
			_inFlowTypeParams->createDoubleParameter("Dimensional Temperature", 288.15);
		}

		if (_solverType == Interface::FITKPHengLEIAbsSolver::PLTransonic)
		{
			// 所有这些类型都需要设置 Temperature
			_inFlowTypeParams->createDoubleParameter("RefReNumber", 6.5e6);
			_inFlowTypeParams->createDoubleParameter("RefDimensionalTemperaure", 288.15);
			// 仅当类型为 PLHypersonic 时，增加 Vibration Temperature
		}

    }
}



