/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorTopoDirection.h
 * @brief       Adaptor for previewing the direction of shape.
 * 
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-04
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORTOPODIRECTION_H__
#define __FITKOCCVIEWADAPTORTOPODIRECTION_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for previewing the direction of shape.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorTopoDirection : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorTopoDirection);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        explicit FITKOCCViewAdaptorTopoDirection() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        ~FITKOCCViewAdaptorTopoDirection() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        bool update() override;

    };

    // Regist topo direction adaptor
    Register2FITKViewAdaptorFactory(TopoDirectionOCC, Interface::FITKAbsVirtualTopo, FITKOCCViewAdaptorTopoDirection);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORTOPODIRECTION_H__