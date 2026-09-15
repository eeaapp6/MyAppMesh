/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorDatumPoint.h
 * @brief       Adaptor for occ 'model' data.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-30
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORDATUMPOINT_H__
#define __FITKOCCVIEWADAPTORDATUMPOINT_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for occ 'model' data.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorDatumPoint : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorDatumPoint);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        explicit FITKOCCViewAdaptorDatumPoint() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        ~FITKOCCViewAdaptorDatumPoint() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        bool update() override;

    };

    // Regist datum point adaptor
    Register2FITKViewAdaptorFactory(GeoDatumPointOCC, Interface::FITKAbsGeoDatum, FITKOCCViewAdaptorDatumPoint, 1);
    Register2FITKViewAdaptorFactory(GeoDatumPointOCC, Interface::FITKAbsGeoDatumPoint, FITKOCCViewAdaptorDatumPoint, 2);
    Register2FITKViewAdaptorFactory(GeoDatumPointOCC, Interface::FITKAbsGeoReferencePoint, FITKOCCViewAdaptorDatumPoint, 3);
    Register2FITKViewAdaptorFactory(GeoDatumPointOCC, Interface::FITKAbsGeoReferenceCurveRadio, FITKOCCViewAdaptorDatumPoint, 4);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORDATUMPOINT_H__