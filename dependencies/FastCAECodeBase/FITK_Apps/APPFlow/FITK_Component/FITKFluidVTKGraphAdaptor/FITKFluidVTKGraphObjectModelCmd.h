/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKGraphObjectModelCmd.h
 * @brief   Shape graph object 3D model for VTK graph widget.( Legacy )

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-05
 *********************************************************************/

#ifndef __FITKFLUIDVTKGRAPHOBJECTMODELCMD_H__
#define __FITKFLUIDVTKGRAPHOBJECTMODELCMD_H__

#include "FITKFluidVTKGraphObjectModelBase.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

// Forward declaration
namespace Interface
{
    class FITKAbsGeoCommand;
}

namespace Exchange
{
    /**
     * @brief   Shape graph object 3D model for VTK graph widget.( Legacy )
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-05
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKGraphObjectModelCmd : public FITKFluidVTKGraphObjectModelBase
    {
        // Regist
        FITKGraphObjectRegist(FITKFluidVTKGraphObjectModelCmd, FITKFluidVTKGraphObjectModelBase);
        FITKCLASS(Exchange, FITKFluidVTKGraphObjectModelCmd);

    public:
        /**
         * @brief   Constructor.
         * @param   modelData: The shape model data object need to exchange
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        FITKFluidVTKGraphObjectModelCmd(Interface::FITKAbsGeoCommand* modelData);

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        ~FITKFluidVTKGraphObjectModelCmd() = default;

        /**
         * @brief   Update all graph actors.[override]
         * @param   forceUpdate: Force update[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-24
         */
        void update(bool forceUpdate = false) override;

    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKGRAPHOBJECTMODELCMD_H__
