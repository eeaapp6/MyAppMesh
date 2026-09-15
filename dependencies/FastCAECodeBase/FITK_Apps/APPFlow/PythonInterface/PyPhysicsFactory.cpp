/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyPhysicsFactory.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFSolver.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundary.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportModel.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundaryType.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRunControl.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace  PyInterface
{
    PyPhysicsFactory::PyPhysicsFactory(QObject* parent)
        : QObject(parent)
    {
        _factoryData = FITKAPP->getComponents()->getComponentTByName<Interface::FITKFlowPhysicsHandlerFactory>("FITKFlowPhysicsHandlerFactory");
    }

    bool PyPhysicsFactory::setSolver(int solverType)
    {
        if (_factoryData == nullptr) return false;

        switch (solverType)
        {
        case Interface::FITKOFSolverTypeEnum::SIMPLE:
            _factoryData->setSolver("SIMPLE");
            return true;
        case Interface::FITKOFSolverTypeEnum::INTER:
            _factoryData->setSolver("Inter");
            return true;
        case Interface::FITKOFSolverTypeEnum::CHT_MULTI_REGION:
            _factoryData->setSolver("CHT Multi Region");
            return true;
        default:
            return false;
        }
    }

    bool PyPhysicsFactory::setTurbence(int turbulenceType)
    {
        if (_factoryData == nullptr) return false;

        switch (turbulenceType)
        {
        case Interface::FITKOFSolverTypeEnum::TurbNone:
            _factoryData->setTurbence("");
            return true;
        case Interface::FITKOFSolverTypeEnum::RANS:
            _factoryData->setTurbence("k-Omega SST");
            return true;
        case Interface::FITKOFSolverTypeEnum::LES:
            return false;
        default:
            return false;
        }
    }

    bool PyPhysicsFactory::setBoundary(const QString& regionName, const QString& boundaryName, int boundaryType, int regionType)
    {
        if (_factoryData == nullptr) return false;

        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (globalData == nullptr) return false;

        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (physicsData == nullptr || meshData == nullptr) return false;

        Interface::FITKFluidRegionsMesh* region = meshData->getDataByName(regionName);
        if (region == nullptr) return false;

        Interface::FITKBoundaryMeshVTKManager* boundaryMeshManager = region->getBoundaryMeshManager();
        if (boundaryMeshManager == nullptr) return false;

        Interface::FITKBoundaryMeshVTK* boundaryMesh = boundaryMeshManager->getDataByName(boundaryName);
        if (boundaryMesh == nullptr) return false;

        Interface::FITKOFBoundaryManager* boundaryManager = physicsData->getBoundaryManager();
        if (boundaryManager == nullptr) return false;
        if (boundaryManager->getBoundary(boundaryMesh->getDataObjectID()) != nullptr) return false;

        _factoryData->setBoundary(
            region->getDataObjectID(),
            boundaryMesh->getDataObjectID(),
            static_cast<Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType>(boundaryType),
            static_cast<Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType>(regionType));

        Interface::FITKOFBoundary* boundary = boundaryManager->getBoundary(boundaryMesh->getDataObjectID());
        if (boundary == nullptr) return false;

        boundary->setDataObjectName(boundaryName);
        return true;
    }

    bool PyPhysicsFactory::setNewtonianModelParam(double v)
    {
        Interface::FITKAbstractOFSolver* solver = getSolver();
        if (solver == nullptr) return false;

        Interface::FITKOFTransportProp* transportProp = solver->getTransportProp();
        if (transportProp == nullptr) return false;

        Interface::FITKOFTransportPhase* phase = transportProp->getPhase(0);
        if (phase == nullptr) return false;

        Interface::FITKAbsOFTransportModel* transportModel = phase->getTransportModel();
        if (transportModel == nullptr || transportModel->getDataObjectName() != "Newtonian") return false;

        Core::FITKParameter* modelPara = transportModel->getTransportModelPara();
        if (modelPara == nullptr) return false;

        Core::FITKParamDouble* vPara = dynamic_cast<Core::FITKParamDouble*>(modelPara->getDataByName("v [m2/s]"));
        if (vPara == nullptr) return false;

        vPara->setValue(v);
        return true;
    }

    bool PyPhysicsFactory::setSurfaceNormalFixedValueBCParam(const QString& name, double v)
    {
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        Interface::FITKOFBoundaryManager* boundaryManager = physicsData->getBoundaryManager();
        if (boundaryManager == nullptr) return false;

        Interface::FITKOFBoundary* boundary = boundaryManager->getDataByName(name);
        if (boundary == nullptr) return false;

        Interface::FITKOFAbsBoundaryType* boundPara = nullptr;
        for (int i = 0; i < boundary->getFlowCount(); ++i)
        {
           QString nameV = boundary->getFlowVariableName(i);
           if(nameV != "U") continue;
           boundPara = boundary->getFlowVBType(i);
        }
        if (boundPara == nullptr ) return false;

        Core::FITKParameter* bcPara = boundPara->getBoundaryTypePara();
        if (bcPara == nullptr) return false;

        Core::FITKParamDouble* referenceValuePara = dynamic_cast<Core::FITKParamDouble*>(bcPara->getDataByName("Reference Value [m/s]"));
        if (referenceValuePara == nullptr) return false;

        referenceValuePara->setValue(v);
        return true;
    }

    bool PyPhysicsFactory::setTotalPressureBCParam(const QString &name, double p0)
    {
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        Interface::FITKOFBoundaryManager* boundaryManager = physicsData->getBoundaryManager();
        if (boundaryManager == nullptr) return false;

        Interface::FITKOFBoundary* boundary = boundaryManager->getDataByName(name);
        if (boundary == nullptr) return false;

        Interface::FITKOFAbsBoundaryType* boundPara = nullptr;
        for (int i = 0; i < boundary->getFlowCount(); ++i)
        {
           QString nameV = boundary->getFlowVariableName(i);
           if(nameV != "p") continue;
           boundPara = boundary->getFlowVBType(i);
        }
        if (boundPara == nullptr ) return false;

        Core::FITKParameter* bcPara = boundPara->getBoundaryTypePara();
        if (bcPara == nullptr) return false;

        Core::FITKParamDouble* p0Para = dynamic_cast<Core::FITKParamDouble*>(bcPara->getDataByName("p0 [m2/s2]"));
        if (p0Para == nullptr) return false;

        p0Para->setValue(p0);
        return true;
    }

    bool PyPhysicsFactory::setRunTimeParam(double time)
    {
         //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        Interface::FITKOFRunControl* runControl = physicsData->getRunControl();
        if (!runControl) return false;

        Core::FITKParameter* tiemPara = runControl->getTimeControl();
        if (tiemPara == nullptr) return false;
        Core::FITKParamInt* timePara = dynamic_cast<Core::FITKParamInt*>(tiemPara->getDataByName("Number of Iterations"));
        if (timePara == nullptr) return false;
        timePara->setValue(time);

        return true;
    }

    bool PyPhysicsFactory::setRunOutputParam(double param)
    {
            //获取全局数据管理器
            AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
            if (!globalData)return false;
            //获取物理数据
            Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
            if (!physicsData)return false;
            Interface::FITKOFRunControl* runControl = physicsData->getRunControl();
            if (!runControl) return false;
    
            Core::FITKParameter* outputPara = runControl->getOutputControl();
            if (outputPara == nullptr) return false;
            Core::FITKParamInt* outputTimePara = dynamic_cast<Core::FITKParamInt*>(outputPara->getDataByName("Write Interval"));
            if (outputTimePara == nullptr) return false;
            outputTimePara->setValue(param);
    
            return true;
    }

    Interface::FITKAbstractOFSolver *PyPhysicsFactory::getSolver()
    {
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (globalData == nullptr) return nullptr;

        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (physicsData == nullptr) return nullptr;

        return physicsData->getSolver();
    }
}