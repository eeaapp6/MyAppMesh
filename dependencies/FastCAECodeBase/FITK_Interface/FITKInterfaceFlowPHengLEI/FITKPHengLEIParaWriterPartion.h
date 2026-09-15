/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKPHengLEIParaWriterPartion.h
 * @brief 网格分区配置文件写出 
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-09-12
 * 
 */
#ifndef _FITKPHENGLEI_PARAMWRITER_GRIDPARTION_H___
#define _FITKPHENGLEI_PARAMWRITER_GRIDPARTION_H___

#include "FITKPHengLEIAbsParaWriter.h"

namespace Interface
{
    /**
     * @brief 网格分区参数文件写出
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-09-12
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIParmWriterPartion : public FITKPHengLEIAbsParaWriter
    {
    public:
        /**
         * @brief Construct a new FITKPHengLEIParmWriterPartion object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-12
         */
        explicit  FITKPHengLEIParmWriterPartion() = default;
        /**
         * @brief Destroy the FITKPHengLEIParmWriterPartion object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-12
         */
        virtual ~FITKPHengLEIParmWriterPartion() = default;
        /**
         * @brief 执行写出操作
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-12
         */
        bool writeParamFile() override;

    };

}


#endif