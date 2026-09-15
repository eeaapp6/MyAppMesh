/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKPHengLEISolverSupersonic.h
 * @brief 超声速求解器
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-09-11
 * 
 */
#ifndef _FITK_PHENGLEI_SOLVER_SUPERSONIC__H___
#define _FITK_PHENGLEI_SOLVER_SUPERSONIC__H___
 
#include "FITKPHengLEIAbsSolver.h"

namespace Interface
{
    /**
     * @brief 亚音速求解器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-09-11
     */
    class FITKFlowPHengLEIAPI FITKPHengLEISupersonicSolver : public FITKPHengLEIAbsSolver
    {
    public:
        /**
         * @brief Construct a new FITKPHengLEISupersonicSolver object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        explicit FITKPHengLEISupersonicSolver();
        /**
         * @brief Destroy the FITKPHengLEISupersonicSolver object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual ~FITKPHengLEISupersonicSolver() = default;
        /**
         * @brief 获取求解器类型
         * @return PHengLEISolverType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-12
         */
        virtual PHengLEISolverType getSolverType() override;

    protected:
        /**
         * @brief 初始化边界条件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initBoundaryTypes() override;
        /**
         * @brief 初始化基本信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initBasicInfo() override;
        /**
         * @brief 初始化来流信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initInflow() override;
        /**
         * @brief 初始化网格参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initGridParam() override;
        /**
         * @brief 初始化粘度
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initViscous()  override;
        /**
         * @brief 初始化离散参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initDiscrete() override;
        /**
         * @brief 初始化多重网格参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initMultiGridParam() override;
		/**
		 * @brief 初始化计算输出参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		virtual void initOutputSetting() override;

		virtual void initFlow() override;

		virtual void initCalculation() override;

		virtual void initInitialCondition() override;

		virtual void initSolution() override;

		virtual void initModels() override;
    };
}


#endif
