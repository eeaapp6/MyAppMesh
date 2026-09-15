/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractPythonRegister.h
 * @brief 抽象Python接口注册器声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-18
 *
 */
#ifndef __FITKPYTHON_INTERFACE_REGISTER_H__
#define __FITKPYTHON_INTERFACE_REGISTER_H__

#include "FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"

namespace AppFrame
{
    /**
     * @brief 抽象Python接口注册器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-18
     */
    class FITKAppFrameworkAPI FITKAbstractPythonRegister
        : public Core::FITKAbstractObject
    {
    public:
        /**
         * @brief Construct a new FITKAbstractPythonRegister object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-18
         */
        explicit FITKAbstractPythonRegister() = default;
        /**
         * @brief Destroy the FITKAbstractPythonRegister object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-18
         */
        virtual ~FITKAbstractPythonRegister() = 0;
        /**
         * @brief 注册Python装饰器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-18
         */
        virtual void  registWapper() = 0;
    };
}

#endif
