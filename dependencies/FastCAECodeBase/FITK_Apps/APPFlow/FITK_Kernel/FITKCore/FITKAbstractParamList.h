/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractParamList.h
 * @brief  参数列表
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-30
 *
 */
#ifndef __FITKABSTRACTPARAM__LIST_H___
#define __FITKABSTRACTPARAM__LIST_H___

#include "FITKCoreAPI.h"
#include "FITKAbstractDataManager.hpp"
#include "FITKAbstractParam.hpp"

namespace Core
{
    /**
     * @brief 参数列表
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
    class FITKCoreAPI FITKAbstractParamList
        : public FITKAbstractDataManager<FITKAbstractParamObj>
    {
    public:
        /**
         * @brief Construct a new FITKAbstractParamList object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        explicit FITKAbstractParamList() = default;
        /**
         * @brief Destroy the FITKAbstractParamList object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        virtual ~FITKAbstractParamList() = default;


        template <typename T>
        /**
         * @brief  根据名称获取参数
         * @param[i]  name           名称
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        T* getParam(const QString& name)
        {
            return dynamic_cast<T*>(this->getDataByName(name));
        }

        /**
         * @brief  参数列表转换为字符串
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        virtual QString params2String();
    };

}


#endif
