/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKStruPostSetting.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"

namespace Interface
{
    /**
     * @brief Construct a new FITKStruPostSetting::FITKStruPostSetting object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-19
     */
    FITKStruPostSetting::FITKStruPostSetting()
    {

    }
    /**
     * @brief Destroy the FITKStruPostSetting::FITKStruPostSetting object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-19
     */
    FITKStruPostSetting::~FITKStruPostSetting()
    {

    }

    double FITKStruPostSetting::getUnDeformFactor()
    {
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        if (appSettings == nullptr) return 0;

        return appSettings->getValue<double>("StructualPost/UnDeformFactor");
    }

    void FITKStruPostSetting::setUnDeformFactor(const double fac)
    {
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        if (appSettings == nullptr) return;

        appSettings->setValue("StructualPost/UnDeformFactor", fac);
    }

    QString FITKStruPostSetting::getDeformVecName()
    {
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        if (appSettings == nullptr) return 0;

        return appSettings->getValue<QString>("StructualPost/DeformVecName");
    }

}



