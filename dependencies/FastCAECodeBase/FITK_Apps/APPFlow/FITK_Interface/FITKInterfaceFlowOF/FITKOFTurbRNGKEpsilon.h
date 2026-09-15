/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFTurbRNGKEpsilon.h
 * @brief  RNG k-Epsilon 湍流数据接口声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITKOF_TURB_RNGKEpsilon_H___
#define _FITKOF_TURB_RNGKEpsilon_H___
 
#include "FITKAbstractOFTurbulence.h"

namespace Interface
{
    /**
     * @brief  RNG k-Epsilon 湍流数据对象
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKInterfaceFlowOFAPI FITKOFTurbRNGKEpsilon : public FITKAbstractOFTurbulence
    {
    public:
        explicit FITKOFTurbRNGKEpsilon();
        virtual ~FITKOFTurbRNGKEpsilon() = default;
        
        /**
         * @brief    获取湍流类型
         * @return   FITKOFSolverTypeEnum::FITKOFTurbulenceType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual FITKOFSolverTypeEnum::FITKOFTurbulenceType getTurbulenceType() override;
    protected:
        /**
         * @brief    初始化湍流模型参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initTurbParam() override;
        /**
         * @brief    初始化Delta数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initDelta() override;
        /**
         * @brief    初始湍流模型下特定的离散数据
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
         * @brief    初始化 Initial Conditions数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        virtual void initInitialConditions() override;

        virtual void initBoundary() override;


    private:
    };
}


#endif
