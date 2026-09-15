/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFPhysicsManager.h
 * @brief  物理数据管理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITKOF_PHYSICS_MANAGER_H___
#define _FITKOF_PHYSICS_MANAGER_H___

#include "FITKInterfaceFlowOFAPI.h"
#include "FITKOFEnum.hpp"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include <QMutex>

namespace Interface
{
    class FITKOFSolverManager;
    class FITKOFTurbulenceManager;
    class FITKOFSolutionSolverManager;
    class FITKOFTransportModelManager;
    class FITKOFBoundaryTypeManager;
    class FITKOFRadiationParaManager;
    class FITKOFThermoPropManager;
}

namespace Interface
{
    class FITKOFPhysicsManagerDeleator;
    /**
     * @brief  物理场数据管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKInterfaceFlowOFAPI FITKOFPhysicsManager
    {
        friend FITKOFPhysicsManagerDeleator;
        DeclSingleton(FITKOFPhysicsManager);
    public:
        /**
         * @brief    获取求解器管理器
         * @return   FITKOFSolverManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFSolverManager* getSolverManager();
        /**
         * @brief    获取湍流模型管理器
         * @return   FITKOFTurbulenceManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFTurbulenceManager* getTurbulenceManager();
        /**
         * @brief    获取SolutionSolver管理器
         * @return   FITKOFSolutionSolverManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        FITKOFSolutionSolverManager* getSolutionSolverManager();
        /**
         * @brief    获取物理材料模型管理器
         * @return   FITKOFTransportModelManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        FITKOFTransportModelManager* getTransportModelManager();
        /**
         * @brief    获取边界类型管理器
         * @return   FITKOFBoundaryTypeManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-26
         */
        FITKOFBoundaryTypeManager* getBoundaryTypeManager();
        /**
         * @brief    获取辐射参数管理器
         * @return   FITKOFRadiationParaManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-26
         */
        FITKOFRadiationParaManager* getRadiationParaManager();
        /**
         * @brief    获取热物理属性管理器
         * @return   FITKOFThermoPropManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        FITKOFThermoPropManager* getThermoPropManager();

    private:
        /**
         * @brief    初始化函数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initialize();
        /**
         * @brief    销毁函数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void finalize();

    private:
        /**
         * @brief  求解器管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        FITKOFSolverManager* m_solverManager{};
        /**
         * @brief  湍流管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        FITKOFTurbulenceManager* m_turbulenceManager{};
        /**
         * @brief  solution Solver管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        FITKOFSolutionSolverManager* m_solutionSolverManager{};
        /**
         * @brief  物理材料模型管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-23
         */
        FITKOFTransportModelManager* m_transportModelManager{};
        /**
         * @brief  边界类型管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-26
         */
        FITKOFBoundaryTypeManager* m_boundaryTypeManager{};
        /**
         * @brief  辐射参数管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-26
         */
        FITKOFRadiationParaManager* m_radiationParaManager{};
        /**
         * @brief  热物理属性管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-20
         */
        FITKOFThermoPropManager* m_thermoPropManager{};
    };

    class FITKOFPhysicsManagerDeleator
    {
    public:
        explicit FITKOFPhysicsManagerDeleator() = default;
        virtual ~FITKOFPhysicsManagerDeleator();
    };
}


#endif // !_FITKOF_PHYSICS_MANAGER_H___
