/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFSolutionSolver.h
 * @brief  solution Solver数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-22
 *********************************************************************/
#ifndef _FITK_OF_SOLUTION_SOLVER__H___
#define _FITK_OF_SOLUTION_SOLVER__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  solution Solver数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFAbsSolutionSolver : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKOFAbsSolutionSolver();
        virtual ~FITKOFAbsSolutionSolver();

        /**
         * @brief    获取参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        Core::FITKParameter* getSolverSolutionPara();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initSolverSolutionParam() = 0;

    protected:
        /**
         * @brief  求解器的参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        Core::FITKParameter* _solverSolutionParams{};
    };

    /**
     * @brief  GAMG solution Solver数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFSolutionSolverGAMG : public FITKOFAbsSolutionSolver
    {
    public:
        explicit FITKOFSolutionSolverGAMG();
        virtual ~FITKOFSolutionSolverGAMG();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initSolverSolutionParam() override;
    };

    /**
     * @brief  PCG solution Solver数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFSolutionSolverPCG : public FITKOFAbsSolutionSolver
    {
    public:
        explicit FITKOFSolutionSolverPCG();
        virtual ~FITKOFSolutionSolverPCG();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initSolverSolutionParam() override;
    };

    /**
     * @brief  Smooth Solver solution Solver数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFSolutionSolverSmoothSolver : public FITKOFAbsSolutionSolver
    {
    public:
        explicit FITKOFSolutionSolverSmoothSolver();
        virtual ~FITKOFSolutionSolverSmoothSolver();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initSolverSolutionParam() override;
    };

    /**
     * @brief  PBiCG Stabilized solution Solver数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFSolutionSolverPBiCGStabilized : public FITKOFAbsSolutionSolver
    {
    public:
        explicit FITKOFSolutionSolverPBiCGStabilized();
        virtual ~FITKOFSolutionSolverPBiCGStabilized();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initSolverSolutionParam() override;
    };

    /**
    * @brief  PBiCG solution Solver数据基类
    * @author liuzhonghua (liuzhonghuaszch@163.com)
    * @date   2024-08-22
    */
    class FITKInterfaceFlowOFAPI FITKOFSolutionSolverPBiCG : public FITKOFAbsSolutionSolver
    {
    public:
        explicit FITKOFSolutionSolverPBiCG();
        virtual ~FITKOFSolutionSolverPBiCG();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initSolverSolutionParam() override;
    };
}


#endif
