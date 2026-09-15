/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEISpatialUnStruct.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKPHengLEISpatialUnStruct::FITKPHengLEISpatialUnStruct(FITKPHengLEIAbsSolver::PHengLEISolverType type)
		:FITKPHengLEIAbsSpatialDiscretisation(type)
    {
        //初始化
        this->init();
        this->setDataObjectName("UnStruct");
    }


    void FITKPHengLEISpatialUnStruct::initSchemeTypeParam()
    {
		if (_solverType == FITKPHengLEIAbsSolver::PLHypersonic)
		{
			QStringList schemeType = { "vanleer","roe","steger","kfvs","lax_f","hlle","ausm+","ausmdv","ausm+w","auspw","ausmpwplus" };
			_schemeTypeParams->createComboxParameter("inviscid Scheme", schemeType, 0);
			QStringList limiterType = { "vencat","barth","nolim" };
			_schemeTypeParams->createComboxParameter("limiter", limiterType, 0);
			_schemeTypeParams->createDoubleParameter("venkatCoeff", 0.1);

			QStringList roeEntropyFixMethod = { "direct fix","Harten type" };
			_schemeTypeParams->createComboxParameter("Roe Entropy FixMethod", roeEntropyFixMethod, 1);

			_schemeTypeParams->createDoubleParameter("Roe Entropy Scale", 1);
		}

		if (_solverType == FITKPHengLEIAbsSolver::PLSubsonic)
		{
			QStringList schemeType = { "vanleer","steger","roe","kfvs","lax_f","hlle","ausm+","ausm+w","auspw","ausmpwplus" };
			_schemeTypeParams->createComboxParameter("inviscid Scheme", schemeType, 0);
			QStringList limiterType = { "vencat","1st","nolim" };
			_schemeTypeParams->createComboxParameter("limiter", limiterType, 0);
			_schemeTypeParams->createDoubleParameter("venkatCoeff", 0.5);

			QStringList roeEntropyFixMethod = { "direct fix","Harten type" };
			_schemeTypeParams->createComboxParameter("Roe Entropy FixMethod", roeEntropyFixMethod, 1);

			_schemeTypeParams->createDoubleParameter("Roe Entropy Scale", 1);
		}

		if (_solverType == FITKPHengLEIAbsSolver::PLSupersonic)
		{
			QStringList schemeType = { "vanleer","steger","roe","kfvs","lax_f","hlle","ausm+","ausmdv","ausm+w","auspw","ausmpwplus" };
			_schemeTypeParams->createComboxParameter("inviscid Scheme", schemeType, 0);
			QStringList limiterType = { "vencat","barth","nolim" };
			_schemeTypeParams->createComboxParameter("limiter", limiterType, 0);
			_schemeTypeParams->createDoubleParameter("venkatCoeff", 0.5);

			QStringList roeEntropyFixMethod = { "direct fix","Harten type" };
			_schemeTypeParams->createComboxParameter("Roe Entropy FixMethod", roeEntropyFixMethod, 1);

			_schemeTypeParams->createDoubleParameter("Roe Entropy Scale", 1);
		}

		if (_solverType == FITKPHengLEIAbsSolver::PLTransonic)
		{
			QStringList schemeType = { "vanleer","roe","steger","kfvs","lax_f","hlle","ausm+","ausmdv","ausm+w","auspw","ausmpwplus" };
			_schemeTypeParams->createComboxParameter("inviscid Scheme", schemeType, 0);
			QStringList limiterType = { "vencat","barth","1st","nolim" };
			_schemeTypeParams->createComboxParameter("limiter", limiterType, 0);
			_schemeTypeParams->createDoubleParameter("venkatCoeff", 0.5);

			QStringList roeEntropyFixMethod = { "direct fix","Harten type" };
			_schemeTypeParams->createComboxParameter("Roe Entropy FixMethod", roeEntropyFixMethod, 1);

			_schemeTypeParams->createDoubleParameter("Roe Entropy Scale", 1);
		}


    }
}



