/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractObject.hpp
 * @brief 抽象对象声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-23
 *
 */
#ifndef _FITKABSTRACTOBJ_HPP__
#define _FITKABSTRACTOBJ_HPP__

#include "FITKCoreAPI.h"
#include "FITKType.h"
#include <QString>
#include <QMetaType>
#include <QVariant>
#include <QList>

namespace Core
{
    class FITKAbstractObjectContainerPrivate;
    /**
     * @brief 私有对象，用于处理与容器对象的关系
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-10
     */
    class FITKCoreAPI FITKAbstractObjectPrivate
    {
        friend FITKAbstractObjectContainerPrivate;
    public:
        /**
         * @brief Construct a new FITKAbstractObjectPrivate object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        FITKAbstractObjectPrivate() = default;
        /**
         * @brief Destroy the FITKAbstractObjectPrivate object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
         virtual  ~FITKAbstractObjectPrivate() = 0;

    private:
        /**
         * @brief 与数据关联的容器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-10
         */
        QList<FITKAbstractObjectContainerPrivate*> _containers{};
    };

    /**
     * @brief 抽象对象声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-23
     */
    class /*FITKCoreAPI*/ FITKAbstractObject : public FITKAbstractObjectPrivate
    {

    public:
        /**
         * @brief Construct a new FITKAbstractObject object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-23
         */
        explicit FITKAbstractObject() = default;
        /**
         * @brief Destroy the FITKAbstractObject object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-23
         */
        FITKCoreAPI virtual ~FITKAbstractObject() = 0 ;

        /**
         * @brief 是否为className的类对象
         * @param[i]  className  类名，不包含命名空间
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual bool isFITKClass(const QString& className) { Q_UNUSED(className); return false; }
        /**
         * @brief 获取类名，包含命名空间
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual QString getClassName() { return QString(); }

        template< class T>
        /**
         * @brief 强制转换this对象类型
         * @return T* 转换后的类型指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-23
         */
        T* FITKSafedownCast()
        {
            return dynamic_cast<T*>(this);
        }

        template< class T>
        /**
         * @brief 强制转换指针类型
         * @param[i]  obj          转换对象
         * @return T* 转换后输出的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-23
         */
        static T* FITKSafedownCast(FITKAbstractObject* obj)
        {
            return dynamic_cast<T*>(obj);
        }
        /**
         * @brief 将对象转换为QVarient
         * @return QVariant
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        QVariant FITKObj2QVarient()
        {
            return QVariant::fromValue<FITKAbstractObject*>(this);
        }

        template <class T>
        /**
         * @brief 将qvarient转化为T对象指针
         * @param[i]  va             qvarient变量
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        static T* QVariant2FITKObj(QVariant va)
        {
            return va.value<T*>();
        }
        /**
         * @brief 是否enable
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-13
         */
        bool FITKCoreAPI isEnable();
        /**
         * @brief 设置enable
         * @param[i]  en  enable
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-13
         */
        void FITKCoreAPI enable(bool en = true);

    private:
        /**
         * @brief 是否enable
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-13
         */
        bool _objEnable{ true };

    };

}

Q_DECLARE_METATYPE(Core::FITKAbstractObject*)

#endif // _FITKABSTRACTOBJ_HPP__
