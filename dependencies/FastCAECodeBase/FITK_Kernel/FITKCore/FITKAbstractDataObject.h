/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractDataObject.h
 * @brief 数据类基类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-22
 *
 */
#ifndef _FITKABSTRACTDATAOBJECT_H__
#define _FITKABSTRACTDATAOBJECT_H__

#include "FITKCoreAPI.h"
#include "FITKCoreMacros.h"
#include "FITKAbstractObject.hpp"
#include <QObject>
#include <QHash>
#include <QVariant>

namespace Core
{
    class FITKDataRepo;
    class FITKAbstractDataIDAlter;
    /**
     * @brief 数据类基类声明，全部的数据类都必须是该类的派生类，抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-22
     */
    enum FITKUserData
    {
        FITKUserRole = 1,
        FITKUserRoleEnd  = 1000000,
    };

    class FITKCoreAPI FITKAbstractDataObject : public QObject , public FITKAbstractObject
    {
        friend FITKAbstractDataIDAlter;
        friend FITKDataRepo;

        Q_OBJECT;
        //FITKSafeDownCastA(FITKAbstractDataObject, FITKAbstractObject)
    public:

        /**
         * @brief Construct a new FITKAbstractDataObject object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        explicit FITKAbstractDataObject(const int parentDataID  = -1 );
        /**
         * @brief Destroy the FITKAbstractDataObject object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual ~FITKAbstractDataObject() = 0;
        /**
         * @brief 获取数据对象的ID
         * @return int  数据ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        int getDataObjectID() const;
        /**
         * @brief 设置父数据对象id
         * @param[i]  id             父数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        virtual void setParentDataID(const int id);
        /**
         * @brief 获取父数据对象id
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        int getParentDataID() const;
        /**
         * @brief 序列化对象
         * @param[i] label 标记位，标记序列化类型
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-24
         */
        virtual QString serialize(const int & label = -1);
        /**
         * @brief 反序列化对象
         * @param[i] label 标记位，标记序列化类型
         * @param[i] text 字符内容
         * @return bool 是否序列化成功
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-24
         */
        virtual bool deserialize(const QString& text, const int &label = -1);

        /**
         * @brief 获取用户数据对象
         * @param[i]  userData       标记
         * @return QVariant
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        QVariant getUserData(int userData);
        /**
         * @brief 记录用户数据对象，ud重复则覆盖
         * @param[i]  ud            标记
         * @param[i]  va             数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        virtual void setUserData(int ud, QVariant va);
        /**
         * @brief 获取全部的数据对象标记
         * @return QList<int>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        QList<int> getUserDataKeys();

        template <typename T>
        /**
         * @brief 获取用户数据，并进行强制类型转换
         * @param[i]  ud            标记
         * @return T
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        T getUserDataT(int ud)
        {
            QVariant v = this->getUserData(ud);
            return v.value<T>();
        }

        /**
         * @brief 复制数据对象,深拷贝，数据id不拷贝
         * @param[i] obj 被拷贝的对象
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-27
         */
        virtual bool copy(FITKAbstractDataObject* obj);
        /**
         * @brief 获取父数据对象
         * @return FITKAbstractDataObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-14
         */
        FITKAbstractDataObject* getParentObject();

        template <typename T>
        /***
         * @brief 获取父数据对象，并强制类型转换
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-14
         */
        T* getParentObjectT()
        {
            FITKAbstractDataObject* tempObj = this;
            while (tempObj)
            {
                //类型转换成功，返回数据对象
                T* obj = dynamic_cast<T*>(tempObj);
                if (obj) return obj;
                tempObj = tempObj->getParentObject();
            }
            return nullptr;
        }
        /**
         * @brief 数据是否被使用（关联）
         * @param[i]  data           被检查的数据ID
         * @return true              管理器中使用了Data中包含的数据
         * @return false             管理器中没有使用Data中包含的数据
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-08
         */
        virtual bool isUsedDataObject(const QList<int> & dataIDs);

        /**
         * @brief 禁止相似名称的函数调用，避免错误
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        QString objectName() = delete;
        /**
         * @brief 禁止相似名称的函数调用，避免错误
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        void setObjectName(const QString &name) = delete;

    private:
        /**
         * @brief 修改最大ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-18
         */
        static void ResetMaxID(const int maxid =0);
        /**
         * @brief 获取最大ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-18
         */
        static int GetMaxID();


    signals:
        /**
         * @brief 实例对象被销毁信号
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void dataObjectDestoried(FITKAbstractDataObject*);

    protected:
        /***
        * @brief 设置数据对象ID，需谨慎使用
        * @param ID 数据对象id
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-05-14
        */
        virtual void setAbsDataID(const int id);

    private:
        /**
         * @brief 数据对象ID，全局数据唯一
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        FITKID _absDataID{ 0 };
        /**
         * @brief 父对象数据ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        FITKID _parentDataID{ 0 };
        /**
         * @brief 用户数据
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        QHash<FITKID, QVariant> _userData{};
    };

    /**
     * @brief 定义接口用于修改数据ID
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-03
     */
    class FITKCoreAPI FITKAbstractDataIDAlter
    {
    public:
        /**
         * @brief Construct a new FITKAbstractDataIDAlter object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        explicit FITKAbstractDataIDAlter() = default;
        /**
         * @brief Destroy the FITKAbstractDataIDAlter object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        virtual ~FITKAbstractDataIDAlter() = default;
        /**
         * @brief 修改数据对象ID
         * @param[i] d 数据指针
         * @param[i] id 数据ID
         * @return true  修改成功
         * @return false 修改失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        bool modifyDataID(FITKAbstractDataObject* d, const int id);


        /**
       * @brief 获取一个可用的ID，没有被占用的ID
       * @return int 
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-06-03
       */
       static int GetAValidDataID();
    };

}


#endif
