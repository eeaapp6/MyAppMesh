/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorModelShape.h
 * @brief       Adaptor for occ 'model' data.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-31
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORMODELSHAPE_H__
#define __FITKOCCVIEWADAPTORMODELSHAPE_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for occ 'model' data.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-31
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorModelShape : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorModelShape);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        explicit FITKOCCViewAdaptorModelShape() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        ~FITKOCCViewAdaptorModelShape() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        bool update() override;

    };

    // Regist model shape adaptor
    Register2FITKViewAdaptorFactory(GeoModelShapeOCC, Interface::FITKAbsGeoShapeAgent, FITKOCCViewAdaptorModelShape);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORMODELSHAPE_H__