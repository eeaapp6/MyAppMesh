/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractOFTurbulence.h"
#include "FITKOFDiscretization.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITKOFSolution.h"
#include "FITKOFInitialConditions.h"
#include "FITKOFBoundary.h"

namespace Interface
{
    FITKAbstractOFTurbulence::FITKAbstractOFTurbulence()
    {
        //初始化
        _turbParams = new Core::FITKParameter;
        _delta = new Core::FITKParameter;
        _discretization = new FITKOFDiscretization;
        _solution = new FITKOFSolution;
        _initialCond = new FITKOFInitialConditions;
    }

    FITKAbstractOFTurbulence::~FITKAbstractOFTurbulence()
    {
        //销毁数据
        if (_turbParams) delete _turbParams;
        if (_delta) delete _delta;
        if (_discretization) delete _discretization;
        if (_solution) delete _solution;
        for (FITKOFBoundary* bd : _boundaryInfos)
            if (bd) delete bd;
        _boundaryInfos.clear();
        if (_initialCond) delete _initialCond;
    }

    FITKOFSolverTypeEnum::FITKOFTurbulenceType FITKAbstractOFTurbulence::getTurbulenceType()
    {
        //获取湍流类型
        return FITKOFSolverTypeEnum::FITKOFTurbulenceType::TurbNone;
    }

    Core::FITKParameter* FITKAbstractOFTurbulence::getTurbulenceParams()
    {
        //获取湍流模型参数
        return _turbParams;
    }

    Core::FITKParameter* FITKAbstractOFTurbulence::getDeltaParams()
    {
        //获取Delta
        return _delta;
    }

    FITKOFSolution* FITKAbstractOFTurbulence::getSolution()
    {
        //获取solution数据
        return _solution;
    }

    FITKOFInitialConditions* FITKAbstractOFTurbulence::getInitialConditions()
    {
        return _initialCond;
    }

    FITKOFBoundary* FITKAbstractOFTurbulence::getBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType bType)
    {
        for (FITKOFBoundary* bd : _boundaryInfos)
        {
            if (!bd)continue;
            if (bd->getBoundaryType() == bType)
                return bd;
        }
        return nullptr;
    }

    FITKOFDiscretization* FITKAbstractOFTurbulence::getDiscretization()
    {
        //获取离散
        return _discretization;
    }

    void FITKAbstractOFTurbulence::init()
    {
        //初始化
        this->initTurbParam();
        this->initDelta();
        this->initDiscretization();
        this->initSolution();
        this->initBoundary();
        this->initInitialConditions();
    }

    void FITKAbstractOFTurbulence::initTurbParam()
    {
        //初始化湍流模型参数
    }

    void FITKAbstractOFTurbulence::initDelta()
    {
        //初始化Delta参数
    }

    void FITKAbstractOFTurbulence::initDiscretization()
    {
        //初始湍流模型下特定的离散数据
    }

    void FITKAbstractOFTurbulence::initSolution()
    {
        //初始化solution
    }

    void FITKAbstractOFTurbulence::initBoundary()
    {
        //初始化边界
    }


    void FITKAbstractOFTurbulence::initInitialConditions()
    {
        //初始化Initial Conditions
    }
}



