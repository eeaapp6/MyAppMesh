/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractNDataObject.h
 * @brief 有名字的数据对象声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-22
 *
 */
#ifndef _FITKABSTRACTNDATAOBJECT_H__
#define _FITKABSTRACTNDATAOBJECT_H__

#include "FITKAbstractDataObject.h"
#include <QString>

namespace Core
{
    /**
     * @brief 有名称的数据类基类，抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-22
     */
    class FITKCoreAPI FITKAbstractNDataObject : public FITKAbstractDataObject
    {
        //FITKSafeDownCastA(FITKAbstractNDataObject, FITKAbstractDataObject);
    public:
        /**
         * @brief Construct a new FITKAbstractNDataObject object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        explicit FITKAbstractNDataObject() = default;
        /**
         * @brief Construct a new FITKAbstractNDataObject object
         * @param[i]  name          名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        explicit FITKAbstractNDataObject(const QString & name);
        /**
         * @brief Destroy the FITKAbstractNDataObject object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual ~FITKAbstractNDataObject() = 0;
        /**
         * @brief 设置名称
         * @param[i]  name         名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void setDataObjectName(const QString & name);
        /**
         * @brief 获取对象名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual QString getDataObjectName();

        /**
       * @brief 复制数据对象,深拷贝，数据id不拷贝
       * @param[i] obj 被拷贝的对象
       * @return true
       * @return false
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-03-27
       */
        virtual bool copy(FITKAbstractDataObject* obj) override;

    private:
        /**
         * @brief 对象名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        QString m_dataObjName{};
    };
}


#endif
