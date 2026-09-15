/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKMessage.h
 * @brief  信息输出接口声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-18
 *
 */
#ifndef ___FITKMESSAGE_H___
#define ___FITKMESSAGE_H___

#include "FITKAppFrameworkAPI.h"

namespace AppFrame
{
    /**
     * @brief 输出正常信息
     * @param[i]  message        提示信息
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-18
     */
    void FITKAppFrameworkAPI FITKMessageNormal(const QString& message);
    /**
     * @brief 输出警告信息
     * @param[i]  message        提示信息
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-18
     */
    void FITKAppFrameworkAPI FITKMessageWarning(const QString& message);
    /**
     * @brief 输出错误信息
     * @param[i]  message        提示信息
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-18
     */
    void FITKAppFrameworkAPI FITKMessageError(const QString& message);

    /**
     * @brief 输出信息
     * @param[i]  message        提示信息
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-18
     */
    void FITKAppFrameworkAPI FITKMessageInfo(const QString& message);


}



#endif
