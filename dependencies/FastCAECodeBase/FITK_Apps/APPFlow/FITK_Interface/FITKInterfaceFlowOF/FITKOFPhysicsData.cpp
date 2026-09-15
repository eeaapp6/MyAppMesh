/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFPhysicsData.h"
#include "FITKAbstractOFSolver.h"
#include "FITKAbstractOFTurbulence.h"
#include "FITKOFTransportProp.h"
#include "FITKOFDiscretization.h"
#include "FITKOFBoundary.h"
#include "FITKOFSolution.h"
#include "FITKOFInitialConditions.h"
#include "FITKOFRunControl.h"
#include "FITKOFOperatingConditions.h"
#include "FITKOFRadiation.h"
#include "FITKOFThermo.h"

namespace Interface
{
    FITKOFPhysicsData::FITKOFPhysicsData()
    {
    }

    FITKOFPhysicsData::~FITKOFPhysicsData()
    {
        //销毁数据
        if (_solver) delete _solver;
        if (_turbulence) delete _turbulence;
        if (_transPortProp) delete _transPortProp;
        if (_discretization) delete _discretization;
        if (_boundaryManager) delete _boundaryManager;
        if (_solution) delete _solution;
        if (_initialCond) delete _initialCond;
        if (_runControl) delete _runControl;
        if (_operatingCond) delete _operatingCond;
        if (_radiation) delete _radiation;
        if (_thermo) delete _thermo;
    }

    FITKAbstractOFSolver* FITKOFPhysicsData::getSolver()
    {
        //获取求解器
        return _solver;
    }

    FITKOFSolverTypeEnum::FITKOFRegionMeshType FITKOFPhysicsData::getRegionMeshType(int regionMeshID)
    {
        //获取区域网格类型
        if (_regionType.contains(regionMeshID))
            return _regionType[regionMeshID];
        return FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid;
    }

    bool FITKOFPhysicsData::isEnableTurbulenceEqu()
    {
        //获取是否启用湍流
        return _enableTurb;
    }

    FITKAbstractOFTurbulence* FITKOFPhysicsData::getTurbulence()
    {
        //获取湍流数据
        return _turbulence;
    }

    FITKOFRadiation * FITKOFPhysicsData::getRadiation()
    {
        //获取辐射模型参量
        return _radiation;
    }

    FITKOFThermo * FITKOFPhysicsData::getThermo()
    {
        //获取离散数据
        return _thermo;
    }

    FITKOFTransportProp* FITKOFPhysicsData::getTransportProp()
    {
        //获取材料
        return _transPortProp;
    }

    FITKOFDiscretization* FITKOFPhysicsData::getDiscretization()
    {
        //获取离散
        return _discretization;
    }

    FITKOFSolution* FITKOFPhysicsData::getSolution()
    {
        //获取solution数据
        return _solution;
    }

    FITKOFInitialConditions* FITKOFPhysicsData::getInitialConditions()
    {
        //获取Initial Conditions数据
        return _initialCond;
    }

    FITKOFBoundaryManager* FITKOFPhysicsData::getBoundaryManager()
    {
        //获取边界数据
        return _boundaryManager;
    }

    FITKOFRunControl* FITKOFPhysicsData::getRunControl()
    {
        //获取运行配置数据
        return _runControl;
    }

    FITKOFOperatingConditions* FITKOFPhysicsData::getOperatingConditions()
    {
        //获取求解工作条件
        return _operatingCond;
    }

    void FITKOFPhysicsData::setSolver(FITKAbstractOFSolver* solver)
    {
        //设置求解器
        if (solver != _solver && _solver) delete _solver;
        _solver = solver;
    }

    void FITKOFPhysicsData::setRegionMeshType(int regionMeshID, FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType)
    {
        //设置区域网格类型
        _regionType.insert(regionMeshID, regionMeshType);
    }

    void FITKOFPhysicsData::setEnableTurbulenceEqu(bool isEnable)
    {
        //设置是否启用湍流
        _enableTurb = isEnable;
    }

    void FITKOFPhysicsData::setTurbulence(FITKAbstractOFTurbulence* turbulence)
    {
        //设置湍流数据
        if (turbulence != _turbulence && _turbulence) delete _turbulence;
        _turbulence = turbulence;
    }

    void FITKOFPhysicsData::setRadiation(FITKOFRadiation * radiation)
    {
        //设置湍流数据
        if (radiation != _radiation && _radiation) delete _radiation;
        _radiation = radiation;
    }

    void FITKOFPhysicsData::setThermo(FITKOFThermo * thermo)
    {
        //设置湍流数据
        if (thermo != _thermo && _thermo) delete _thermo;
        _thermo = thermo;
    }

    void FITKOFPhysicsData::setTransportProp(FITKOFTransportProp* transProp)
    {
        //设置材料数据
        if (transProp != _transPortProp && _transPortProp) delete _transPortProp;
        _transPortProp = transProp;
    }

    void FITKOFPhysicsData::setDiscretization(FITKOFDiscretization* discretization)
    {
        //设置离散数据
        if (discretization != _discretization && _discretization) delete _discretization;
        _discretization = discretization;
    }

    void FITKOFPhysicsData::setSolution(FITKOFSolution* solution)
    {
        //设置solution数据
        if (solution != _solution && _solution) delete _solution;
        _solution = solution;
    }

    void FITKOFPhysicsData::setInitialConditions(FITKOFInitialConditions* initial)
    {
        //设置初始值参数配置
        if (initial != _initialCond && _initialCond) delete _initialCond;
        _initialCond = initial;
    }

    void FITKOFPhysicsData::setBoundaryManager(FITKOFBoundaryManager* manager)
    {
        //设置边界管理器
        if (manager != _boundaryManager && _boundaryManager) delete _boundaryManager;
        _boundaryManager = manager;
    }

    void FITKOFPhysicsData::setRunControl(FITKOFRunControl* run)
    {
        //设置运行配置数据
        if (run != _runControl && _runControl)delete _runControl;
        _runControl = run;
    }

    void FITKOFPhysicsData::setOperatingConditions(FITKOFOperatingConditions* operatingCond)
    {
        //设置求解工作条件
        if (operatingCond != _operatingCond && _operatingCond)delete _operatingCond;
        _operatingCond = operatingCond;
    }

    QHash<int, FITKOFSolverTypeEnum::FITKOFRegionMeshType> FITKOFPhysicsData::getAllRegionMeshType()
    {
        return _regionType;
    }

    void FITKOFPhysicsData::clearData()
    {
        //清理数据
        _regionType.clear();
        _enableTurb = false;
        if (_solver) delete _solver;
        _solver = nullptr;
        if (_turbulence) delete _turbulence;
        _turbulence = nullptr;
        if (_transPortProp) delete _transPortProp;
        _transPortProp = nullptr;
        if (_discretization) delete _discretization;
        _discretization = nullptr;
        if (_solution) delete _solution;
        _solution = nullptr;
        if (_boundaryManager) delete _boundaryManager;
        _boundaryManager = nullptr;
        if (_initialCond) delete _initialCond;
        _initialCond = nullptr;
        if (_runControl) delete _runControl;
        _runControl = nullptr;
        if (_operatingCond) delete _operatingCond;
        _operatingCond = nullptr;
        if (_radiation) delete _radiation;
        _radiation = nullptr;
        if (_thermo) delete _thermo;
        _thermo = nullptr;
    }
}
