/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEISolverManager.h"
#include "FITKPHengLEIData.h"
#include "FITKPHengLEISolverSubsonic.h"
#include "FITKPHengLEISolverTransonic.h"
#include "FITKPHengLEISolverSupersonic.h"
#include "FITKPHengLEISolverHypersonic.h"
#include "FITKPHengLEISolverCompressibleSIMPLE.h"
#include "FITKPHengLEISolverPISO.h"
#include "FITKPHengLEISolverSIMPLE.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITKPHengLEISolverManager.h"

namespace Interface
{
    FITKPHengLEISolverManager::FITKPHengLEISolverManager()
    {
        this->initSolvers();
    }

    FITKPHengLEISolverManager::~FITKPHengLEISolverManager()
    {

    }

    FITKPHengLEISolverInfo FITKPHengLEISolverManager::getSolverInfoBySolverName(QString solverName)
    {
        for (QList<FITKPHengLEISolverInfo>::iterator iter = m_solvers.begin(); iter != m_solvers.end(); ++iter)
        {
            if ((*iter)._solverName == solverName) return *iter;
        }
        return FITKPHengLEISolverInfo();
    }

	QStringList FITKPHengLEISolverManager::getSolvers()
	{
		QStringList sovlers;
		for (QList<FITKPHengLEISolverInfo>::iterator iter = m_solvers.begin();iter!= m_solvers.end();++iter)
		{
			sovlers.append(iter->_solverName);
		}
		return sovlers;
	}

	QStringList FITKPHengLEISolverManager::filterSolvers(QList<FITKPHengLEIAbsSolver::FITKPHengLEISolverFiltersType> filtersList)
	{
		QStringList solvers;
		Core::FITKEnumTransfer<FITKPHengLEIAbsSolver::FITKPHengLEISolverFiltersType> filterTypeTransfer;
		//获取过滤器
		QStringList filters;
		for (FITKPHengLEIAbsSolver::FITKPHengLEISolverFiltersType type : filtersList)
		{
			bool ok = false;
			QString filterStr = filterTypeTransfer.toString(type, ok);
			if (!ok)return QStringList();
			filters.append(filterStr);
		}

		//求解器遍历
		for (QList<FITKPHengLEISolverInfo>::iterator iter = m_solvers.begin(); iter != m_solvers.end(); ++iter)
		{
			//false 被过滤掉 true保留
			bool isFilter = true;
			//进行过滤
			for (QString type : filters)
			{
				isFilter = iter->_solverLabels.contains(type);
				if (!isFilter)break;
			}
			if (!isFilter)continue;
			//添加求解器
			solvers.append(iter->_solverName);
		}
		return solvers;
	}


    void FITKPHengLEISolverManager::initSolvers()
    {
        //Subsonic求解器
        FITKPHengLEISolverInfo infoSubsonic;
		infoSubsonic._solverName = "Subsonic";
		infoSubsonic._solverType = FITKPHengLEIAbsSolver::PHengLEISolverType::PLSubsonic;
		infoSubsonic._solverLabels << "Compressible" << "SteadyState";
		infoSubsonic._createFuns = [&] {return new FITKPHengLEISubsonicSolver; };
        m_solvers.append(infoSubsonic);

        //Transonic求解器
        FITKPHengLEISolverInfo infoTransonic;
		infoTransonic._solverName = "Transonic";
		infoTransonic._solverType = FITKPHengLEIAbsSolver::PHengLEISolverType::PLTransonic;
		infoTransonic._solverLabels << "Compressible" << "SteadyState";
		infoTransonic._createFuns = [&] {return new FITKPHengLEITransonicSolver; };
        m_solvers.append(infoTransonic);

		//Supersonic求解器
		FITKPHengLEISolverInfo infoTSupersonic;
		infoTSupersonic._solverName = "Supersonic";
		infoTSupersonic._solverType = FITKPHengLEIAbsSolver::PHengLEISolverType::PLSupersonic;
		infoTSupersonic._solverLabels << "Compressible" << "SteadyState";
		infoTSupersonic._createFuns = [&] {return new FITKPHengLEISupersonicSolver; };
		m_solvers.append(infoTSupersonic);

		//Hypersonic求解器
		FITKPHengLEISolverInfo infoHypersonic;
		infoHypersonic._solverName = "Hypersonic";
		infoHypersonic._solverType = FITKPHengLEIAbsSolver::PHengLEISolverType::PLHypersonic;
		infoHypersonic._solverLabels << "Compressible" << "SteadyState";
		infoHypersonic._createFuns = [&] {return new FITKPHengLEIHypersonicSolver; };
		m_solvers.append(infoHypersonic);

		// CompressibleSIMPLE求解器
		FITKPHengLEISolverInfo infoCompressibleSIMPLE;
		infoCompressibleSIMPLE._solverName = "Compressible SIMPLE";
		infoCompressibleSIMPLE._solverType = FITKPHengLEIAbsSolver::PHengLEISolverType::PLCompressibleSIMPLE;
		infoCompressibleSIMPLE._solverLabels << "" << "";
		infoCompressibleSIMPLE._createFuns = [&] {return new FITKPHengLEISolverCompressibleSIMPLE; };
		m_solvers.append(infoCompressibleSIMPLE);

		// PISO求解器
		FITKPHengLEISolverInfo infoPISO;
		infoPISO._solverName = "PISO";
		infoPISO._solverType = FITKPHengLEIAbsSolver::PHengLEISolverType::PLPISO;
		infoPISO._solverLabels << "" << "";
		infoPISO._createFuns = [&] {return new FITKPHengLEISolverPISO; };
		m_solvers.append(infoPISO);

		// SIMPLE求解器
		FITKPHengLEISolverInfo infoSIMPLE;
		infoSIMPLE._solverName = "SIMPLE";
		infoSIMPLE._solverType = FITKPHengLEIAbsSolver::PHengLEISolverType::PLSIMPLE;
		infoSIMPLE._solverLabels << "" << "";
		infoSIMPLE._createFuns = [&] {return new FITKPHengLEISolverSIMPLE; };
		m_solvers.append(infoSIMPLE);

    }

    

}