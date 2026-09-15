/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKRunTimeSetting.h
 * @brief 软件运行时参数设定
 * @author libaojun (libaojunqd@Foxmail.com)
 * @date 2024-04-10
 *
 */
#ifndef __FITKRUNTIME__SETTING_H__
#define __FITKRUNTIME__SETTING_H__

#include "FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
namespace AppFrame
{
    /**
     * @brief 软件运行时参数设定
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-04-10
     */ 
    class  FITKAppFrameworkAPI FITKRunTimeSetting : public Core::FITKVarientParams
    {
    public:
        /**
         * @brief Construct a new FITKRunTimeSetting object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        explicit FITKRunTimeSetting() = default;
        /**
         * @brief Destroy the FITKRunTimeSetting object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        virtual ~FITKRunTimeSetting() = default;
      

    };

}



#endif
