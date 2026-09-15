/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIAbsSpatialDiscretisation.h
 * @brief  空间离散类型基类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef __FITK_ABSTRACT_PHengLEI_AbsSpatialDiscretisation_H___
#define __FITK_ABSTRACT_PHengLEI_AbsSpatialDiscretisation_H___

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKPHengLEIAbsSolver.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  空间离散型基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIAbsSpatialDiscretisation : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKPHengLEIAbsSpatialDiscretisation(FITKPHengLEIAbsSolver::PHengLEISolverType type);
        virtual ~FITKPHengLEIAbsSpatialDiscretisation();
        /**
         * @brief    获取空间离散类型数据参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        Core::FITKParameter* getSchemeTypeParams();
    protected:
        /**
         * @brief    初始化函数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void init();
        /**
         * @brief    初始空间离散类型参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initSchemeTypeParam() = 0;
    protected:
        /**
         * @brief  空间离散类型参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        Core::FITKParameter* _schemeTypeParams{};
		/**
		 * @brief 求解器类型
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIAbsSolver::PHengLEISolverType _solverType{};
    };
}


#endif
