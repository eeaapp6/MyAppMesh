/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIDataManager.h
 * @brief  物理数据管理器
 * @author WangKai (wang_starry@outlook.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITKPHengLEI_DATA_MANAGER_H___
#define _FITKPHengLEI_DATA_MANAGER_H___

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include <QMutex>

namespace Interface
{
    class FITKPHengLEISolverManager;
	class FITKPHengLEIInFlowTypeManager;
	class FITKPHengLEISpatialManager;
	class FITKPHengLEIBoundaryTypeManager;
	class FITKPHengLEIViscousTypeManager;
}

namespace Interface
{
    class FITKPHengLEIDataManagerDeleator;
    /**
     * @brief  物理场数据管理器
     * @author WangKai (wang_starry@outlook.com)
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIDataManager
    {
        friend FITKPHengLEIDataManagerDeleator;
        DeclSingleton(FITKPHengLEIDataManager);
    public:
        /**
         * @brief    获取求解器管理器
         * @return   FITKPHengLEISolverManager *
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-08-21
         */
        FITKPHengLEISolverManager* getSolverManager();

		/**
		 * @brief 获取来流类型管理器
		 * @return  FITKPHengLEIInFlowTypeManager
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-09-18
		 */
		FITKPHengLEIInFlowTypeManager* getInFlowManager();
		/**
		 * @brief 获取时间离散类型管理器
		 * @return  FITKPHengLEISpatialManager
		 * @author 
		 * @date 2024-10-05
		 */
		FITKPHengLEISpatialManager* getSpatialManager();
		/**
		 * @brief 获取边界条件类型管理器
		 * @return  FITKPHengLEIBoundaryTypeManager
		 * @author 
		 * @date 2024-10-05
		 */
		FITKPHengLEIBoundaryTypeManager* getBoundaryTypeManager();
		FITKPHengLEIViscousTypeManager* getViscousTypeManager();
    private:
        /**
         * @brief    初始化函数
         * @return   void
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-08-21
         */
        void initialize();
        /**
         * @brief    销毁函数
         * @return   void
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-08-21
         */
        void finalize();
    private:
        /**
         * @brief  求解器管理器
         * @author WangKai (wang_starry@outlook.com)
         * @date   2024-08-21
         */
        FITKPHengLEISolverManager* m_solverManager{};

		/**
		 * @brief 来流参数管理器
		 * @author WangKai (wang_starry@outlook.com)
		 * @date   2024-08-21
		 */
		FITKPHengLEIInFlowTypeManager* m_inFLowManager{};
		/**
		 * @brief 时间离散类型管理器
		 * @author WangKai (wang_starry@outlook.com)
		 * @date   2024-08-21
		 */
		FITKPHengLEISpatialManager* m_spatialManager{};
		/**
		 * @brief 边界条件类型管理器
		 * @author WangKai (wang_starry@outlook.com)
		 * @date   2024-08-21
		 */
		FITKPHengLEIBoundaryTypeManager* m_boundaryTypeManager{};

		FITKPHengLEIViscousTypeManager* m_viscousTypeManager{};
    };

    class FITKPHengLEIDataManagerDeleator
    {
    public:
        explicit FITKPHengLEIDataManagerDeleator() = default;
        virtual ~FITKPHengLEIDataManagerDeleator();
    };
}


#endif // !_FITKPHengLEI_PHYSICS_MANAGER_H___
