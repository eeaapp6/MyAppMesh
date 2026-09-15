/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKJobManager.h
 * @brief 作业管理器声明
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-04-01
 * 
 */
#ifndef _FITKJobManager_H_
#define _FITKJobManager_H_
#include "FITKAbaqusDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace AbaqusData
{
    class FITKJob;
    /**
     * @brief 作业管理器声明
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-04-01
     */
    class FITKAbaqusDataAPI FITKJobManager : public Core::FITKAbstractDataManager< FITKJob >
    {
    public:
        /**
         * @brief Construct a new FITKJobManager object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        FITKJobManager();
        /**
         * @brief Destroy the FITKJobManager object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        ~FITKJobManager();
    };
};  // namespace Document
#endif
