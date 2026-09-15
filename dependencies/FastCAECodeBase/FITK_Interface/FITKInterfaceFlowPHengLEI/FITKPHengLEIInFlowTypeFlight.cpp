/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIInFlowTypeFlight.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKPHengLEIInFlowTypeFlight::FITKPHengLEIInFlowTypeFlight(FITKPHengLEIAbsSolver::PHengLEISolverType type)
		:FITKPHengLEIAbstractInFlowType(type)
    {
        //初始化
        this->init();
        this->setDataObjectName("Flight");
    }


    void FITKPHengLEIInFlowTypeFlight::initInFlowTypeParam()
    {
		if (_solverType == Interface::FITKPHengLEIAbsSolver::PLSubsonic ||
			_solverType == Interface::FITKPHengLEIAbsSolver::PLTransonic ||
			_solverType == Interface::FITKPHengLEIAbsSolver::PLHypersonic ||
			_solverType == Interface::FITKPHengLEIAbsSolver::PLSupersonic)
		{
			//设置参数
			_inFlowTypeParams->createDoubleParameter("height", 0.001);
		}
    }
}



