/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorDatumPlane.h
 * @brief       Adaptor for occ 'model' data.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-30
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORDATUMPLANE_H__
#define __FITKOCCVIEWADAPTORDATUMPLANE_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for occ 'model' data.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorDatumPlane : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorDatumPlane);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        explicit FITKOCCViewAdaptorDatumPlane() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        ~FITKOCCViewAdaptorDatumPlane() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        bool update() override;

    };

    // Regist datum plane adaptor
    Register2FITKViewAdaptorFactory(GeoDatumPlaneOCC, Interface::FITKAbsGeoDatum, FITKOCCViewAdaptorDatumPlane, 1);
    Register2FITKViewAdaptorFactory(GeoDatumPlaneOCC, Interface::FITKAbsGeoDatumPlane, FITKOCCViewAdaptorDatumPlane, 2);
    Register2FITKViewAdaptorFactory(GeoDatumPlaneOCC, Interface::FITKAbsGeoReferencePlane, FITKOCCViewAdaptorDatumPlane, 3);
    Register2FITKViewAdaptorFactory(GeoDatumPlaneOCC, Interface::FITKAbsGeoReferenceOffsetPlane, FITKOCCViewAdaptorDatumPlane, 4);
    Register2FITKViewAdaptorFactory(GeoDatumPlaneOCC, Interface::FITKAbsGeoReferenceThreePointsPlane, FITKOCCViewAdaptorDatumPlane, 5);
    Register2FITKViewAdaptorFactory(GeoDatumPlaneOCC, Interface::FITKAbsGeoReferenceEquationPlane, FITKOCCViewAdaptorDatumPlane, 6);
    Register2FITKViewAdaptorFactory(GeoDatumPlaneOCC, Interface::FITKAbsGeoReferencePointAndDirectionPlane, FITKOCCViewAdaptorDatumPlane, 7);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORDATUMPLANE_H__