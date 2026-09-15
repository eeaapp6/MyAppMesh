/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectDimensionArea.h
 * @brief       Graph object for area dimension tool.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-12
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTDIMENSIONAREA_H__
#define __FITKOCCGRAPHOBJECTDIMENSIONAREA_H__

#include "FITKOCCGraphObjectDimensionToolBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

class AIS_Shape;
class AIS_TextLabel;

namespace Exchange
{
    /**
     * @brief       Graph object for area dimension tool.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-12
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectDimensionArea : public FITKOCCGraphObjectDimensionToolBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectDimensionArea, FITKOCCGraphObjectDimensionToolBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectDimensionArea);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        FITKOCCGraphObjectDimensionArea();

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        ~FITKOCCGraphObjectDimensionArea();

    private:
        /**
         * @brief       Generate the tool with the given data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        void generateShape();

        /**
         * @brief       Update the geometry list.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        bool updateMeasureGeometrys() override;

        /**
         * @brief       Resize the measure tool.[override]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        void rescale() override;

    private:
        /**
         * @brief       The area text label of the shape.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        AIS_TextLabel* m_aisLabel{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTDIMENSIONAREA_H__
