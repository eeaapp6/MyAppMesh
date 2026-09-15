/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorSketchCommand.h
 * @brief       Adaptor for occ sketch 2d data.( Legacy )
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-04
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORSKETCHCOMMAND_H__
#define __FITKOCCVIEWADAPTORSKETCHCOMMAND_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for occ sketch 2d data.( Legacy )
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorSketchCommand : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorSketchCommand);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        explicit FITKOCCViewAdaptorSketchCommand() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        ~FITKOCCViewAdaptorSketchCommand() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        bool update() override;

    };

    // Regist sketch 2d adaptor
    Register2FITKViewAdaptorFactory(GeoSketchCmdOCC, Interface::FITKAbsGeoSketch2D, FITKOCCViewAdaptorSketchCommand);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORSKETCHCOMMAND_H__