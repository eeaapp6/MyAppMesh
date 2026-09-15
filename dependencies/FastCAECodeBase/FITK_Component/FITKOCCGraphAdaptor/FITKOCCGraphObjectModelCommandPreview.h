/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectModelCommandPreview.h
 * @brief       Graph object for previewing OCC model.( Legacy )
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-28
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTMODELCOMMANDPREVIEW_H__
#define __FITKOCCGRAPHOBJECTMODELCOMMANDPREVIEW_H__

#include "FITKOCCGraphObjectModelPreviewBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

// Forward declaration
namespace Interface
{
    class FITKAbsGeoCommand;
}

namespace Exchange
{
    /**
     * @brief       Graph object for previewing OCC model.( Legacy )
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-28
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectModelCommandPreview : public FITKOCCGraphObjectModelPreviewBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectModelCommandPreview, FITKOCCGraphObjectModelPreviewBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectModelCommandPreview);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   modelData: The model data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-28
         */
        FITKOCCGraphObjectModelCommandPreview(Interface::FITKAbsGeoCommand* modelData);

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-28
         */
        ~FITKOCCGraphObjectModelCommandPreview();

        /**
         * @brief       Update the graph object.[override]
         * @param[in]   forceUpdate: Force update[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-28
         */
        void update(bool forceUpdate = false) override;

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-28
         */
        FITKOCCGraphObjectModelCommandPreview() = default;

    private:
        /**
         * @brief       Initialize.
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-28
         */
        bool initShapeData();

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTMODELCOMMANDPREVIEW_H__
