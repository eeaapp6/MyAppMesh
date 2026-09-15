/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKViewAdaptorBoundMesh.h
 * @brief   Adaptor for 'boundrary mesh' data.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-23
 *********************************************************************/

#ifndef __FITKFLUIDVTKVIEWADAPTORBOUNDMESH_H__
#define __FITKFLUIDVTKVIEWADAPTORBOUNDMESH_H__

#include "FITKFluidVTKViewAdaptorBase.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief   Graph object data adaptor for 'boundrary mesh' data.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-23
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKViewAdaptorBoundMesh : public FITKFluidVTKViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKFluidVTKViewAdaptorBoundMesh);

    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        explicit FITKFluidVTKViewAdaptorBoundMesh() = default;

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        ~FITKFluidVTKViewAdaptorBoundMesh() = default;

        /**
         * @brief   Execute the data exchange.[override]
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        bool update() override;

    };

    // Regist boundary mesh adaptor
    Register2FITKViewAdaptorFactory(BoundMesh, Interface::FITKBoundaryMeshVTK, FITKFluidVTKViewAdaptorBoundMesh);
}   // namespace Exchange

#endif // __FITKFLUIDVTKVIEWADAPTORBOUNDMESH_H__