/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKComponentFactory.h
 * @brief 定义抽象组件工厂
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-04
 *
 */
#ifndef ___FITKCOMPONENT_FACTORY_H___
#define ___FITKCOMPONENT_FACTORY_H___

#include "FITKAppFrameworkAPI.h"
#include <QList>

namespace AppFrame
{
    class FITKComponentInterface;
    /**
     * @brief 抽象组件工厂定义
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-04
     */
    class FITKAppFrameworkAPI FITKComponentFactory
    {
    public:
        /**
         * @brief Construct a new FITKComponentFactory object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        FITKComponentFactory() = default;
        /**
         * @brief Destroy the FITKComponentFactory object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual ~FITKComponentFactory() = default;
        /**
         * @brief 执行创建操作
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        void create();

    private:
        /**
         * @brief 创建组件对象
         * @return QList<FITKComponentInterface*>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual QList<FITKComponentInterface*> createComponents() = 0;
    };

}
#endif
