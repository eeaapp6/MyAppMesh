/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIInFlowTypeDensity.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKPHengLEIInFlowTypeDensity::FITKPHengLEIInFlowTypeDensity(FITKPHengLEIAbsSolver::PHengLEISolverType type)
		:FITKPHengLEIAbstractInFlowType(type)
    {
        //初始化
        this->init();
        this->setDataObjectName("Density");
    }


	void FITKPHengLEIInFlowTypeDensity::initInFlowTypeParam()
	{
		if (_solverType == Interface::FITKPHengLEIAbsSolver::PLHypersonic)
		{
			//设置参数
			_inFlowTypeParams->createDoubleParameter("Velocity", 1000.0);
			_inFlowTypeParams->createDoubleParameter("Density", 1.0e3);
		}
	}
}



