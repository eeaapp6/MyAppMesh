/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIViscousTypeManager.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITKPHengLEIViscousType.h"

namespace Interface
{
	FITKPHengLEIViscousTypeManager::FITKPHengLEIViscousTypeManager()
    {
        this->initViscousTypes();
    }

	FITKPHengLEIViscousTypeManager::~FITKPHengLEIViscousTypeManager()
    {

    }

	QStringList FITKPHengLEIViscousTypeManager::filterTurbence(FITKPHengLEIAbsSolver::PHengLEISolverType solverType, FITKPHengLEIAbsSolver::PHengLEIViscousType viscousType)
	{
		QStringList filteredTurbulenceModels;


		for (const FITKPHengLEIViscousTypeInfo& viscousInfo : _viscousTypes)
		{
			bool supportsSolverType = viscousInfo._enableType.contains(solverType);
			
			bool supportsViscousType = viscousInfo._viscousType.contains(viscousType);
			
			if (supportsSolverType && supportsViscousType)
			{
				filteredTurbulenceModels.append(viscousInfo._viscousTypeName);
			}
		}

		return filteredTurbulenceModels;
	}


	FITKPHengLEIViscousTypeInfo FITKPHengLEIViscousTypeManager::getViscousTypeInfoByName(QString inFLowTypeName)
	{

		for (QList<FITKPHengLEIViscousTypeInfo>::iterator iter = _viscousTypes.begin(); iter != _viscousTypes.end(); ++iter)
		{
			if (iter->_viscousTypeName == inFLowTypeName)
				return *iter;
		}
		return FITKPHengLEIViscousTypeInfo();
	}

	void FITKPHengLEIViscousTypeManager::initViscousTypes()
    {
        //注册Turbulent1eq
		FITKPHengLEIViscousTypeInfo turbulent1;
		turbulent1._viscousTypeName = "Turbulent1eq";
		turbulent1._enableType = { FITKPHengLEIAbsSolver::PLHypersonic,FITKPHengLEIAbsSolver::PLSubsonic,FITKPHengLEIAbsSolver::PLSupersonic,FITKPHengLEIAbsSolver::PLTransonic};
		turbulent1._viscousType = { FITKPHengLEIAbsSolver::RANS,FITKPHengLEIAbsSolver::DES,FITKPHengLEIAbsSolver::DDES,FITKPHengLEIAbsSolver::IDDES };
		turbulent1._createFuns = [&]{return new Turbulent1eq(); };
		_viscousTypes.append(turbulent1);

        //注册Turbulent2eq
		FITKPHengLEIViscousTypeInfo turbulent2;
		turbulent2._viscousTypeName = "Turbulent2eq";
		turbulent2._enableType = { FITKPHengLEIAbsSolver::PLHypersonic,FITKPHengLEIAbsSolver::PLSubsonic,FITKPHengLEIAbsSolver::PLSupersonic,FITKPHengLEIAbsSolver::PLTransonic };
		turbulent2._viscousType = { FITKPHengLEIAbsSolver::RANS,FITKPHengLEIAbsSolver::DES,FITKPHengLEIAbsSolver::DDES,FITKPHengLEIAbsSolver::IDDES };
		turbulent2._createFuns = [&]{return new Turbulent2eq(); }; 
        _viscousTypes.append(turbulent2);

		//注册k-epsilon
		FITKPHengLEIViscousTypeInfo KEpsilon;
		KEpsilon._viscousTypeName = "KEpsilon";
		KEpsilon._enableType = { FITKPHengLEIAbsSolver::PLCompressibleSIMPLE,FITKPHengLEIAbsSolver::PLSIMPLE,FITKPHengLEIAbsSolver::PLPISO };
		KEpsilon._viscousType = { FITKPHengLEIAbsSolver::RANS,FITKPHengLEIAbsSolver::DES,FITKPHengLEIAbsSolver::DDES,FITKPHengLEIAbsSolver::IDDES };
		KEpsilon._createFuns = [&]{return new Interface::KEpsilon(); };
		_viscousTypes.append(KEpsilon);

		//注册SpalartAllmaras
		FITKPHengLEIViscousTypeInfo SpalartAllmaras;
		SpalartAllmaras._viscousTypeName = "SpalartAllmaras";
		SpalartAllmaras._enableType = { FITKPHengLEIAbsSolver::PLCompressibleSIMPLE,FITKPHengLEIAbsSolver::PLSIMPLE,FITKPHengLEIAbsSolver::PLPISO };
		SpalartAllmaras._viscousType = { FITKPHengLEIAbsSolver::RANS,FITKPHengLEIAbsSolver::DES,FITKPHengLEIAbsSolver::DDES,FITKPHengLEIAbsSolver::IDDES };
		SpalartAllmaras._createFuns = [&]{return new Interface::SpalartAllmaras(); };
		_viscousTypes.append(SpalartAllmaras);
    }
}




