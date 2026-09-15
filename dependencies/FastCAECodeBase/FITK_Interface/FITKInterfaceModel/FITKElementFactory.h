/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKElementFactory.h
 * @brief  单元创建工厂声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-29
 *
 */
#ifndef _FITKELEMENT_FACTORY_H__
#define _FITKELEMENT_FACTORY_H__

#include "FITKInterfaceModelAPI.h"
#include "FITKModelEnum.h"

namespace  Interface
{
    class FITKAbstractElement;
    /**
     * @brief 单元创建简单工厂，不可实例化
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-29
     */
    class FITKInterfaceModelAPI FITKElementFactory
    {
    public:
        /**
         * @brief 根据类型创建单元对象
         * @param[i]  t           单元类型
         * @return FITKAbstractElement*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        static FITKAbstractElement* createElement(FITKModelEnum::FITKEleType t);


    private:
        /**
         * @brief Construct a new FITKElementFactory object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        FITKElementFactory() = default;
        FITKElementFactory(const FITKElementFactory &) = delete;
        /**
         * @brief Destroy the FITKElementFactory object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        ~FITKElementFactory() = default;
    };
}
#endif
