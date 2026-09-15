/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorSketchShape.h
 * @brief       Adaptor for occ sketch 2d data.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-31
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORSKETCHSHAPE_H__
#define __FITKOCCVIEWADAPTORSKETCHSHAPE_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for occ sketch 2d data.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-31
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorSketchShape : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorSketchShape);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        explicit FITKOCCViewAdaptorSketchShape() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        ~FITKOCCViewAdaptorSketchShape() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        bool update() override;

    };

    // Regist sketch 2d adaptor
    Register2FITKViewAdaptorFactory(GeoSketchShapeOCC, Interface::FITKAbsGeoShapeAgent, FITKOCCViewAdaptorSketchShape);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORSKETCHSHAPE_H__