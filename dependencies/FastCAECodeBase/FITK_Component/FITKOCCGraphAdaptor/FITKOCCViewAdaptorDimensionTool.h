/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorDimensionTool.h
 * @brief       Adaptor for all kinds of occ measure tool graph object.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-12
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORDIMENSIONTOOL_H__
#define __FITKOCCVIEWADAPTORDIMENSIONTOOL_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object adaptor all kinds of occ measure tool graph object.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-12
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorDimensionTool : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorDimensionTool);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        explicit FITKOCCViewAdaptorDimensionTool() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        ~FITKOCCViewAdaptorDimensionTool() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        bool update() override;

    };

    // Regist dimension tools adaptor
    Register2FITKViewAdaptorFactory(MeasureToolOCC, DimensionTools, FITKOCCViewAdaptorDimensionTool);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORDIMENSIONTOOL_H__