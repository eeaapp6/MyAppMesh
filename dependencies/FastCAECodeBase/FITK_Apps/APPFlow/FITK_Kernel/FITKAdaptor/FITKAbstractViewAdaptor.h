/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractViewAdaptor.h
 * @brief 显示对象适配器抽象类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-07
 *
 */
#ifndef __FITKABSTRACT_VIEW_ADAPTOR_H___
#define __FITKABSTRACT_VIEW_ADAPTOR_H___

#include "FITKAbstractAdaptor.h"


namespace Adaptor
{
    /**
     * @brief 显示对象适配器抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKAdaptorAPI FITKAbstractViewAdaptor : public FITKAbstractAdaptor
    {
        FITKCLASS(Adaptor, FITKAbstractViewAdaptor);
    public:
        /**
         * @brief Construct a new FITKAbstractViewAdaptor object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        explicit FITKAbstractViewAdaptor() = default;
        /**
         * @brief Destroy the FITKAbstractViewAdaptor object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual ~FITKAbstractViewAdaptor() = 0;
        /**
         * @brief 获取数据类名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual QString getAdaptorClass() override;
        /**
         * @brief 获取适配器类型
         * @return FITKAdaptorType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual FITKAdaptorType getAdaptorType() override;

    };
}

#endif
