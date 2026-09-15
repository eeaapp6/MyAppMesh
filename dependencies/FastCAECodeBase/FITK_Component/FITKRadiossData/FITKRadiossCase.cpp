/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossCase.h"
#include "FITKRadiossMeshModel.h"
#include "FITKRadiossNodesParts.h"
#include "FITKPropAbstract.hpp"
#include "FITKNonLinerCurve.h"
#include "FITKRadiossSolution.h"
#include "FITKAbstractInteraction.h"
#include "FITKAbstractEquationOfState.h"
#include "FITKAbstractFailureModel.h"
#include "FITKRadiossGeomPart.h"
#include "FITKAbstractBEM.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"

namespace Radioss
{
    FITKRadiossCase::FITKRadiossCase()
        : Core::FITKAbstractNDataObject()
    {
        _geomPartManager = new FITKRadiossGeomPartManager();
        _defaultPropValueManager = new FITKRadiossPropValueManager();
        _sectionManager = new Interface::FITKAbstractSectionManager();
        _materialManager = new Interface::FITKMaterialManager();
        _meshModel = new FITKRadiossMeshModel();
        _nonLinerCurveManager = new FITKNonLinerCurveManager();
        _solutionManager = new FITKRadiossSolutionManager();
        _equOfStateManager = new FITKEquationOfStateManager();
        _failureModelManager = new FITKRadiossFailureModelManager();
        _bemManager = new FITKRadiossBEMManager();
        FITKRadiossSolution* solution = new FITKRadiossSolution;
        solution->setDataObjectName("Solution-1");
        _solutionManager->appendDataObj(solution);
 
    }
    FITKRadiossCase::~FITKRadiossCase()
    {
        if (_geomPartManager) { delete _geomPartManager; _geomPartManager = nullptr; }
        if (_defaultPropValueManager) { delete _defaultPropValueManager; _defaultPropValueManager = nullptr; }
        if (_sectionManager) { delete _sectionManager; _sectionManager = nullptr; }
        if (_materialManager) { delete _materialManager; _materialManager = nullptr; }
        if (_meshModel) { delete _meshModel; _meshModel = nullptr; }
        if (_nonLinerCurveManager) { delete _nonLinerCurveManager; _nonLinerCurveManager = nullptr; }
        if (_solutionManager) { delete _solutionManager; _solutionManager = nullptr; }
        if (_equOfStateManager) { delete _equOfStateManager; _equOfStateManager = nullptr; }
        if (_failureModelManager) { delete _failureModelManager; _failureModelManager = nullptr; }
        if (_bemManager) { delete _bemManager; _bemManager = nullptr; }
    }
    FITKRadiossGeomPartManager *FITKRadiossCase::getGeomPartManager() const
    {
        return _geomPartManager;
    }
    Interface::FITKAbstractSectionManager *FITKRadiossCase::getSectionManager() const
    {
        return _sectionManager;
    }
    Interface::FITKMaterialManager* FITKRadiossCase::getMaterialManager() const
    {
        return _materialManager;
    }

    FITKEquationOfStateManager * FITKRadiossCase::getEquationOfState() const
    {
        return _equOfStateManager;
    }

    FITKRadiossMeshModel *FITKRadiossCase::getMeshModel() const
    {
        return _meshModel;
    }

    FITKRadiossFailureModelManager * FITKRadiossCase::getFailureModelManager() const
    {
        return _failureModelManager;
    }

    FITKRadiossBCSManager *FITKRadiossCase::getBCSManager() 
    {
        FITKRadiossSolution* sol = getCurrentSolution();
        if(sol)
            return sol->getBCSManager();
        return nullptr;
    }

    FITKNonLinerCurveManager *FITKRadiossCase::getNonLinerCurveManager() const
    {
        return _nonLinerCurveManager;
    }

    FITKRadiossPropValueManager *FITKRadiossCase::getDefaultPropValueManager() const
    {
        return _defaultPropValueManager;
    }

    FITKRadiossInteractionDefaultManager * FITKRadiossCase::getDefaultInteractionManager()
    {
        FITKRadiossSolution* sol = getCurrentSolution();
        if (sol)
            return sol->getDefaultInteractionManager();
        return nullptr;
    }

    FITKInitialFieldManager *FITKRadiossCase::getInitialFieldManager()
    {
        FITKRadiossSolution* sol = getCurrentSolution();
        if(sol)
            return sol->getInitialFieldManager();
        return nullptr;
    }

    FITKRadiossRWallManager *FITKRadiossCase::getRWallManager()
    {
        FITKRadiossSolution* sol = getCurrentSolution();
        if(sol)
            return sol->getRWallManager();
        return nullptr;
    }

    FITKGravityManager *FITKRadiossCase::getGravityManager()
    {
        FITKRadiossSolution* sol = getCurrentSolution();
        if(sol)
            return sol->getGravityManager();
        return nullptr;
    }

    FITKConnectionManager * FITKRadiossCase::getConnectionManager()
    {
        FITKRadiossSolution* sol = getCurrentSolution();
        if (sol)
            return sol->getConnectionManager();
        return nullptr;
    }

    FITKInteractionManager *FITKRadiossCase::getInteractionManager()
    {
        FITKRadiossSolution* sol = getCurrentSolution();
        if(sol)
            return sol->getInteractionManager();
        return nullptr;
    }

    FITKProbeManager * FITKRadiossCase::getProbeManager()
    {
        FITKRadiossSolution* sol = getCurrentSolution();
        if (sol)
            return sol->getProbeManager();
        return nullptr;
    }

    FITKRadiossBEMManager *FITKRadiossCase::getBEMManager()
    {
        return _bemManager;
    }

    FITKRadiossSolverSettings * FITKRadiossCase::getSolverSettings()
    {
        FITKRadiossSolution* sol = getCurrentSolution();
        if(sol)
            return sol->getSolverSettings();
        return nullptr;
    }

    FITKRadiossResultRequest * FITKRadiossCase::getResultRequest()
    {
        FITKRadiossSolution* sol = getCurrentSolution();
        if(sol)
            return sol->getResultRequest();
        return nullptr;
    }

    FITKRadiossSolutionManager *FITKRadiossCase::getSolutionManager() const
    {
        return _solutionManager;
    }
    FITKRadiossSolution* FITKRadiossCase::getCurrentSolution()
    {
        if(_solutionManager)
        {
            return _solutionManager->getCurrentSolution();
        }
        return nullptr;
    }

    void FITKRadiossCase::clearData()
    {
        if (_geomPartManager) _geomPartManager->clear();
        if (_sectionManager) _sectionManager->clear();
        if (_materialManager) _materialManager->clear();
        if (_meshModel) _meshModel->clearData();
        if (_nonLinerCurveManager) _nonLinerCurveManager->clear();
        if (_solutionManager) _solutionManager->clear();
        if (_equOfStateManager) _equOfStateManager->clear();
        if (_failureModelManager) _failureModelManager->clear();
        if (_bemManager) _bemManager->clear();
    }

} // namespace Radioss
