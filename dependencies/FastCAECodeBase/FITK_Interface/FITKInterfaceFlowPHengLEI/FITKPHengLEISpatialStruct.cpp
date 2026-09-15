/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEISpatialStruct.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITKPHengLEIAbsSolver.h"

namespace Interface
{
    FITKPHengLEISpatialStruct::FITKPHengLEISpatialStruct(FITKPHengLEIAbsSolver::PHengLEISolverType type)
		:FITKPHengLEIAbsSpatialDiscretisation(type)
    {
        //初始化
        this->init();
        this->setDataObjectName("Struct");
    }


    void FITKPHengLEISpatialStruct::initSchemeTypeParam()
    {
		if (_solverType == FITKPHengLEIAbsSolver::PLHypersonic)
		{
			QStringList inviscidType = { "vanleer","steger","auspw","ausmpw+" };
			_schemeTypeParams->createComboxParameter("inviscid Scheme", inviscidType, 0);
			QStringList limiterType = { "minmod","3rd_minmod_smooth","vanalbada" };
			_schemeTypeParams->createComboxParameter("limiter", limiterType, 0);

			QStringList roeEntropyFixMethod = { "direct fix","multi-dimensional fix","Harten type" };
			_schemeTypeParams->createComboxParameter("Roe Entropy FixMethod", roeEntropyFixMethod, 2);

			_schemeTypeParams->createDoubleParameter("Roe Entropy Scale", 1);
		}
		if (_solverType == FITKPHengLEIAbsSolver::PLSubsonic)
		{
			QStringList inviscidType = { "vanleer","steger","auspw","ausmpw+" };
			_schemeTypeParams->createComboxParameter("inviscid Scheme", inviscidType, 0);
			QStringList limiterType = { "minmod","smooth","3rdsmooth","3rd_minmod_smooth","nolim" };
			_schemeTypeParams->createComboxParameter("limiter", limiterType, 0);

			QStringList roeEntropyFixMethod = { "direct fix","multi-dimensional fix","Harten type" };
			_schemeTypeParams->createComboxParameter("Roe Entropy FixMethod", roeEntropyFixMethod, 2);

			_schemeTypeParams->createDoubleParameter("Roe Entropy Scale", 1);
		}
		if (_solverType == FITKPHengLEIAbsSolver::PLSupersonic)
		{
			QStringList inviscidType = { "vanleer","steger","auspw","ausmpw+","roe" };
			_schemeTypeParams->createComboxParameter("inviscid Scheme", inviscidType, 0);
			QStringList limiterType = { "minmod","3rd_minmod_smooth","minvan"};
			_schemeTypeParams->createComboxParameter("limiter", limiterType, 0);

			QStringList roeEntropyFixMethod = { "direct fix","multi-dimensional fix","Harten type" };
			_schemeTypeParams->createComboxParameter("Roe Entropy FixMethod", roeEntropyFixMethod, 2);

			_schemeTypeParams->createDoubleParameter("Roe Entropy Scale", 1);
		}
		if (_solverType == FITKPHengLEIAbsSolver::PLTransonic)
		{
			QStringList inviscidType = { "roe","vanleer","ausm+up","ausmpw" };
			_schemeTypeParams->createComboxParameter("inviscid Scheme", inviscidType, 0);
			QStringList limiterType = { "3rdsmooth","smooth","nolim" };
			_schemeTypeParams->createComboxParameter("limiter", limiterType, 0);

			QStringList roeEntropyFixMethod = { "direct fix","multi-dimensional fix","Harten type" };
			_schemeTypeParams->createComboxParameter("Roe Entropy FixMethod", roeEntropyFixMethod, 2);

			_schemeTypeParams->createDoubleParameter("Roe Entropy Scale", 1);
		}
    }
}



