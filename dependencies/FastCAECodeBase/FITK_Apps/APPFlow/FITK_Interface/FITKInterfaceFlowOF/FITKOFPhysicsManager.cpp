/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFPhysicsManager.h"
#include "FITKOFSolverManager.h"
#include "FITKOFTurbulenceManager.h"
#include "FITKOFSolutionSolverManager.h"
#include "FITKOFTransportModelManager.h"
#include "FITKOFBoundaryTypeManager.h"
#include "FITKOFRadiationParaManager.h"
#include "FITKOFThermoPropManager.h"

namespace Interface
{
    FITKOFPhysicsManager* FITKOFPhysicsManager::_instance = nullptr;
    QMutex FITKOFPhysicsManager::m_mutex;

    FITKOFSolverManager* FITKOFPhysicsManager::getSolverManager()
    {
        return m_solverManager;
    }

    FITKOFTurbulenceManager* FITKOFPhysicsManager::getTurbulenceManager()
    {
        return m_turbulenceManager;
    }

    FITKOFSolutionSolverManager* FITKOFPhysicsManager::getSolutionSolverManager()
    {
        return m_solutionSolverManager;
    }

    FITKOFTransportModelManager* FITKOFPhysicsManager::getTransportModelManager()
    {
        return m_transportModelManager;
    }

    FITKOFBoundaryTypeManager* FITKOFPhysicsManager::getBoundaryTypeManager()
    {
        return m_boundaryTypeManager;
    }

    FITKOFRadiationParaManager* FITKOFPhysicsManager::getRadiationParaManager()
    {
        return m_radiationParaManager;
    }

    FITKOFThermoPropManager * FITKOFPhysicsManager::getThermoPropManager()
    {
        return m_thermoPropManager;
    }

    void FITKOFPhysicsManager::finalize()
    {
        if (m_solverManager) delete m_solverManager;
        if (m_turbulenceManager)delete m_turbulenceManager;
        if (m_solutionSolverManager) delete m_solutionSolverManager;
        if (m_transportModelManager) delete m_transportModelManager;
        if (m_boundaryTypeManager) delete m_boundaryTypeManager;
        if (m_radiationParaManager) delete m_radiationParaManager;
        if (m_thermoPropManager) delete m_thermoPropManager;
    }

    void FITKOFPhysicsManager::initialize()
    {
        m_solverManager = new FITKOFSolverManager;
        m_turbulenceManager = new FITKOFTurbulenceManager;
        m_solutionSolverManager = new FITKOFSolutionSolverManager;
        m_transportModelManager = new FITKOFTransportModelManager;
        m_boundaryTypeManager = new FITKOFBoundaryTypeManager;
        m_radiationParaManager = new FITKOFRadiationParaManager;
        m_thermoPropManager = new FITKOFThermoPropManager;
    }

    FITKOFPhysicsManagerDeleator::~FITKOFPhysicsManagerDeleator()
    {
        if (FITKOFPhysicsManager::_instance)
            delete FITKOFPhysicsManager::_instance;
        FITKOFPhysicsManager::_instance = nullptr;
    }
    static FITKOFPhysicsManagerDeleator FOFPHMAGDELEATOR_;

}
