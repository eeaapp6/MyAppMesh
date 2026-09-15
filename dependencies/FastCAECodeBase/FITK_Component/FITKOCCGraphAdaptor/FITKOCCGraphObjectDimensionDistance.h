/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectDimensionDistance.h
 * @brief       Graph object for distance dimension tool.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-09
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTDIMENSIONDISTANCE_H__
#define __FITKOCCGRAPHOBJECTDIMENSIONDISTANCE_H__

#include "FITKOCCGraphObjectDimensionToolBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

#if OCC_VERSION_HEX < 0x070600
class AIS_LengthDimension;
#else
class PrsDim_LengthDimension;
#endif

namespace Exchange
{
    /**
     * @brief       Graph object for distance dimension tool.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-09
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectDimensionDistance : public FITKOCCGraphObjectDimensionToolBase
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectDimensionDistance, FITKOCCGraphObjectDimensionToolBase);
        FITKCLASS(Exchange, FITKOCCGraphObjectDimensionDistance);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-09
         */
        FITKOCCGraphObjectDimensionDistance();

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-09
         */
        ~FITKOCCGraphObjectDimensionDistance();

    private:
        /**
         * @brief       Generate the tool with the given data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-09
         */
        void generateShape();

        /**
         * @brief       Update the geometry list.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-10
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
         * @brief       The interactive tool object for calculating distance.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-09
         */
#if OCC_VERSION_HEX < 0x070600
        AIS_LengthDimension* m_aisTool{ nullptr };
#else
        PrsDim_LengthDimension* m_aisTool{ nullptr };
#endif
        
    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTDIMENSIONDISTANCE_H__
