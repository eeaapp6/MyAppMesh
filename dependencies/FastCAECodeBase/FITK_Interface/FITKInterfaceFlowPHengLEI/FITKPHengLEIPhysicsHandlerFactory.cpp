/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIPhysicsHandlerFactory.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITKPHengLEIData.h"
#include "FITKPHengLEIAbsSolver.h"
#include "FITKPHengLEIDataManager.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKPHengLEISolverManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITKPHengLEIGeneralControl.h"
#include "FITKPHengLEIInFlow.h"
#include "FITKPHengLEIMeshPara.h"
#include "FITKPHengLEIViscousModel.h"
#include "FITKPHengLEIMultiGrid.h"
#include "FITKPHengLEIInFlowTypeManager.h"
#include "FITKPHengLEIAbstractInFlowType.h"
#include "FITKPHengLEIDiscretisation.h"
#include "FITKPHengLEISpatialManager.h"
#include "FITKPHengLEIAbsSpatialDiscretisation.h"
#include "FITKPHengLEIBoundary.h"
#include "FITKPHengLEIBoundaryType.h"
#include "FITKPHengLEIBoundaryTypeManager.h"
#include "FITKPHengLEIOutputSetting.h"
#include "FITKPHengLEIFlow.h"
#include "FITKPHengLEIViscousType.h"
#include "FITKPHengLEIViscousTypeManager.h"
#include "FITKPHengLEICalculation.h"
#include "FITKPHengLEISolution.h"
#include "FITKPHengLEIInitialCondition.h"
#include "FITKPHengLEIModels.h"
#include "FITKPHengLEIEnergyModel.h"
#include "FITKPHengLEISpeciesModel.h"
#include <QDebug>

namespace Interface
{
    QString FITKPhysicsHandlerFactory::getComponentName()
    {
        return "FITKPHengLEIPhysicsHandlerFactory";
    }

    void FITKPhysicsHandlerFactory::setSolver(QString solverName)
    {
        //获取物理数据与物理场管理器
		FITKPHengLEIData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
        FITKPHengLEIDataManager* physicsManager = FITKPHengLEIDataManager::getInstance();
        if (!physicsManager || !physicsData)return;
        //查看是否是当前求解器
        if (physicsData->getSolver() && physicsData->getSolver()->getDataObjectName() == solverName)return;
        //获取求解器管理器
        FITKPHengLEISolverManager* solverManager = physicsManager->getSolverManager();
        if (!solverManager)return;
        //通过求解器名称获取求解器信息
        FITKPHengLEISolverInfo solverInfo = solverManager->getSolverInfoBySolverName(solverName);
        //创建求解器
		FITKPHengLEIAbsSolver* solver = solverInfo._createFuns();
        this->actionSetSolver(solver);
    }
	void FITKPhysicsHandlerFactory::setInflowType(QString InFlowType,QString solverName)
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;

		FITKPHengLEIInFlow* inFlow = physicData->getInFlow();
		if (!inFlow) return;
		if (inFlow->getInFlowType() && inFlow->getInFlowType()->getDataObjectName() == InFlowType) return;

		FITKPHengLEIInFlowTypeManager* inFlowManager = dataManager->getInFlowManager();
		if (!inFlowManager) return;

		FITKPHengLEIInFlowTypeInfo info = inFlowManager->getInFlowTypeInfoByName(InFlowType);
		if (info._InFlowTypeName.isEmpty())return;
		FITKPHengLEIAbstractInFlowType* inFlowType = info._createFuns(physicData->getSolver()->getSolverType());
		inFlow->setInFlowType(inFlowType);
	}

	void FITKPhysicsHandlerFactory::setViscousType(QString ViscousType)
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;

		FITKPHengLEIModels* models = physicData->getModels();
		if (!models) return;
		Interface::FITKPHengLEIViscousModel* viscousModel = models->getViscousModel();
		if (viscousModel->getViscousType() && viscousModel->getViscousType()->getDataObjectName() == ViscousType) return;

		FITKPHengLEIViscousTypeManager* viscousManager = dataManager->getViscousTypeManager();
		if (!viscousManager) return;
		FITKPHengLEIViscousTypeInfo info = viscousManager->getViscousTypeInfoByName(ViscousType);
		if (info._viscousTypeName.isEmpty()) return;
		FITKPHengLEIViscousType* viscousType = info._createFuns();
		viscousModel->setViscousType(viscousType);
	}

	void FITKPhysicsHandlerFactory::setBoundary(int meshBoundaryId, FITKPHengLEIBoundary::PHengLEIBoundaryType bType)
	{
		//获取物理数据与物理场管理器
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;
		//获取物理场边界管理器
		FITKPHengLEIBoundaryManager* boundaryManagerPhy = physicData->getBoundaryManager();
		if (!boundaryManagerPhy)return;
		//获取边界类型管理器
		FITKPHengLEIBoundaryTypeManager* boundaryTypeManager = dataManager->getBoundaryTypeManager();
		if (!boundaryTypeManager)return;
		//获取求解器
		FITKPHengLEIAbsSolver* solver = physicData->getSolver();
		if (!solver)return;
		//获取求解器里的边界信息
		FITKPHengLEIBoundary* boundarySol = solver->getBoundary(bType);
		if (!boundarySol)return;
		//创建新边界并添加到物理场边界管理器
		FITKPHengLEIBoundary* boundaryPhy = new FITKPHengLEIBoundary(bType);
		boundaryManagerPhy->appendDataObj(boundaryPhy);
		boundaryPhy->setMeshBoundaryID(meshBoundaryId);

		FITKPHengLEIBoundaryType* absBoundType = boundarySol->getBoundaryType();

		FITKPHengLEIBoundaryTypeInfo info = boundaryTypeManager->getBoundaryType(absBoundType->getDataObjectName(), bType);

		if (info._BoundaryTypeTypeName.isEmpty()) return;
		FITKPHengLEIBoundaryType* boundTypeNew = info._createFuns();
		
		Core::FITKParameter* para = boundTypeNew->getBoundaryTypeParams();
		para->copyPara(absBoundType->getBoundaryTypeParams());

		boundaryPhy->setBoundaryType(boundTypeNew);
	}

	void FITKPhysicsHandlerFactory::setBoundaryType(int meshID, QString typeName, FITKPHengLEIBoundary::PHengLEIBoundaryType bType)
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;

		FITKPHengLEIBoundaryManager* boundaryManager = physicData->getBoundaryManager();
		if (!boundaryManager) return;
		Interface::FITKPHengLEIBoundary* boundary = boundaryManager->getBoundary(meshID);
		if (!boundary) return;

		if (boundary->getBoundaryType() && boundary->getBoundaryType()->getDataObjectName() == typeName) return;
		
		FITKPHengLEIBoundaryTypeManager* boundaryTypeManager = dataManager->getBoundaryTypeManager();
		if (!boundaryManager) return;
		FITKPHengLEIBoundaryTypeInfo info = boundaryTypeManager->getBoundaryType(typeName, bType);
		if (info._BoundaryTypeTypeName.isEmpty()) return;
		FITKPHengLEIBoundaryType* boundTypeNew = info._createFuns();
		boundary->setBoundaryType(boundTypeNew);
	}


    void FITKPhysicsHandlerFactory::actionSetSolver(FITKPHengLEIAbsSolver* solver)
    {
        //获取物理数据
		FITKPHengLEIData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
        if (!physicsData)return;
        //设置求解器
        physicsData->setSolver(solver);
        if (!solver)return;

		this->setGeneralControl();

		this->setInFlow();

		this->setMeshPara();

		this->setViscousModel();

		this->setMultiGrid();

		this->setDiscretisation();

		this->setOutputSetting();

		this->setCalculation();

		this->setSolution();

		this->setInitialCondition();

		this->setModels();

		physicsData->setBoundaryManager(new FITKPHengLEIBoundaryManager);

		// 设置flow参数
		this->setFlow();
    }

	void FITKPhysicsHandlerFactory::setGeneralControl()
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;
		
		//获取求解器
		FITKPHengLEIAbsSolver* solver = physicData->getSolver();
		if (!solver) return;

		// 获取求解器下的通用控制参数
		FITKPHengLEIGeneralControl* generalControlSolver = solver->getGeneralControl();
		if (!generalControlSolver) return;

		// 添加调试信息
		qDebug() << "About to create FITKPHengLEIGeneralControl";
		
		// 创建通用控制参数数据，初始化通用控制参数数据复制到实际数据中
		FITKPHengLEIGeneralControl* generalControl = new FITKPHengLEIGeneralControl;
		
		// 验证对象创建成功
		if (!generalControl) {
			qDebug() << "Failed to create FITKPHengLEIGeneralControl";
			return;
		}
		
		qDebug() << "Successfully created FITKPHengLEIGeneralControl";

		Core::FITKParameter* addgeneralControlSol = generalControlSolver->getGeneralControlAdditionalData();
		Core::FITKParameter* addgeneralControl = generalControl->getGeneralControlAdditionalData();
		if (addgeneralControl && addgeneralControlSol)
		{
			addgeneralControl->copyPara(addgeneralControlSol);
		}
		physicData->setGeneralControl(generalControl);
	}

	void FITKPhysicsHandlerFactory::setMeshPara()
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;
		//获取求解器
		FITKPHengLEIAbsSolver* solver = physicData->getSolver();
		if (!solver) return;

		// 获取求解器下的网格相关参数
		FITKPHengLEIMeshPara* meshParaSolver = solver->getMeshPara();
		if (!meshParaSolver) return;

		FITKPHengLEIMeshPara* meshPara = new FITKPHengLEIMeshPara;

		Core::FITKParameter* addMeshParaSol = meshParaSolver->getAdditionalMeshData();
		Core::FITKParameter* addMeshPara = meshPara->getAdditionalMeshData();
		if (addMeshPara && addMeshParaSol)
		{
			addMeshPara->copyPara(addMeshParaSol);
		}

		Core::FITKParameter* addForceReferenceSol = meshParaSolver->getAdditionalForceReference();
		Core::FITKParameter* addForceReference = meshPara->getAdditionalForceReference();
		if (addForceReferenceSol && addForceReference)
		{
			addForceReference->copyPara(addForceReferenceSol);
		}

		Core::FITKParameter* addReferencePointSol = meshParaSolver->getAdditionalReferencePoint();
		Core::FITKParameter* addReferencePoint = meshPara->getAdditionalReferencePoint();
		if (addReferencePointSol && addReferencePoint)
		{
			addReferencePoint->copyPara(addReferencePointSol);
		}

		physicData->setMeshPara(meshPara);
	}
	void FITKPhysicsHandlerFactory::setViscousModel()
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;
		//获取求解器
		FITKPHengLEIAbsSolver* solver = physicData->getSolver();
		if (!solver) return;

		//获取求解器下的粘性模型参数
		FITKPHengLEIViscousModel* viscousModelSolver = solver->getViscousModel();
		if (!viscousModelSolver) return;
		FITKPHengLEIViscousType* viscousTypeSol = viscousModelSolver->getViscousType();

		FITKPHengLEIViscousModel* viscousModel = new FITKPHengLEIViscousModel;

		Core::FITKParameter* addviscousModelSol = viscousModelSolver->getAdditionalVicousModelData();
		
		Core::FITKParameter* addviscousModel = viscousModel->getAdditionalVicousModelData();

		if (addviscousModelSol && addviscousModel)
		{
			addviscousModel->copyPara(addviscousModelSol);
		}


		//获取粘性模型类型管理器
		FITKPHengLEIViscousTypeManager* viscousTypeManager = dataManager->getViscousTypeManager();
		if (!viscousTypeManager) return;
		if (!viscousModelSolver->getViscousType()) return;

		FITKPHengLEIViscousTypeInfo info = viscousTypeManager->getViscousTypeInfoByName(viscousModelSolver->getViscousType()->getDataObjectName());

		if (info._viscousTypeName.isEmpty())return;
		FITKPHengLEIViscousType* viscousType = info._createFuns();

		Core::FITKParameter* viscousTypeSolPara = viscousTypeSol->getViscousTypeParams();
		Core::FITKParameter* viscousTypePara = viscousType->getViscousTypeParams();
		if (viscousTypeSol&& viscousTypePara)
		{
			viscousTypePara->copyPara(viscousTypeSolPara);
		}

		viscousModel->setViscousType(viscousType);

		physicData->setViscousModel(viscousModel);


	}
	void FITKPhysicsHandlerFactory::setMultiGrid()
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;
		//获取求解器
		FITKPHengLEIAbsSolver* solver = physicData->getSolver();
		if (!solver) return;

		FITKPHengLEIMultiGrid* MultiGridSolver = solver->getMultiGrid();
		if (!MultiGridSolver)
		{
			physicData->setMultiGrid(nullptr);
			return;
		}
		FITKPHengLEIMultiGrid* MultiGrid = new FITKPHengLEIMultiGrid;
		Core::FITKParameter* addMultiGridSol = MultiGridSolver->getAdditionalMultiGridData();
		Core::FITKParameter* addMultiGrid = MultiGrid->getAdditionalMultiGridData();

		if (addMultiGridSol&& addMultiGrid)
		{
			addMultiGrid->copyPara(addMultiGridSol);
		}
		physicData->setMultiGrid(MultiGrid);
	}
	void FITKPhysicsHandlerFactory::setInFlow()
	{
		// 获取物理数据与物理场管理器
		FITKPHengLEIData* data = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!data || !dataManager)return;
		// 获取求解器
		FITKPHengLEIAbsSolver* solver = data->getSolver();
		if (!solver) return;
		// 获取求解器下的来流参数
		FITKPHengLEIInFlow* inflowsol = solver->getInFlow();
		if (!inflowsol) return;

		FITKPHengLEIAbstractInFlowType* inFlowTypeSol = inflowsol->getInFlowType();
		if (!inFlowTypeSol) return;

		//获取来流类型管理器
		FITKPHengLEIInFlowTypeManager* inFlowTypeManager = dataManager->getInFlowManager();
		if (!inFlowTypeManager) return;

		FITKPHengLEIInFlowTypeInfo info = inFlowTypeManager->getInFlowTypeInfoByName(inFlowTypeSol->getDataObjectName());

		if (info._InFlowTypeName.isEmpty())return;
		FITKPHengLEIAbstractInFlowType* inFlowType = info._createFuns(data->getSolver()->getSolverType());

		Core::FITKParameter* addInFlowTypeSol =  inFlowTypeSol->getInFlowTypeParams();
		Core::FITKParameter* addInFLow = inFlowType->getInFlowTypeParams();
		if (addInFlowTypeSol&& addInFLow)
		{
			addInFLow->copyPara(addInFlowTypeSol);
		}

		FITKPHengLEIInFlow* inFLow = new FITKPHengLEIInFlow;

		inFLow->setInFlowType(inFlowType);

		Core::FITKParameter* addDataSol = inflowsol->getInFlowAdditionalData();
		Core::FITKParameter* addData = inFLow->getInFlowAdditionalData();
		if (addData && addDataSol)
		{
			addData->copyPara(addDataSol);
		}
		data->setInFlow(inFLow);
	}
	void FITKPhysicsHandlerFactory::setDiscretisation()
	{
		// 获取物理数据与物理场管理器
		FITKPHengLEIData* data = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!data || !dataManager)return;
		// 获取求解器
		FITKPHengLEIAbsSolver* solver = data->getSolver();
		if (!solver) return;
		// 获取求解器下的离散参数
		FITKPHengLEIDiscretisation* discretisationsol = solver->getDiscretisation();
		if (!discretisationsol) return;

		FITKPHengLEIDiscretisation* discretisation = new FITKPHengLEIDiscretisation;

		FITKPHengLEIAbsSpatialDiscretisation* spatialDisationSol = discretisationsol->getSpatialDiscretisation();
		if (spatialDisationSol)
		{
			//获取空间离散类型管理器
			FITKPHengLEISpatialManager* spatialManager = dataManager->getSpatialManager();
			if (!spatialManager) return;

			QString name = discretisationsol->getDataObjectName();

			FITKPHengLEISpatialInfo info = spatialManager->getSpatialTypeInfoByName(spatialDisationSol->getDataObjectName());


			if (info._spatialTypeName.isEmpty())return;
			FITKPHengLEIAbsSpatialDiscretisation* spatialType = info._createFuns(data->getSolver()->getSolverType());

			Core::FITKParameter* addSpatialTypeSol = spatialDisationSol->getSchemeTypeParams();
			Core::FITKParameter* addSpatial = spatialType->getSchemeTypeParams();
			if (addSpatialTypeSol && addSpatial)
			{
				addSpatial->copyPara(addSpatialTypeSol);
			}

			discretisation->setSpatialDiscretisation(spatialType);
		}

		// 获取流动离散
		Core::FITKParameter* addFlowDataSol = discretisationsol->getFlowDiscretisation();
		Core::FITKParameter* addFlowData = discretisation->getFlowDiscretisation();
		if (addFlowDataSol && addFlowData)
		{
			addFlowData->copyPara(addFlowDataSol);
		}

		Core::FITKParameter* addDataSol = discretisationsol->getTemporalAdditionalData();
		Core::FITKParameter* addData = discretisation->getTemporalAdditionalData();
		if (addData && addDataSol)
		{
			addData->copyPara(addDataSol);
		}
		data->setDiscretisation(discretisation);
	}

	void FITKPhysicsHandlerFactory::setOutputSetting()
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;
		//获取求解器
		FITKPHengLEIAbsSolver* solver = physicData->getSolver();
		if (!solver) return;

		// 获取求解器下的风雷输出参数
		FITKPHengLEIOutputSetting* outputSettingSolver = solver->getOutputSetting();
		if (!outputSettingSolver) return;

		// 创建通用控制参数数据，初始化通用控制参数数据复制到实际数据中
		FITKPHengLEIOutputSetting* outputSetting = new FITKPHengLEIOutputSetting;

		Core::FITKParameter* addbasicFlowSol = outputSettingSolver->getBasicFlowData();
		Core::FITKParameter* addbasicFlow = outputSetting->getBasicFlowData();
		if (addbasicFlowSol && addbasicFlow)
		{
			addbasicFlow->copyPara(addbasicFlowSol);
		}

		Core::FITKParameter* addViscositySol = outputSettingSolver->getViscosityData();
		Core::FITKParameter* addViscosity = outputSetting->getViscosityData();
		if (addViscositySol && addViscosity)
		{
			addViscosity->copyPara(addViscositySol);
		}

		Core::FITKParameter* addVorticitySol = outputSettingSolver->getVorticityData();
		Core::FITKParameter* addVorticity = outputSetting->getVorticityData();
		if (addVorticitySol && addVorticity)
		{
			addVorticity->copyPara(addVorticitySol);
		}

		Core::FITKParameter* addDerivedFlowSol = outputSettingSolver->getDerivedFlowData();
		Core::FITKParameter* addDerivedFlow = outputSetting->getDerivedFlowData();
		if (addDerivedFlowSol && addDerivedFlow)
		{
			addDerivedFlow->copyPara(addDerivedFlowSol);                                                                                                                                                                                                                                                                                                                                                                                                                                                   
		}

		Core::FITKParameter* addTurbulenceModelSol = outputSettingSolver->getTurbulenceModelData();
		Core::FITKParameter* addTurbulenceModel = outputSetting->getTurbulenceModelData();
		if (addTurbulenceModelSol && addTurbulenceModel)
		{
			addTurbulenceModel->copyPara(addTurbulenceModelSol);
		}

		Core::FITKParameter* addNonEquilibriumSol = outputSettingSolver->getNonEquilibriumData();
		Core::FITKParameter* addNonEquilibrium = outputSetting->getNonEquilibriumData();
		if (addNonEquilibriumSol && addNonEquilibrium)
		{
			addNonEquilibrium->copyPara(addNonEquilibriumSol);
		}

		Core::FITKParameter* addDimensionalSol = outputSettingSolver->getDimensionalData();
		Core::FITKParameter* addDimensional = outputSetting->getDimensionalData();
		if (addDimensionalSol && addDimensional)
		{
			addDimensional->copyPara(addDimensionalSol);
		}

		Core::FITKParameter* addDimensionlessSol = outputSettingSolver->getDimensionlessData();
		Core::FITKParameter* addDimensionless = outputSetting->getDimensionlessData();
		if (addDimensionlessSol && addDimensionless)
		{
			addDimensionless->copyPara(addDimensionlessSol);
		}
		physicData->setOutputSetting(outputSetting);
	}
	void FITKPhysicsHandlerFactory::setPHengLEIDiscretisationType(QString SpatialType, QString sovlerName)
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;

		FITKPHengLEIDiscretisation* discretisation = physicData->getDiscretisation();
		if (!discretisation) return;
		if (discretisation->getSpatialDiscretisation() && discretisation->getSpatialDiscretisation()->getDataObjectName() == SpatialType) return;

		FITKPHengLEISpatialManager* spatialManager = dataManager->getSpatialManager();
		if (!spatialManager) return;

		FITKPHengLEISpatialInfo info = spatialManager->getSpatialTypeInfoByName(SpatialType);
		if (info._spatialTypeName.isEmpty())return;
		FITKPHengLEIAbsSpatialDiscretisation* spatialType = info._createFuns(physicData->getSolver()->getSolverType());
		discretisation->setSpatialDiscretisation(spatialType);
	}

	void FITKPhysicsHandlerFactory::setFlow()
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;
		//获取求解器
		FITKPHengLEIAbsSolver* solver = physicData->getSolver();
		if (!solver) return;
		//获取参数
		FITKPHengLEIFlow* FlowSolver = solver->getFlow();
		if (!FlowSolver)
		{
			physicData->setFlow(nullptr);
			return;
		}

		FITKPHengLEIFlow* Flow = new FITKPHengLEIFlow;

		Core::FITKParameter* addSolverNamesol = FlowSolver->getAdditionalSolverName();
		Core::FITKParameter* addSolverName = Flow->getAdditionalSolverName();
		if (addSolverNamesol&& addSolverName)
		{
			addSolverName->copyPara(addSolverNamesol);
		}

		Core::FITKParameter* addInitialValuesol = FlowSolver->getAdditionalInitialValue();
		Core::FITKParameter* addInitialValue = Flow->getAdditionalInitialValue();
		if (addInitialValue&& addInitialValuesol)
		{
			addInitialValue->copyPara(addInitialValuesol);
		}

		Core::FITKParameter* addMomentumEquationsol = FlowSolver->getAdditionalMomentumEquation();
		Core::FITKParameter* addMomentumEquation = Flow->getAdditionalMomentumEquation();
		if (addMomentumEquationsol&& addMomentumEquation)
		{
			addMomentumEquation->copyPara(addMomentumEquationsol);
		}

		Core::FITKParameter* addPressurePossionEquationsol = FlowSolver->getAdditionalPressurePossionEquation();
		Core::FITKParameter* addPressurePossionEquation = Flow->getAdditionalPressurePossionEquation();
		if (addPressurePossionEquationsol&& addPressurePossionEquation)
		{
			addPressurePossionEquation->copyPara(addPressurePossionEquationsol);
		}
;
		Core::FITKParameter* addSurfaceFluxsol = FlowSolver->getAdditionalSurfaceFlux();
		Core::FITKParameter* addSurfaceFlux = Flow->getAdditionalSurfaceFlux();
		if (addSurfaceFluxsol&& addSurfaceFlux)
		{
			addSurfaceFlux->copyPara(addSurfaceFluxsol);
		}

		physicData->setFlow(Flow);
	}
	void FITKPhysicsHandlerFactory::setCalculation()
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;
		//获取求解器
		FITKPHengLEIAbsSolver* solver = physicData->getSolver();
		if (!solver) return;

		// 获取求解器下的网格相关参数
		FITKPHengLEICalculation* calculationParaSolver = solver->getCalculation();
		if (!calculationParaSolver) return;

		FITKPHengLEICalculation* calculationPara = new FITKPHengLEICalculation;

		Core::FITKParameter* addConvergenceParaSol = calculationParaSolver->getAdditionalConvergenceData();
		Core::FITKParameter* addConvergencePara = calculationPara->getAdditionalConvergenceData();
		if (addConvergencePara && addConvergenceParaSol)
		{
			addConvergencePara->copyPara(addConvergenceParaSol);
		}

		Core::FITKParameter* addCalculationSol = calculationParaSolver->getAdditionalSavingIntervalsData();
		Core::FITKParameter* addCalculation = calculationPara->getAdditionalSavingIntervalsData();
		if (addCalculationSol && addCalculation)
		{
			addCalculation->copyPara(addCalculationSol);
		}

		Core::FITKParameter* addParallelSol = calculationParaSolver->getAdditionalParallelData();
		Core::FITKParameter* addParallel = calculationPara->getAdditionalParallelData();
		if (addParallelSol && addParallel)
		{
			addParallel->copyPara(addParallelSol);
		}

		Core::FITKParameter* addSolverControlSol = calculationParaSolver->getAdditionalSolverControlData();
		Core::FITKParameter * addSolverControl = calculationPara->getAdditionalSolverControlData();
		if (addSolverControl && addSolverControlSol)
		{
			addSolverControl->copyPara(addSolverControlSol);
		}


		physicData->setCalculation(calculationPara);
	}

	void FITKPhysicsHandlerFactory::setSolution()
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;
		//获取求解器
		FITKPHengLEIAbsSolver* solver = physicData->getSolver();
		if (!solver) return;

		FITKPHengLEISolution* solSolution = solver->getSolution();
		if (!solSolution) return;

		// 复制MomentEquation数据
		FITKPHengLEISolution* solution = new FITKPHengLEISolution;
		Core::FITKParameter* addMomentumData = solution->getAdditionalMomentumEquationData();
		Core::FITKParameter* addMomentDataSol = solSolution->getAdditionalMomentumEquationData();
		if (addMomentDataSol && addMomentumData)
		{
			addMomentumData->copyPara(addMomentDataSol);
		}

		// 复制PressureEquation数据
		Core::FITKParameter* addPressureData = solution->getAdditionalPressurePossionEquation();
		Core::FITKParameter* addPressureDataSol = solSolution->getAdditionalPressurePossionEquation();
		if (addPressureDataSol && addPressureData)
		{
			addPressureData->copyPara(addPressureDataSol);
		}

		// 复制TurbulenceEquation数据
		Core::FITKParameter* addTurbulenceData = solution->getAdditionalTurbulenceEquationData();
		Core::FITKParameter* addTurbulenceDataSol = solSolution->getAdditionalTurbulenceEquationData();
		if (addTurbulenceDataSol && addTurbulenceData)
		{
			addTurbulenceData->copyPara(addTurbulenceDataSol);
		}

		// 复制SpeciesEquation数据
		Core::FITKParameter* addSpeciesData = solution->getAdditionalSpeciesEquationData();
		Core::FITKParameter* addSpeciesDataSol = solSolution->getAdditionalSpeciesEquationData();
		if (addSpeciesDataSol && addSpeciesData)
		{
			addSpeciesData->copyPara(addSpeciesDataSol);
		}

		// 复制EnergyEquation数据
		Core::FITKParameter* addEnergyData = solution->getAdditionalEnergyEquationData();
		Core::FITKParameter* addEnergyDataSol = solSolution->getAdditionalEnergyEquationData();
		if (addEnergyDataSol && addEnergyData)
		{
			addEnergyData->copyPara(addEnergyDataSol);
		}

		// 复制Relaxation数据
		Core::FITKParameter* addRelaxationData = solution->getAdditionalRelaxationData();
		Core::FITKParameter* addRelaxationDataSol = solSolution->getAdditionalRelaxationData();
		if (addRelaxationDataSol && addRelaxationData)
		{
			addRelaxationData->copyPara(addRelaxationDataSol);
		}
		
		physicData->setSolution(solution);
	}

	void FITKPhysicsHandlerFactory::setInitialCondition()
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;
		//获取求解器
		FITKPHengLEIAbsSolver* solver = physicData->getSolver();
		if (!solver) return;

		FITKPHengLEIInitialCondition* solInitialCondition = solver->getInitialCondition();
		if (!solInitialCondition) return;

		// 复制
		FITKPHengLEIInitialCondition* initialCondition = new FITKPHengLEIInitialCondition;
		Core::FITKParameter* addData = initialCondition->getInitialConditionAdditionalData();
		Core::FITKParameter* addDataSol = solInitialCondition->getInitialConditionAdditionalData();
		if (addDataSol && addData)
		{
			addData->copyPara(addDataSol);
		}

		physicData->setInitialCondition(initialCondition);
	}

	void FITKPhysicsHandlerFactory::setModels()
	{
		FITKPHengLEIData* physicData = FITKAPP->getGlobalData()->getPhysicsData<FITKPHengLEIData>();
		FITKPHengLEIDataManager* dataManager = FITKPHengLEIDataManager::getInstance();
		if (!physicData || !dataManager) return;
		//获取求解器
		FITKPHengLEIAbsSolver* solver = physicData->getSolver();
		if (!solver) return;

		// 复制Energy模型参数
		Interface::FITKPHengLEIModels* modelsSol = solver->getModels();
		Interface::FITKPHengLEIModels* models = physicData->getModels();
		if (!modelsSol) return;
		Interface::FITKPHengLEIEnergyModel* energyModelSol = modelsSol->getEnergyModel();
		if (energyModelSol) 
		{
			Interface::FITKPHengLEIEnergyModel* energyModel = new Interface::FITKPHengLEIEnergyModel;
			Core::FITKParameter* addEnergyModelSol = energyModelSol->getAdditionalData();
			Core::FITKParameter* addEnergyModel = energyModel->getAdditionalData();
			if (addEnergyModelSol && addEnergyModel)
			{
				addEnergyModel->copyPara(addEnergyModelSol);
			}
			models->setEnergyModel(energyModel);
		}


		// 复制Species模型参数
		Interface::FITKPHengLEISpeciesModel* speciesModelSol = modelsSol->getSpeciesModel();
		if (speciesModelSol)
		{
			Interface::FITKPHengLEISpeciesModel* speciesModel = new Interface::FITKPHengLEISpeciesModel;
			Core::FITKParameter* addSpeciesModelSol = speciesModelSol->getAdditionalData();
			Core::FITKParameter* addSpeciesModel = speciesModel->getAdditionalData();
			if (addSpeciesModelSol && addSpeciesModel)
			{
				addSpeciesModel->copyPara(addSpeciesModelSol);
			}
			models->setSpeciesModel(speciesModel);
		}

		// 复制viscous模型参数
		Interface::FITKPHengLEIViscousModel* viscousModelSol = modelsSol->getViscousModel();
		if (viscousModelSol) 
		{
			Interface::FITKPHengLEIViscousModel* viscousModel = new Interface::FITKPHengLEIViscousModel;
			viscousModel->setViscousEnumType(viscousModelSol->getViscousEnumType());
			models->setViscousModel(viscousModel);
		}
	}

}