/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorPickTool.h
 * @brief       Adaptor for all kinds of occ pick tool graph object.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-13
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORPICKTOOL_H__
#define __FITKOCCVIEWADAPTORPICKTOOL_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object adaptor all kinds of occ pick tool graph object.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-13
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorPickTool : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorPickTool);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        explicit FITKOCCViewAdaptorPickTool() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        ~FITKOCCViewAdaptorPickTool() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        bool update() override;

    };

    // Regist Pick tools adaptor
    Register2FITKViewAdaptorFactory(PickToolOCC, PickTools, FITKOCCViewAdaptorPickTool);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORPICKTOOL_H__