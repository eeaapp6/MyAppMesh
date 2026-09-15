/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKViewAdaptorRegionMesh.h
 * @brief   Adaptor for previewing 'region mesh' data.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-25
 *********************************************************************/

#ifndef __FITKFLUIDVTKVIEWADAPTORREGIONMESH_H__
#define __FITKFLUIDVTKVIEWADAPTORREGIONMESH_H__

#include "FITKFluidVTKViewAdaptorBase.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief   Graph object data adaptor for previewing 'region mesh' data.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-25
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKViewAdaptorRegionMesh : public FITKFluidVTKViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKFluidVTKViewAdaptorRegionMesh);

    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        explicit FITKFluidVTKViewAdaptorRegionMesh() = default;

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        ~FITKFluidVTKViewAdaptorRegionMesh() = default;

        /**
         * @brief   Execute the data exchange.[override]
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        bool update() override;

    };

    // Regist region mesh adaptor
    Register2FITKViewAdaptorFactory(RegionMeshPreview, Interface::FITKAbstractRegionMeshSize, FITKFluidVTKViewAdaptorRegionMesh);
}   // namespace Exchange

#endif // __FITKFLUIDVTKVIEWADAPTORREGIONMESH_H__