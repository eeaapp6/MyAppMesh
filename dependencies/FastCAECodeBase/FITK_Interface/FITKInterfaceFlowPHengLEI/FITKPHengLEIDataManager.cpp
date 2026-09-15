/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIDataManager.h"
#include "FITKPHengLEISolverManager.h"
#include "FITKPHengLEIInFlowTypeManager.h"
#include "FITKPHengLEISpatialManager.h"
#include "FITKPHengLEIBoundaryTypeManager.h"
#include "FITKPHengLEIViscousTypeManager.h"

namespace Interface
{
    FITKPHengLEIDataManager* FITKPHengLEIDataManager::_instance = nullptr;
    QMutex FITKPHengLEIDataManager::m_mutex;


    void FITKPHengLEIDataManager::finalize()
    {
		// 释放内存
        if (m_solverManager) delete m_solverManager;
		if (m_inFLowManager) delete m_inFLowManager;
		if (m_boundaryTypeManager) delete m_boundaryTypeManager;
		if (m_spatialManager) delete m_spatialManager;
		if (m_viscousTypeManager) delete m_viscousTypeManager;
			 
    }

	FITKPHengLEISolverManager * FITKPHengLEIDataManager::getSolverManager()
	{
		return m_solverManager;
	}

	FITKPHengLEIInFlowTypeManager * FITKPHengLEIDataManager::getInFlowManager()
	{
		return m_inFLowManager;
	}

	FITKPHengLEISpatialManager * FITKPHengLEIDataManager::getSpatialManager()
	{
		return m_spatialManager;
	}

	FITKPHengLEIBoundaryTypeManager * FITKPHengLEIDataManager::getBoundaryTypeManager()
	{
		return m_boundaryTypeManager;
	}

	FITKPHengLEIViscousTypeManager * FITKPHengLEIDataManager::getViscousTypeManager()
	{
		return m_viscousTypeManager;
	}

	void FITKPHengLEIDataManager::initialize()
    {
		// 创建管理器实例
        m_solverManager = new FITKPHengLEISolverManager;
		m_inFLowManager = new FITKPHengLEIInFlowTypeManager;
		m_boundaryTypeManager = new FITKPHengLEIBoundaryTypeManager;
		m_spatialManager = new FITKPHengLEISpatialManager;
		m_viscousTypeManager = new FITKPHengLEIViscousTypeManager;

    }

    FITKPHengLEIDataManagerDeleator::~FITKPHengLEIDataManagerDeleator()
    {
        if (FITKPHengLEIDataManager::_instance)
            delete FITKPHengLEIDataManager::_instance;
        FITKPHengLEIDataManager::_instance = nullptr;
    }
    static FITKPHengLEIDataManagerDeleator FPHengLEIPHMAGDELEATOR_;

}
