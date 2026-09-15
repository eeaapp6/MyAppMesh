/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorTopoShape.h
 * @brief       Adaptor for previewing the shape of the virtual topo.
 * 
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-06
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORTOPOSHAPE_H__
#define __FITKOCCVIEWADAPTORTOPOSHAPE_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for previewing the shape of the virtual topo.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-06
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorTopoShape : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorTopoShape);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        explicit FITKOCCViewAdaptorTopoShape() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        ~FITKOCCViewAdaptorTopoShape() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        bool update() override;

    };

    // Regist virtual topo shape adaptor
    Register2FITKViewAdaptorFactory(TopoShapeOCC, Interface::FITKAbsVirtualTopo, FITKOCCViewAdaptorTopoShape);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORTOPOSHAPE_H__