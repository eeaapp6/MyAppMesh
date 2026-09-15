/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorTopoIndexLabels.h
 * @brief       Adaptor for previewing the labels of shape elements.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-02
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORTOPOINDEXLABELS_H__
#define __FITKOCCVIEWADAPTORTOPOINDEXLABELS_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for previewing the labels of shape elements.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-02
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorTopoIndexLabels : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorTopoIndexLabels);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-02
         */
        explicit FITKOCCViewAdaptorTopoIndexLabels() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-02
         */
        ~FITKOCCViewAdaptorTopoIndexLabels() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-02
         */
        bool update() override;

    };

    // Regist labels adaptor
    Register2FITKViewAdaptorFactory(TopoIndexLabelsOCC, Interface::FITKAbsVirtualTopo, FITKOCCViewAdaptorTopoIndexLabels);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORTOPOINDEXLABELS_H__