/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectModelShapePreview.h
 * @brief       Graph object for previewing OCC model.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-31
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTMODELSHAPEPREVIEW_H__
#define __FITKOCCGRAPHOBJECTMODELSHAPEPREVIEW_H__

#include "FITKOCCGraphObjectModelPreviewBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

// Forward declaration
namespace Interface
{
    class FITKAbsGeoShapeAgent;
}

namespace Exchange
{
    /**
     * @brief       Graph object for previewing OCC model.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-31
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectModelShapePreview : public FITKOCCGraphObjectModelPreviewBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectModelShapePreview, FITKOCCGraphObjectModelPreviewBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectModelShapePreview);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   modelData: The model data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCGraphObjectModelShapePreview(Interface::FITKAbsGeoShapeAgent* modelData);

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        ~FITKOCCGraphObjectModelShapePreview() = default;

        /**
         * @brief       Update the graph object.[override]
         * @param[in]   forceUpdate: Force update[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void update(bool forceUpdate = false) override;

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCGraphObjectModelShapePreview() = default;

    private:
        /**
         * @brief       Initialize.
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        bool initShapeData();

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTMODELSHAPEPREVIEW_H__
