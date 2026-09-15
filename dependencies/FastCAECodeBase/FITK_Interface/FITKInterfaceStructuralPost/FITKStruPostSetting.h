/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKStruPostSetting.h
 * @brief 后处理参数设置
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-05-19
 *
 */
#ifndef __FITKSTRYCTUAL_POST_SETTING_H__
#define __FITKSTRYCTUAL_POST_SETTING_H__

#include "FITKInterfaceStructuralPostAPI.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
#include <QString>

namespace Interface
{
    /**
     * @brief 后处理参数
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-19
     */
    class FITKInterfaceStructuralPostAPI FITKStruPostSetting
        : public Core::FITKVarientParams
    {
    public:
        /**
         * @brief Construct a new FITKStruPostSetting object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        explicit FITKStruPostSetting();
        /**
         * @brief Destroy the FITKStruPostSetting object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        virtual ~FITKStruPostSetting() override;

        double getUnDeformFactor();

        void setUnDeformFactor(const double fac);

        QString getDeformVecName();
    };
}


#endif
