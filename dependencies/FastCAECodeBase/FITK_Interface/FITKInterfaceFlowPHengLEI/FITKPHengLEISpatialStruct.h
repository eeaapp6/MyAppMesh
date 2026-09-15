/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEISpatialStruct.h
 * @brief  空间离散类型Struct数据
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITKOF_PHengLEISpatialStruct_H___
#define _FITKOF_PHengLEISpatialStruct_H___
 
#include "FITKPHengLEIAbsSpatialDiscretisation.h"

namespace Interface
{
    /**
     * @brief  空间离散类型Struct类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI FITKPHengLEISpatialStruct : public FITKPHengLEIAbsSpatialDiscretisation
    {
    public:
        explicit FITKPHengLEISpatialStruct(FITKPHengLEIAbsSolver::PHengLEISolverType type);
        virtual ~FITKPHengLEISpatialStruct() = default;
    protected:
        /**
         * @brief    初始化空间离散类型Struct参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initSchemeTypeParam() override;
    private:
    };
}


#endif
