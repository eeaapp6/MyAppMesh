/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFSolutionSolverManager.h"
#include "FITKOFSolutionSolver.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

namespace Interface
{
    FITKOFSolutionSolverManager::FITKOFSolutionSolverManager()
    {
        this->initSolutionSolvers();
    }

    FITKOFSolutionSolverManager::~FITKOFSolutionSolverManager()
    {

    }

    QStringList FITKOFSolutionSolverManager::filterSolutionSolvers(QString variableType, FITKOFSolverTypeEnum::FITKOFSolverType solverType)
    {
        QStringList listType;
        //求解器遍历
        for (QList<FITKOFSolutionSolverInfo>::iterator iter = m_solutionSolvers.begin(); iter != m_solutionSolvers.end(); ++iter)
        {
            //进行过滤
            if (iter->_enableType.contains(solverType) && iter->_enableType[solverType].contains(variableType))
                listType.append(iter->_solutionSolverName);
        }
        return listType;
    }

    FITKOFSolutionSolverInfo FITKOFSolutionSolverManager::getSolutionSolverInfo(QString solutionSolverName)
    {
        for (QList<FITKOFSolutionSolverInfo>::iterator iter = m_solutionSolvers.begin(); iter != m_solutionSolvers.end(); ++iter)
        {
            if (iter->_solutionSolverName == solutionSolverName) return *iter;
        }
        return FITKOFSolutionSolverInfo();
    }

    void FITKOFSolutionSolverManager::initSolutionSolvers()
    {
        //GAMG
        FITKOFSolutionSolverInfo infoGAMG;
        infoGAMG._solutionSolverName = "GAMG";
        infoGAMG._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE, QStringList() << "p" << "U" << "k" << "omega");
        infoGAMG._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::INTER, QStringList() << "p_rgh" << "U" << "pcorr" << "k" << "alpha" << "omega");
        infoGAMG._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION, QStringList() << "p_rgh" << "U" << "h" << "p" << "rho" << "e(solid)");
        infoGAMG._createFuns = [&] { return new FITKOFSolutionSolverGAMG; };
        m_solutionSolvers.append(infoGAMG);

        //PCG
        FITKOFSolutionSolverInfo infoPCG;
        infoPCG._solutionSolverName = "PCG";
        infoPCG._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE, QStringList() << "p");
        infoPCG._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::INTER, QStringList() << "p_rgh" << "pcorr");
        infoPCG._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION, QStringList() << "p_rgh" << "p" << "rho" << "e(solid)");
        infoPCG._createFuns = [&] { return new FITKOFSolutionSolverPCG; };
        m_solutionSolvers.append(infoPCG);

        //Smooth Solver
        FITKOFSolutionSolverInfo infoSmoothSolver;
        infoSmoothSolver._solutionSolverName = "Smooth Solver";
        infoSmoothSolver._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE, QStringList() << "p" << "U" << "k" << "omega");
        infoSmoothSolver._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::INTER, QStringList() << "p_rgh" << "U" << "pcorr" << "k" << "alpha" << "omega");
        infoSmoothSolver._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION, QStringList() << "p_rgh" << "U" << "h" << "p" << "rho" << "e(solid)");
        infoSmoothSolver._createFuns = [&] { return new FITKOFSolutionSolverSmoothSolver; };
        m_solutionSolvers.append(infoSmoothSolver);

        //PBiCG Stabilized
        FITKOFSolutionSolverInfo infoPBiCGStabilized;
        infoPBiCGStabilized._solutionSolverName = "PBiCG Stabilized";
        infoPBiCGStabilized._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE, QStringList() << "U" << "k" << "omega");
        infoPBiCGStabilized._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::INTER, QStringList() << "U" << "omega" << "alpha" << "k" << "omega");
        infoPBiCGStabilized._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION, QStringList() << "U" << "h");
        infoPBiCGStabilized._createFuns = [&] { return new FITKOFSolutionSolverPBiCGStabilized; };
        m_solutionSolvers.append(infoPBiCGStabilized);

        //PBiCG
        FITKOFSolutionSolverInfo infoPBiCG;
        infoPBiCG._solutionSolverName = "PBiCG";
        infoPBiCG._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE, QStringList() << "U" << "k" << "omega");
        infoPBiCG._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::INTER, QStringList() << "U" << "omega" << "alpha" << "k" << "omega");
        infoPBiCG._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION, QStringList() << "U" << "h");
        infoPBiCG._createFuns = [&] { return new FITKOFSolutionSolverPBiCG; };
        m_solutionSolvers.append(infoPBiCG);
    }
}