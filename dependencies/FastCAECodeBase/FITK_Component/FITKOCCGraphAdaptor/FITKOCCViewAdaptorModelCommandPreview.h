/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorModelCommandPreview.h
 * @brief       Adaptor for previewing occ 'model' data.( Legacy )
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-28
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORMODELCOMMANDPREVIEW_H__
#define __FITKOCCVIEWADAPTORMODELCOMMANDPREVIEW_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for previewing occ 'model' data.( Legacy )
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-28
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorModelCommandPreview : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorModelCommandPreview);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-28
         */
        explicit FITKOCCViewAdaptorModelCommandPreview() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-28
         */
        ~FITKOCCViewAdaptorModelCommandPreview() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-28
         */
        bool update() override;

    };

    // Regist model adaptor
    Register2FITKViewAdaptorFactory(GeoModelCmdPreviewOCC, Interface::FITKAbsGeoCommand, FITKOCCViewAdaptorModelCommandPreview);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORMODELCOMMANDPREVIEW_H__