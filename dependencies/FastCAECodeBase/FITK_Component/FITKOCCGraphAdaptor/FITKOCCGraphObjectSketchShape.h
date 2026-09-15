/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectSketchShape.h
 * @brief       Graph object for sketch data object and OCC graph widget.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-31
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTSKETCHSHAPE_H__
#define __FITKOCCGRAPHOBJECTSKETCHSHAPE_H__

#include "FITKOCCGraphObjectModelShape.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

namespace Exchange
{
    /**
     * @brief       Graph object for sketch data object and OCC graph widget.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-31
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectSketchShape : public FITKOCCGraphObjectModelShape
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectSketchShape, FITKOCCGraphObjectModelShape);
        FITKCLASS(Exchange, FITKOCCGraphObjectSketchShape);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   sketchData: The sketch data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCGraphObjectSketchShape(Interface::FITKAbsGeoShapeAgent* sketchData);

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        ~FITKOCCGraphObjectSketchShape() = default;

        /**
         * @brief       Set the select mode for this graph object's interactive objects.[override]
         * @param[in]   mode: The select mode
         * @param[in]   force: Force add the select mode[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void addSelectMode(CommonOCC::SelectMode mode, bool force = false) override;

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCGraphObjectSketchShape() = default;

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTSKETCHSHAPE_H__
