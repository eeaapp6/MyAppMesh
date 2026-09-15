/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_OF__SOLVER_CHT_MULTI__RIGON_H___
#define _FITK_OF__SOLVER_CHT_MULTI__RIGON_H___

#include "FITKAbstractOFSolver.h"

namespace Interface
{
    /**
     * @brief    CHT Multi Rigon 求解器类型 chtMultiRigonFoam求解器
     * @author   libaojun
     * @date     2025-05-08
     */
    class FITKInterfaceFlowOFAPI FITKOFSolverCHTMultiRigon : public FITKAbstractOFSolver
    {
    public:
        explicit FITKOFSolverCHTMultiRigon();
        virtual ~FITKOFSolverCHTMultiRigon() = default;
        /**
         * @brief    获取求解器类型
         * @return   FITKOFSolverTypeEnum::FITKOFSolverType
         * @author   libaojun
         * @date     2025-05-08
         */
        FITKOFSolverTypeEnum::FITKOFSolverType getSolverType() override;
        /**
         * @brief    获取求解器运行命令
         * @return   QString
         * @author   libaojun
         * @date     2025-05-08
         */
        virtual QString getSolverCommand() override;

    private:
        /**
         * @brief    初始化辐射参量
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        virtual void initRadiation() override;
        /**
         * @brief    初始化定义材料的热物理属性
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        virtual void initThermo() override;
        /**
         * @brief    初始化材料
         * @return   void
         * @author   libaojun
         * @date     2025-05-08
         */
        virtual void initTransportProp() override;
        /**
         * @brief    初始化离散数据
         * @return   void
         * @author   libaojun
         * @date     2025-05-08
         */
        virtual void initDiscretization() override;
        /**
         * @brief    初始化solution数据
         * @return   void
         * @author   libaojun
         * @date     2025-05-08
         */
        virtual void initSolution() override;
        /**
         * @brief    初始化边界数据
         * @return   void
         * @author   libaojun
         * @date     2025-05-08
         */
        virtual void initBoundarys() override;
        /**
         * @brief    初始化 Initial Conditions数据
         * @return   void
         * @author   libaojun
         * @date     2025-05-08
         */
        virtual void initInitialConditions() override;
        /**
         * @brief    初始化运行配置数据
         * @return   void
         * @author   libaojun
         * @date     2025-05-08
         */
        virtual void initRunControl() override;
        /**
         * @brief    初始化工作条件
         * @return   void
         * @author   libaojun
         * @date     2025-05-08
         */
        virtual void initOperatingConditions() override;
    };
}

#endif
