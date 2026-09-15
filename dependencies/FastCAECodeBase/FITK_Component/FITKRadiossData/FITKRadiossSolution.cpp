/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossSolution.h"
#include "FITKAbstractBCS.h"
#include "FITKAbstractRWall.h"
#include "FITKAbstractInitialField.h"
#include "FITKAbstractInteraction.h"
#include "FITKGravity.h"
#include "FITKRadiossSolverSettings.h"
#include "FITKRadiossResultRequest.h"
#include "FITKAbstractProbe.h"
#include "FITKAbstractTimeHistory.h"
#include "FITKAbstractConnection.h"

namespace Radioss
{
    FITKRadiossSolution::FITKRadiossSolution()
    {
        _bcsManager = new FITKRadiossBCSManager();
        _initialFieldManager = new FITKInitialFieldManager();
        _defaultInteractionManager = new FITKRadiossInteractionDefaultManager();
        _rWallManager = new FITKRadiossRWallManager();
        _gravityManager = new FITKGravityManager();
        _interactionManager = new FITKInteractionManager();
        _solverSettings = new FITKRadiossSolverSettings();
        _resultRequest = new FITKRadiossResultRequest();
        _probeManager = new FITKProbeManager();
        _timeHietoryManager = new FITKTimeHistoryManager();
        _connectionManager = new FITKConnectionManager();
    }
    FITKRadiossSolution::~FITKRadiossSolution()
    {
        if (_bcsManager) { delete _bcsManager; _bcsManager = nullptr; }
        if (_initialFieldManager) { delete _initialFieldManager; _initialFieldManager = nullptr; }
        if (_defaultInteractionManager) { delete _defaultInteractionManager; _defaultInteractionManager = nullptr; }
        if (_rWallManager) { delete _rWallManager; _rWallManager = nullptr; }
        if (_gravityManager) { delete _gravityManager; _gravityManager = nullptr; }
        if (_interactionManager) { delete _interactionManager; _interactionManager = nullptr; }
        if (_solverSettings) { delete _solverSettings; _solverSettings = nullptr; }
        if (_resultRequest) { delete _resultRequest; _resultRequest = nullptr; }
        if (_probeManager) { delete _probeManager; _probeManager = nullptr; }
        if (_timeHietoryManager) { delete _timeHietoryManager; _timeHietoryManager = nullptr; }
        if (_connectionManager) { delete _connectionManager; _connectionManager = nullptr; }
    }
    FITKRadiossBCSManager* FITKRadiossSolution::getBCSManager() const
    {
        return _bcsManager;
    }
    FITKInitialFieldManager* FITKRadiossSolution::getInitialFieldManager() const
    {
        return _initialFieldManager;
    }
    FITKRadiossInteractionDefaultManager* FITKRadiossSolution::getDefaultInteractionManager() const
    {
        return _defaultInteractionManager;
    }
    FITKRadiossRWallManager* FITKRadiossSolution::getRWallManager() const
    {
        return _rWallManager;
    }
    FITKGravityManager* FITKRadiossSolution::getGravityManager() const
    {
        return _gravityManager;
    }
    FITKInteractionManager *FITKRadiossSolution::getInteractionManager() const
    {
        return _interactionManager;
    }
    FITKProbeManager * FITKRadiossSolution::getProbeManager() const
    {
        return _probeManager;
    }
    FITKRadiossSolverSettings *FITKRadiossSolution::getSolverSettings() const
    {
        return _solverSettings;
    }
    FITKRadiossResultRequest* FITKRadiossSolution::getResultRequest() const
    {
        return _resultRequest;
    }

    FITKTimeHistoryManager * FITKRadiossSolution::getTimeHietoryManager() const
    {
        return _timeHietoryManager;
    }

    FITKConnectionManager * FITKRadiossSolution::getConnectionManager() const
    {
        return _connectionManager;
    }

    FITKRadiossSolution *FITKRadiossSolutionManager::getCurrentSolution()
    {
        // 首先根据当前ID获取
        FITKRadiossSolution* sol = this->getDataByID(_currentSolutionID);
        if (sol != nullptr) return sol;
        // 其次获取第一个不是空的求解对象
        const int n = this->getDataCount();
        for (int i = 0;i<n; ++i)
        {
            sol = this->getDataByIndex(i);
            if(sol == nullptr) continue;
            _currentSolutionID = sol->getDataObjectID();
            return sol;
        }
        // 最后创建一个新的求解对象
        sol = new FITKRadiossSolution();
        sol->setDataObjectName(this->checkName("Solution-1"));
        this->appendDataObj(sol);
        _currentSolutionID = sol->getDataObjectID();
        return sol;
    }

    void FITKRadiossSolutionManager::setCurrentSolution(int id)
    {
        _currentSolutionID = id;
    }
    int FITKRadiossSolutionManager::getCurrentSolutionID() const
    {
        return _currentSolutionID;
    }
}
