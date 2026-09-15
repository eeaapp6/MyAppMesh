/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAbstractOFTurbulence.h
 * @brief  湍流模型数据基类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef __FITK_ABSTRACT_TURB_OF__H___
#define __FITK_ABSTRACT_TURB_OF__H___

#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKOFEnum.hpp"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    class FITKOFDiscretization;
    class FITKOFSolution;
    class FITKOFBoundary;

    class FITKOFInitialConditions;
    /**
     * @brief  湍流模型数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKInterfaceFlowOFAPI FITKAbstractOFTurbulence: public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKAbstractOFTurbulence();
        virtual ~FITKAbstractOFTurbulence();

        /**
         * @brief    获取湍流类型
         * @return   FITKOFSolverTypeEnum::FITKOFTurbulenceType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual FITKOFSolverTypeEnum::FITKOFTurbulenceType getTurbulenceType() = 0;
        /**
         * @brief    获取湍流数据参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        Core::FITKParameter* getTurbulenceParams();
        /**
         * @brief    获取Delta数据参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        Core::FITKParameter* getDeltaParams();
        /**
         * @brief    获取离散数据
         * @return   FITKOFDiscretization *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFDiscretization* getDiscretization();
        /**
         * @brief    获取solution数据
         * @return   FITKOFSolution *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        FITKOFSolution* getSolution();
        /**
         * @brief    获取Initial Conditions数据
         * @return   FITKOFInitialConditions *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        FITKOFInitialConditions* getInitialConditions();
        /**
         * @brief    获取边界
         * @param[i] bType 边界类型
         * @return   FITKOFBoundary *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-26
         */
        FITKOFBoundary* getBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType bType);

    protected:
        /**
         * @brief    初始化函数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void init();
        /**
         * @brief    初始化湍流模型参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initTurbParam() = 0;
        /**
         * @brief    初始化Delta数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initDelta() = 0;
        /**
         * @brief    初始湍流模型下特定的离散数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initDiscretization() = 0;
        /**
         * @brief    初始化solution数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initSolution() = 0;
        /**
         * @brief    初始化 Initial Conditions数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        virtual void initInitialConditions() = 0;
        /**
         * @brief    初始化 Boundary 数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-26
         */
        virtual void initBoundary() = 0;

    protected:
        /**
         * @brief  湍流参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        Core::FITKParameter* _turbParams{};
        /**
         * @brief  Delta参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        Core::FITKParameter* _delta{};
        /**
         * @brief  湍流模型下特定的离散数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        FITKOFDiscretization* _discretization{};
        /**
         * @brief  solution数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-23
         */
        FITKOFSolution* _solution{};
        /**
         * @brief  边界数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-26
         */
        QList<FITKOFBoundary*> _boundaryInfos{};
        /**
         * @brief  Initial Conditions数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-23
         */
        FITKOFInitialConditions* _initialCond{};
    };
}


#endif
