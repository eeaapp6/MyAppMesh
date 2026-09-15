/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITKABSTRACT_OBJECT_CONTAINER_H__
#define __FITKABSTRACT_OBJECT_CONTAINER_H__

#include "FITKCoreAPI.h"
#include "FITKAbstractObject.hpp"

namespace Core
{
    class FITKAbstractObjectPrivate;
    /**
     * @brief 内存对象容器私有成员，用于管理容器与数据对象的关系
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-10
     */
    class FITKCoreAPI FITKAbstractObjectContainerPrivate
    {
        friend FITKAbstractObjectPrivate;
    public:
        /**
         * @brief Construct a new FITKAbstractObjectContainerPrivate object
         * @param[i]  absObj        内存数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        FITKAbstractObjectContainerPrivate(FITKAbstractObjectPrivate* absObj);
        FITKAbstractObjectContainerPrivate();
        virtual ~FITKAbstractObjectContainerPrivate() = 0;
        /**
         * @brief 获取抽象对象
         * @return FITKAbstractObjectPrivate*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        FITKAbstractObjectPrivate* getObject();

        void setObj(FITKAbstractObjectPrivate* obj);
    private:
        /**
         * @brief 内存对象被析构
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        virtual void ObjectDestoried();

    private:
        /**
         * @brief 内存对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        FITKAbstractObjectPrivate* _object{};
    };



    template <typename T >
    /**
     * @brief 抽象对象容器，类似智能指针，可以避免野指针出现
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-10
     */
    class FITKAbstractObjectContainer : protected FITKAbstractObjectContainerPrivate
    {
    public:
       /**
        * @brief Construct a new FITKAbstractObjectContainer object
        * @param[i]  obj          内存对象
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-05-10
        */
        explicit FITKAbstractObjectContainer(FITKAbstractObject* obj)
            :FITKAbstractObjectContainerPrivate(obj) {}
        explicit FITKAbstractObjectContainer() = default;
        /**
         * @brief Destroy the FITKAbstractObjectContainer object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        virtual ~FITKAbstractObjectContainer() = default;
        FITKAbstractObjectContainer(const FITKAbstractObjectContainer&) = delete;
        /**
         * @brief 获取对象并转换类型
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        T* getObjectPtr() { return dynamic_cast<T*>(this->getObject()); }
        /**
         * @brief *运算符重载
         * @return T&
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        T& operator *()  { return *(this->getObjectPtr()); }
        /**
         * @brief ->运算符重载
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        T* operator->()  { return this->getObjectPtr(); }
        /**
         * @brief 运算符重载
         * @return bool
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        operator bool()  { return this->getObjectPtr() != nullptr; }

         
        /**
         * @brief 运算符重载
         * @return bool
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        void operator = (FITKAbstractObject* obj) { this->setObj(obj); }

        /**
         * @brief 运算符重载
         * @return bool
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        bool operator == (FITKAbstractObject* obj) { return this->getObjectPtr() == obj; }

        /**
         * @brief 运算符重载
         * @return bool
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        bool operator != (FITKAbstractObject* obj) { return this->getObjectPtr() != obj; }

    };

}



#endif

