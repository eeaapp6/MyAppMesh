/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFSolutionSolverManager.h
 * @brief  solution Solver管理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-22
 *********************************************************************/
#ifndef _FITK___OF__SOLUTION__SOLVER_MANAGER__H___
#define _FITK___OF__SOLUTION__SOLVER_MANAGER__H___

#include "FITKInterfaceFlowOFAPI.h"
#include "FITKOFEnum.hpp"
#include <functional>
#include <QHash>
#include <QStringList>

namespace Interface
{
    class FITKOFAbsSolutionSolver;
}

typedef std::function<Interface::FITKOFAbsSolutionSolver*()>  CREATESOLUTIONSOLVERFUN;


namespace Interface
{
    /**
     * @brief  Solution Solver信息 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKOFSolutionSolverInfo
    {
        QString _solutionSolverName{};
        QHash<FITKOFSolverTypeEnum::FITKOFSolverType, QStringList> _enableType;//SolverType, VariableType
        CREATESOLUTIONSOLVERFUN _createFuns{};
    };
    /**
     * @brief  solution Solver管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKInterfaceFlowOFAPI FITKOFSolutionSolverManager
    {
    public:
        explicit FITKOFSolutionSolverManager();
        virtual ~FITKOFSolutionSolverManager();

        /**
         * @brief    过滤solution solver类型
         * @param[i] variableType 
         * @param[i] solverType 
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        QStringList filterSolutionSolvers(QString variableType, FITKOFSolverTypeEnum::FITKOFSolverType solverType);
        /**
         * @brief    获取solution solver信息
         * @param[i] solutionSolverName solution solver名称
         * @return   FITKOFSolverInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFSolutionSolverInfo getSolutionSolverInfo(QString solutionSolverName);

    private:
        /**
         * @brief    初始化Solution Solver数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initSolutionSolvers();
    
    private:
        /**
         * @brief  Solution Solver数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        QList<FITKOFSolutionSolverInfo> m_solutionSolvers{};
    };
}


#endif
