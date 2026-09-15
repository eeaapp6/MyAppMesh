/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIInFlowTypeNondimensional.h
 * @brief  来流类型Nondimensional数据
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITKOF_PHengLEIInFlowTypeNondimensional_H___
#define _FITKOF_PHengLEIInFlowTypeNondimensional_H___
 
#include "FITKPHengLEIAbstractInFlowType.h"

namespace Interface
{
    /**
     * @brief  来流类型Nondimensional类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIInFlowTypeNondimensional : public FITKPHengLEIAbstractInFlowType
    {
    public:
        explicit FITKPHengLEIInFlowTypeNondimensional(FITKPHengLEIAbsSolver::PHengLEISolverType type);
        virtual ~FITKPHengLEIInFlowTypeNondimensional() = default;
    protected:
        /**
         * @brief    初始化来流类型Nondimensional参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initInFlowTypeParam() override;
    private:
    };
}


#endif
