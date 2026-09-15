/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIInFlowTypeFlight.h
 * @brief  来流类型Flight数据
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITKOF_PHengLEIInFlowTypeFlight_H___
#define _FITKOF_PHengLEIInFlowTypeFlight_H___
 
#include "FITKPHengLEIAbstractInFlowType.h"

namespace Interface
{
    /**
     * @brief  来流类型Flight类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIInFlowTypeFlight : public FITKPHengLEIAbstractInFlowType
    {
    public:
        explicit FITKPHengLEIInFlowTypeFlight(FITKPHengLEIAbsSolver::PHengLEISolverType type);
        virtual ~FITKPHengLEIInFlowTypeFlight() = default;
    protected:
        /**
         * @brief    初始化来流类型Flight参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initInFlowTypeParam() override;
    private:
    };
}


#endif
