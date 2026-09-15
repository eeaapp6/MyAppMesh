/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractFactory.h
 * @brief 抽象工厂类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-02
 *
 */
#ifndef __FITKABSTRACTFACTORY_H__
#define __FITKABSTRACTFACTORY_H__

#include "FITKAbstractObject.hpp"
#include <QString>

namespace Core
{
    /**
     * @brief 抽象工工厂声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    class FITKCoreAPI FITKAbstractFactory : public FITKAbstractObject
    {
    public:
       /**
        * @brief Construct a new FITKAbstractFactory object
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-02
        */
        explicit FITKAbstractFactory() = default;
        /**
         * @brief Destroy the FITKAbstractFactory object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        virtual ~FITKAbstractFactory() = 0;
        /**
         * @brief 根据key创建对象
         * @param[i]  key         关键字
         * @return FITKAbstractObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        virtual FITKAbstractObject* create(const QString& key );
        /**
         * @brief 根据key创建对象
         * @param[i]  key          关键字
         * @return FITKAbstractObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        virtual FITKAbstractObject* create(const int key) ;

        template<class T>
         /**
         * @brief 根据key创建对象，并转换类型
         * @param[i]  key         关键字
         * @return FITKAbstractObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        T* createT(const QString & key)
        {
            return dynamic_cast<T*>(create(key));
        }

        template<class T>
         /**
         * @brief 根据key创建对象并转换类型
         * @param[i]  key         关键字
         * @return FITKAbstractObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        T* createT(const int key)
        {
            return dynamic_cast<T*>(create(key));
        }

    };
}



#endif

