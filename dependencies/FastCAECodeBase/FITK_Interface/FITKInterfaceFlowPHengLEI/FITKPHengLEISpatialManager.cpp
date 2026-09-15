/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEISpatialManager.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

#include "FITKPHengLEISpatialStruct.h"
#include "FITKPHengLEISpatialUnStruct.h"


namespace Interface
{
    FITKPHengLEISpatialManager::FITKPHengLEISpatialManager()
    {
        this->initspatialTypes();
    }

    FITKPHengLEISpatialManager::~FITKPHengLEISpatialManager()
    {

    }

	QStringList FITKPHengLEISpatialManager::filterSpatial()
	{
		QStringList SpatialType = { "Struct","UnStruct" };
		return SpatialType;
	}

	FITKPHengLEISpatialInfo FITKPHengLEISpatialManager::getSpatialTypeInfoByName(QString inFLowTypeName)
	{

		for (QList<FITKPHengLEISpatialInfo>::iterator iter = _spatialTypes.begin(); iter != _spatialTypes.end(); ++iter)
		{
			if (iter->_spatialTypeName == inFLowTypeName)
				return *iter;
		}
		return FITKPHengLEISpatialInfo();
	}

	void FITKPHengLEISpatialManager::initspatialTypes()
    {
		//注册UnStruct
		FITKPHengLEISpatialInfo infoUnStruct;
		infoUnStruct._spatialTypeName = "UnStruct";
		infoUnStruct._createFuns = [&](Interface::FITKPHengLEIAbsSolver::PHengLEISolverType type) {return new FITKPHengLEISpatialUnStruct(type); };
		_spatialTypes.append(infoUnStruct);

        //注册NStruct
		FITKPHengLEISpatialInfo infoStruct;
		infoStruct._spatialTypeName = "Struct";
		infoStruct._createFuns = [&](Interface::FITKPHengLEIAbsSolver::PHengLEISolverType type) {return new FITKPHengLEISpatialStruct(type); };
		_spatialTypes.append(infoStruct);


    }
}




