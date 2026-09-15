/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file CADOCCSettings.h
 * @brief 软件运行参数
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-06
 *
 */
#ifndef __STRUCTURAL_SETTINGS_H__
#define __STRUCTURAL_SETTINGS_H__

#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"

/**
 * @brief 软件运行参数
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-06
 */
class CADOCCSettings : public AppFrame::FITKAppSettings
{
public:
    /**
     * @brief Construct a new Structural Settings object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-06
     */
    explicit CADOCCSettings() = default;
    /**
     * @brief Destroy the Structural Settings object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-06
     */
    virtual ~CADOCCSettings() = default;
    /**
     * @brief 读取ini文件
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-06
     */
    void read() override;


};


#endif
