/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractIOAdaptor.h
 * @brief 数据IO适配器基类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-03
 *
 */
#ifndef __FITKABSTRACT_IO_ADAPTOR_H___
#define __FITKABSTRACT_IO_ADAPTOR_H___


#include "FITKAbstractAdaptor.h"

namespace Adaptor
{
    /**
     * @brief 数据读写IO适配器声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-03
     */
    class FITKAdaptorAPI FITKAbstractIOAdaptor : public FITKAbstractAdaptor
    {
        FITKCLASS(Adaptor, FITKAbstractIOAdaptor);
    public:
        /**
         * @brief Construct a new FITKAbstractIOAdaptor object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        explicit FITKAbstractIOAdaptor() = default;
        /**
         * @brief Destroy the FITKAbstractIOAdaptor object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        virtual ~FITKAbstractIOAdaptor() = 0;
        /**
         * @brief 获取对应的数据类名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        virtual QString getAdaptorClass() override;
        /**
         * @brief 获取适配器类型
         * @return FITKAdaptorType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        virtual FITKAdaptorType getAdaptorType() override;
        /**
         * @brief 适配器读数据
         * @return bool
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        virtual bool adaptR();
        /**
         * @brief 适配器写数据
         * @return bool
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        virtual bool adaptW();

    };

}

#endif