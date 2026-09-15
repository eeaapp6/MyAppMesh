/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCViewAdaptorMesh.h
 * @brief       Adaptor for occ 'mesh' data.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-06-13
 *********************************************************************/

#ifndef __FITKOCCVIEWADAPTORMESH_H__
#define __FITKOCCVIEWADAPTORMESH_H__

#include "FITKOCCViewAdaptorBase.h"

#include "FITKOCCGraphAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief       Graph object data adaptor for occ 'mesh' data.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCViewAdaptorMesh : public FITKOCCViewAdaptorBase
    {
        // Regist
        FITKCLASS(Exchange, FITKOCCViewAdaptorMesh);

    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        explicit FITKOCCViewAdaptorMesh() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        ~FITKOCCViewAdaptorMesh() = default;

        /**
         * @brief       Execute the data exchange.[override]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        bool update() override;

    };

    // Regist unstructured mesh adaptor
    Register2FITKViewAdaptorFactory(MeshOCC, OCC::FITKUnstructuredMesh, FITKOCCViewAdaptorMesh, 1);

    // Regist structured mesh adaptor
    // Register2FITKViewAdaptorFactory(MeshOCC, Interface::FITKStructuredMesh, FITKOCCViewAdaptorMesh, 2);
}   // namespace Exchange

#endif // __FITKOCCVIEWADAPTORMESH_H__