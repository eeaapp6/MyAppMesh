/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIInFlowTypeManager.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

#include "FITKPHengLEIInFlowTypeNondimensional.h"
#include "FITKPHengLEIInFlowTypeFlight.h"
#include "FITKPHengLEIInFlowTypeExperiment.h"
#include "FITKPHengLEIInFlowTypePressure.h"
#include "FITKPHengLEIInFlowTypeDensity.h"


namespace Interface
{
    FITKPHengLEIInFlowTypeManager::FITKPHengLEIInFlowTypeManager()
    {
        this->initInFlowTypes();
    }

    FITKPHengLEIInFlowTypeManager::~FITKPHengLEIInFlowTypeManager()
    {

    }

	QStringList FITKPHengLEIInFlowTypeManager::filterTurbence(FITKPHengLEIAbsSolver::PHengLEISolverType type)
	{
		QStringList listType;
		//求解器遍历
		for (QList<FITKPHengLEIInFlowTypeInfo>::iterator iter = _inFlowTypes.begin(); iter != _inFlowTypes.end(); ++iter)
		{
			//进行过滤
			if (iter->_enableType.contains(type))
				listType.append(iter->_InFlowTypeName);
		}
		return listType;
	}


	FITKPHengLEIInFlowTypeInfo FITKPHengLEIInFlowTypeManager::getInFlowTypeInfoByName(QString inFLowTypeName)
	{

		for (QList<FITKPHengLEIInFlowTypeInfo>::iterator iter = _inFlowTypes.begin(); iter != _inFlowTypes.end(); ++iter)
		{
			if (iter->_InFlowTypeName == inFLowTypeName)
				return *iter;
		}
		return FITKPHengLEIInFlowTypeInfo();
	}

	void FITKPHengLEIInFlowTypeManager::initInFlowTypes()
    {
        //注册Nondimensional
		FITKPHengLEIInFlowTypeInfo infoNondimensional;
		infoNondimensional._InFlowTypeName = "Nondimensional";
		infoNondimensional._enableType = { FITKPHengLEIAbsSolver::PLHypersonic,FITKPHengLEIAbsSolver::PLSubsonic,FITKPHengLEIAbsSolver::PLSupersonic,FITKPHengLEIAbsSolver::PLTransonic};
		infoNondimensional._createFuns = [&](Interface::FITKPHengLEIAbsSolver::PHengLEISolverType type) {return new FITKPHengLEIInFlowTypeNondimensional(type); };
		_inFlowTypes.append(infoNondimensional);

        //注册Flight
		FITKPHengLEIInFlowTypeInfo infoFlight;
		infoFlight._InFlowTypeName = "Flight";
		infoFlight._enableType = { FITKPHengLEIAbsSolver::PLHypersonic,FITKPHengLEIAbsSolver::PLSubsonic,FITKPHengLEIAbsSolver::PLSupersonic,FITKPHengLEIAbsSolver::PLTransonic };
		infoFlight._createFuns = [&](Interface::FITKPHengLEIAbsSolver::PHengLEISolverType type) {return new FITKPHengLEIInFlowTypeFlight(type); };
        _inFlowTypes.append(infoFlight);

        //注册experiment
		FITKPHengLEIInFlowTypeInfo infoExperiment;
		infoExperiment._InFlowTypeName = "Experiment";
		infoExperiment._enableType = {};
		infoExperiment._createFuns = [&](Interface::FITKPHengLEIAbsSolver::PHengLEISolverType type) {return new FITKPHengLEIInFlowTypeExperiment(type); };
        _inFlowTypes.append(infoExperiment);

        //注册Pressure
		FITKPHengLEIInFlowTypeInfo infoPressure;
		infoPressure._InFlowTypeName = "Pressure";
		infoPressure._enableType = { FITKPHengLEIAbsSolver::PLHypersonic };
		infoPressure._createFuns = [&](Interface::FITKPHengLEIAbsSolver::PHengLEISolverType type) {return new FITKPHengLEIInFlowTypePressure(type); };
		_inFlowTypes.append(infoPressure);

        //注册Density
		FITKPHengLEIInFlowTypeInfo infoDensity;
		infoDensity._InFlowTypeName = "Density";
		infoDensity._enableType = { FITKPHengLEIAbsSolver::PLHypersonic };
		infoDensity._createFuns = [&](Interface::FITKPHengLEIAbsSolver::PHengLEISolverType type) {return new FITKPHengLEIInFlowTypeDensity(type); };
        _inFlowTypes.append(infoDensity);
    }
}




