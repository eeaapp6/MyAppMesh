/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorDatumLine.h
 * @brief       Adaptor for occ 'model' data.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-30
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORDATUMLINE_H__
#define __FITKOCCVIEWADAPTORDATUMLINE_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for occ 'model' data.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorDatumLine : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorDatumLine);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        explicit FITKOCCViewAdaptorDatumLine() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        ~FITKOCCViewAdaptorDatumLine() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        bool update() override;

    };

    // Regist datum line adaptor
    Register2FITKViewAdaptorFactory(GeoDatumLineOCC, Interface::FITKAbsGeoDatum, FITKOCCViewAdaptorDatumLine, 1);
    Register2FITKViewAdaptorFactory(GeoDatumLineOCC, Interface::FITKAbsGeoDatumLine, FITKOCCViewAdaptorDatumLine, 2);
    Register2FITKViewAdaptorFactory(GeoDatumLineOCC, Interface::FITKAbsGeoReferenceLine, FITKOCCViewAdaptorDatumLine, 3);
    Register2FITKViewAdaptorFactory(GeoDatumLineOCC, Interface::FITKAbsGeoReferenceSegment, FITKOCCViewAdaptorDatumLine, 4);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORDATUMLINE_H__