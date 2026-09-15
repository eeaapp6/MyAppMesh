/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKMeshParameterLine.h
 * @brief       线单元划分参数。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-11-04
 *********************************************************************/

#ifndef __FITKMeshParameterLine_H__
#define __FITKMeshParameterLine_H__

#include "FITKInterfaceMeshGenAPI.h"
#include "FITKMeshParameterBase.h"

namespace Interface
{
    /**
     * @brief       线单元划分参数。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-11-04
     */
    class FITKInterfaceMeshGenAPI FITKMeshParameterLine : public FITKMeshParameterBase
    {
        FITKCLASS(Interface, FITKMeshParameterLine);
        Q_OBJECT;
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        explicit FITKMeshParameterLine() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        virtual ~FITKMeshParameterLine() = default;
        /**
         * @brief    获取网格参数类型
         * @return   FITKMeshParamType          网格参数类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-25
         */
        virtual FITKMeshParamType getMeshParamType() override;
    };
}

#endif // !__FITKMeshParameterLine_H__
