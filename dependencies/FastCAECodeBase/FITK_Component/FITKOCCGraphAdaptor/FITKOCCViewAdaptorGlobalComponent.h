/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorGlobalComponent.h
 * @brief       Adaptor for occ 'global geometry component' data.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-10-31
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORGLOBALCOMPONENT_H__
#define __FITKOCCVIEWADAPTORGLOBALCOMPONENT_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for occ 'global geometry component' data.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-10-31
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorGlobalComponent : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorGlobalComponent);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-31
         */
        explicit FITKOCCViewAdaptorGlobalComponent() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-31
         */
        ~FITKOCCViewAdaptorGlobalComponent() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-31
         */
        bool update() override;

    };

    // Regist global component adaptor
    Register2FITKViewAdaptorFactory(GlobalGeoComp, Interface::FITKGlobalGeoComponent, FITKOCCViewAdaptorGlobalComponent);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORGLOBALCOMPONENT_H__