/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIAbstractInFlowType.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITKPHengLEIAbsSolver.h"

namespace Interface
{
    FITKPHengLEIAbstractInFlowType::FITKPHengLEIAbstractInFlowType(FITKPHengLEIAbsSolver::PHengLEISolverType type)
		:_solverType(type)
	{
        //初始化
		_inFlowTypeParams = new Core::FITKParameter;
    }

    FITKPHengLEIAbstractInFlowType::~FITKPHengLEIAbstractInFlowType()
    {
        //销毁数据
        if (_inFlowTypeParams) delete _inFlowTypeParams;
    }

    Core::FITKParameter* FITKPHengLEIAbstractInFlowType::getInFlowTypeParams()
    {
        //获取湍流模型参数
        return _inFlowTypeParams;
    }


    void FITKPHengLEIAbstractInFlowType::init()
    {
        //初始化
        this->initInFlowTypeParam();
    }

    void FITKPHengLEIAbstractInFlowType::initInFlowTypeParam()
    {
        //初始化来流类型参数
    }

}



