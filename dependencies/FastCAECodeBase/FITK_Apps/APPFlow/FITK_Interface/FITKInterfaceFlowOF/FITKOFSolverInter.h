/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFSolverInter.h
 * @brief  Inter 求解器数据
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-30
 *********************************************************************/
#ifndef _FITKSOLVER_OF_INTER_H___
#define _FITKSOLVER_OF_INTER_H___
 
#include "FITKAbstractOFSolver.h"


namespace Interface
{
    /**
     * @brief  Inter 求解器数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class  FITKInterfaceFlowOFAPI FITKOFSolverInter : public FITKAbstractOFSolver
    {
    public:
        explicit FITKOFSolverInter();
        virtual ~FITKOFSolverInter() = default;
        /**
         * @brief    获取求解器类型
         * @return   FITKOFSolverTypeEnum::FITKOFSolverType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFSolverTypeEnum::FITKOFSolverType getSolverType() override;
        /**
         * @brief    获取求解器运行命令
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-06
         */
        virtual QString getSolverCommand() override;

    private:
        /**
         * @brief    初始化材料
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initTransportProp() override;
        /**
         * @brief    初始化离散
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initDiscretization() override;
        /**
         * @brief    初始化solution数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initSolution() override;
        /**
         * @brief    初始化边界数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-26
         */
        virtual void initBoundarys() override;
        /**
         * @brief    初始化 Initial Conditions数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        virtual void initInitialConditions() override;
        /**
         * @brief    初始化运行配置数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-31
         */
        virtual void initRunControl() override;
        /**
         * @brief    初始化工作条件
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-04
         */
        virtual void initOperatingConditions() override;
    };
}


#endif
