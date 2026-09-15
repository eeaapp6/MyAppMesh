/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIAbsSpatialDiscretisation.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITKPHengLEIAbsSolver.h"

namespace Interface
{
    FITKPHengLEIAbsSpatialDiscretisation::FITKPHengLEIAbsSpatialDiscretisation(FITKPHengLEIAbsSolver::PHengLEISolverType type)
		:_solverType(type)
    {
        //初始化
		_schemeTypeParams = new Core::FITKParameter;
    }

    FITKPHengLEIAbsSpatialDiscretisation::~FITKPHengLEIAbsSpatialDiscretisation()
    {
        //销毁数据
        if (_schemeTypeParams) delete _schemeTypeParams;
    }

    Core::FITKParameter* FITKPHengLEIAbsSpatialDiscretisation::getSchemeTypeParams()
    {
        //获取空间离散类型参数
        return _schemeTypeParams;
    }

    void FITKPHengLEIAbsSpatialDiscretisation::init()
    {
        //初始化参数
        this->initSchemeTypeParam();
    }

    void FITKPHengLEIAbsSpatialDiscretisation::initSchemeTypeParam()
    {
        //初始化空间离散类型参数
    }
}



