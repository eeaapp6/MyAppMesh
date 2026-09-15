/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorModelCommand.h
 * @brief       Adaptor for occ 'model' data.( Legacy )
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-06-12
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORMODELCOMMAND_H__
#define __FITKOCCVIEWADAPTORMODELCOMMAND_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for occ 'model' data.( Legacy )
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-12
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorModelCommand : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorModelCommand);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        explicit FITKOCCViewAdaptorModelCommand() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        ~FITKOCCViewAdaptorModelCommand() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        bool update() override;

    };

    // Regist model adaptor
    Register2FITKViewAdaptorFactory(GeoModelCmdOCC, Interface::FITKAbsGeoCommand, FITKOCCViewAdaptorModelCommand);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORMODELCOMMAND_H__