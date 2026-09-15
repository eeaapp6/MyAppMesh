/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFSolverManager.h
 * @brief  求解器管理
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITK___OF____SOLVER_MANAGER__H___
#define _FITK___OF____SOLVER_MANAGER__H___

#include "FITKInterfaceFlowOFAPI.h"
#include "FITKOFEnum.hpp"
#include <functional>
#include <QStringList>

namespace Interface
{
    class FITKAbstractOFSolver;
}

typedef std::function<Interface::FITKAbstractOFSolver*()>  CREATESOLVERFUN;


namespace Interface
{
    /**
     * @brief  求解器信息 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKOFSolverInfo 
    {
        QString _solverName{};
        FITKOFSolverTypeEnum::FITKOFSolverType _solverType{ FITKOFSolverTypeEnum::FITKOFSolverType::SolverNone};
        QStringList _solverLabels{};
        CREATESOLVERFUN _createFuns{};
    };
    /**
     * @brief  求解器管理
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKInterfaceFlowOFAPI FITKOFSolverManager
    {
    public:
        explicit FITKOFSolverManager();
        virtual ~FITKOFSolverManager();

        /**
         * @brief    通过过滤类型获取求解器名称链表
         * @param[i] filtersList
         * @return   QList<FITKOFPostProcessEnum::FITKOFSolverType>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        QStringList filterSolvers(QList<FITKOFSolverTypeEnum::FITKOFSolverFiltersType> filtersList);
        /**
         * @brief    获取求解器信息
         * @param[i] solverName 求解器名称
         * @return   FITKOFSolverInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFSolverInfo getSolverInfoBySolverName(QString solverName);

    private:
        /**
         * @brief    初始化求解器数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initSolvers();
    
    private:
        /**
         * @brief  求解器数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        QList<FITKOFSolverInfo> m_solvers{};
    };
}


#endif
