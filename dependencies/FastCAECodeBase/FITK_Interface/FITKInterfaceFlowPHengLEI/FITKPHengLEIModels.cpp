/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIModels.h"
#include "FITKPHengLEIEnergyModel.h"
#include "FITKPHengLEISpeciesModel.h"
#include "FITKPHengLEIViscousModel.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
	FITKPHengLEIModels::FITKPHengLEIModels()
	{
	}

	FITKPHengLEIModels::~FITKPHengLEIModels()
	{
		if (_energyModel) delete _energyModel;
		if (_speciesModel) delete _speciesModel;
		if (_viscousModel) delete _viscousModel;
	}
	FITKPHengLEIEnergyModel * FITKPHengLEIModels::getEnergyModel()
	{
		return _energyModel;
	}
	void FITKPHengLEIModels::setEnergyModel(FITKPHengLEIEnergyModel * model)
	{
		if (_energyModel != model && _energyModel) delete _energyModel;
		_energyModel = model;
	}
	FITKPHengLEISpeciesModel * FITKPHengLEIModels::getSpeciesModel()
	{
		return _speciesModel;
	}
	void FITKPHengLEIModels::setSpeciesModel(FITKPHengLEISpeciesModel * model)
	{
		if (_speciesModel != model && _speciesModel) delete _speciesModel;
		_speciesModel = model;
	}
	FITKPHengLEIViscousModel* FITKPHengLEIModels::FITKPHengLEIModels::getViscousModel()
	{
		return _viscousModel;
	}
	void FITKPHengLEIModels::setViscousModel(FITKPHengLEIViscousModel* model)
	{
		if (_viscousModel != model && _viscousModel) delete _viscousModel;
		_viscousModel = model;
	}
}
