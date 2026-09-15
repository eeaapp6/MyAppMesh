/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorModelShapePreview.h
 * @brief       Adaptor for previewing occ 'model' data.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-31
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORMODELSHAPEPREVIEW_H__
#define __FITKOCCVIEWADAPTORMODELSHAPEPREVIEW_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for previewing occ 'model' data.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-31
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorModelShapePreview : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorModelShapePreview);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        explicit FITKOCCViewAdaptorModelShapePreview() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        ~FITKOCCViewAdaptorModelShapePreview() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        bool update() override;

    };

    // Regist model shape adaptor
    Register2FITKViewAdaptorFactory(GeoModelShapePreviewOCC, Interface::FITKAbsGeoShapeAgent, FITKOCCViewAdaptorModelShapePreview);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORMODELSHAPEPREVIEW_H__