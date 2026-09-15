/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFSolutionSolver.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

namespace Interface
{
    FITKOFAbsSolutionSolver::FITKOFAbsSolutionSolver()
    {
        //初始化
        _solverSolutionParams = new Core::FITKParameter;
    }

    FITKOFAbsSolutionSolver::~FITKOFAbsSolutionSolver()
    {
        //销毁对象
        if (_solverSolutionParams) delete _solverSolutionParams;
    }

    Core::FITKParameter* FITKOFAbsSolutionSolver::getSolverSolutionPara()
    {
        //获取参数
        return _solverSolutionParams;
    }

    void FITKOFAbsSolutionSolver::initSolverSolutionParam()
    {
        //设置参数
    }

    FITKOFSolutionSolverGAMG::FITKOFSolutionSolverGAMG()
    {
        //初始化
        this->setDataObjectName("GAMG");
        this->initSolverSolutionParam();
    }

    FITKOFSolutionSolverGAMG::~FITKOFSolutionSolverGAMG()
    {
        //销毁对象
    }

    void FITKOFSolutionSolverGAMG::initSolverSolutionParam()
    {
        //设置参数
        if (!_solverSolutionParams) return;
        _solverSolutionParams->createComboxParameter("Smoother", QStringList() << "Gauss-Seidel" << "DIC" << "DIC & Gauss-Seidel", 0);
        _solverSolutionParams->createBoolParameter("Cache Agglomeration", true);
        _solverSolutionParams->createComboxParameter("Agglomerator", QStringList() << "Face Area Pair" << "Algebraic Pair", 0);
        _solverSolutionParams->createIntParameter("Cells In Coarsest Level", 10);
        _solverSolutionParams->createIntParameter("Merge Levels", 1);
        _solverSolutionParams->createIntParameter("Pre Sweeps", 0);
        _solverSolutionParams->createIntParameter("Post Sweeps", 1);
        _solverSolutionParams->createIntParameter("Finest Sweeps", 2);
        _solverSolutionParams->createIntParameter("Max Iterations", 100);
        _solverSolutionParams->createDoubleParameter("Tolerance", 1e-06);
        _solverSolutionParams->createDoubleParameter("Relative Tolerance", 1e-02);
    }

    FITKOFSolutionSolverPCG::FITKOFSolutionSolverPCG()
    {
        //初始化
        this->setDataObjectName("PCG");
        this->initSolverSolutionParam();
    }

    FITKOFSolutionSolverPCG::~FITKOFSolutionSolverPCG()
    {
        //销毁对象
    }

    void FITKOFSolutionSolverPCG::initSolverSolutionParam()
    {
        //设置参数
        if (!_solverSolutionParams) return;
        _solverSolutionParams->createComboxParameter("Preconditioner", QStringList() << "DIC" << "Fast DIC" << "Diagonal" << "none", 0);
        _solverSolutionParams->createIntParameter("Max Iterations", 100);
        _solverSolutionParams->createDoubleParameter("Tolerance", 1e-06);
        _solverSolutionParams->createDoubleParameter("Relative Tolerance", 1e-02);
    }

    FITKOFSolutionSolverSmoothSolver::FITKOFSolutionSolverSmoothSolver()
    {
        //初始化
        this->setDataObjectName("Smooth Solver");
        this->initSolverSolutionParam();
    }

    FITKOFSolutionSolverSmoothSolver::~FITKOFSolutionSolverSmoothSolver()
    {
        //销毁对象
    }

    void FITKOFSolutionSolverSmoothSolver::initSolverSolutionParam()
    {
        //设置参数
        if (!_solverSolutionParams) return;
        _solverSolutionParams->createComboxParameter("Smoother", QStringList() << "Gauss-Seidel" << "DIC" << "DIC & Gauss-Seidel", 0);
        _solverSolutionParams->createIntParameter("Sweeps", 2);
        _solverSolutionParams->createIntParameter("Max Iterations", 100);
        _solverSolutionParams->createDoubleParameter("Tolerance", 1e-06);
        _solverSolutionParams->createDoubleParameter("Relative Tolerance", 0.1);
    }

    FITKOFSolutionSolverPBiCGStabilized::FITKOFSolutionSolverPBiCGStabilized()
    {
        //初始化
        this->setDataObjectName("PBiCG Stabilized");
        this->initSolverSolutionParam();
    }

    FITKOFSolutionSolverPBiCGStabilized::~FITKOFSolutionSolverPBiCGStabilized()
    {
        //销毁对象
    }

    void FITKOFSolutionSolverPBiCGStabilized::initSolverSolutionParam()
    {
        //设置参数
        if (!_solverSolutionParams) return;
        _solverSolutionParams->createComboxParameter("Preconditioner", QStringList() << "DILU" << "Diagonal" << "none", 0);
        _solverSolutionParams->createIntParameter("Max Iterations", 100);
        _solverSolutionParams->createDoubleParameter("Tolerance", 1e-06);
        _solverSolutionParams->createDoubleParameter("Relative Tolerance", 0.1);
    }

    FITKOFSolutionSolverPBiCG::FITKOFSolutionSolverPBiCG()
    {
        //初始化
        this->setDataObjectName("PBiCG");
        this->initSolverSolutionParam();
    }

    FITKOFSolutionSolverPBiCG::~FITKOFSolutionSolverPBiCG()
    {
        //销毁对象
    }

    void FITKOFSolutionSolverPBiCG::initSolverSolutionParam()
    {
        //设置参数
        if (!_solverSolutionParams) return;
        _solverSolutionParams->createComboxParameter("Preconditioner", QStringList() << "DILU" << "Diagonal" << "none", 0);
        _solverSolutionParams->createIntParameter("Max Iterations", 100);
        _solverSolutionParams->createDoubleParameter("Tolerance", 1e-06);
        _solverSolutionParams->createDoubleParameter("Relative Tolerance", 0.1);
    }
}
