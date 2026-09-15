/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEISolverManager.h
 * @brief  求解器管理
 * @author WangKai (wang_starry@outlook.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITK___PHENGLEI____SOLVER_MANAGER__H___
#define _FITK___PHENGLEI____SOLVER_MANAGER__H___

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include <functional>
#include <QStringList>
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIAbsSolver.h"
#include <QStringList>

namespace Interface
{
	class  FITKPHengLEIAbsSolver;
}

typedef std::function<Interface::FITKPHengLEIAbsSolver*()>  CREATEPHengLEISOLVERFUN;


namespace Interface
{
    /**
     * @brief  求解器信息 结构体
     * @author WangKai (wang_starry@outlook.com)
     * @date   2024-08-21
     */
    struct FITKPHengLEISolverInfo
    {
        QString _solverName{};
        FITKPHengLEIAbsSolver::PHengLEISolverType _solverType{ FITKPHengLEIAbsSolver::PHengLEISolverType::PLSolverNone};
        QStringList _solverLabels{};
		CREATEPHengLEISOLVERFUN _createFuns{};
    };
    /**
     * @brief  求解器管理
     * @author WangKai (wang_starry@outlook.com)
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI FITKPHengLEISolverManager
    {
    public:
        explicit FITKPHengLEISolverManager();
        virtual ~FITKPHengLEISolverManager();

        /**
         * @brief    获取求解器信息
         * @param[i] solverName 求解器名称
         * @return   FITKPHengLEISolverInfo
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-08-21
         */
        FITKPHengLEISolverInfo getSolverInfoBySolverName(QString solverName);

		QStringList getSolvers();

		/**
		 * @brief    通过过滤类型获取求解器名称链表
		 * @param[i] filtersList
		 * @return   QList<FITKOFPostProcessEnum::FITKOFSolverType>
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-08-21
		 */
		QStringList filterSolvers(QList<FITKPHengLEIAbsSolver::FITKPHengLEISolverFiltersType> filtersList);

    private:
        /**
         * @brief    初始化求解器数据信息
         * @return   void
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-08-21
         */
        void initSolvers();
    
    private:
        /**
         * @brief  求解器数据信息
         * @author WangKai (wang_starry@outlook.com)
         * @date   2024-08-21
         */
        QList<FITKPHengLEISolverInfo> m_solvers{};
    };
}


#endif
