/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKDataRepo.h
 * @brief 数据仓库定义，所有的数据对象都会在仓库中
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-27
 *
 */
#ifndef __FITKDATAREPO_H__
#define __FITKDATAREPO_H__

#include "FITKAbstractDataManager.hpp"
#include "FITKCoreMacros.h"
#include "FITKAbstractDataObject.h"
#include <QList>
#include <QListIterator>

namespace Core
{
    /**
     * @brief 实例化数据管理类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-27
     */
    class FITKDataRepoPrivate : public FITKAbstractDataManager<FITKAbstractDataObject>
    {
    public:
        explicit FITKDataRepoPrivate() = default;
        virtual ~FITKDataRepoPrivate() = default;

        /**
        * @brief 追加数据对象
        * @param[i]  obj            追加数据
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-29
        */
        virtual void appendDataObj(FITKAbstractDataObject* item) override;
        /**
         * @brief 只从列表中移除
         * @param[i]  obj            数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        void remveFromListOnly(FITKAbstractDataObject* obj);

    };
    class FITKDataRepoDeleator;
    /**
     * @brief 数据仓库声明，所有的FITKAbstractDataObject派生类对象都可以通过该类访问
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-27
     */
    class FITKCoreAPI FITKDataRepo
    {
        DeclSingleton(FITKDataRepo);
        friend FITKDataRepoDeleator;
        friend FITKAbstractDataObject;
        /**
         * @brief 获取数据的数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        int getDataCount();
        /**
         * @brief 根据ID获取数据对象
         * @param[i]  id
         * @return FITKAbstractDataObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        FITKAbstractDataObject* getDataByID(const int id);

        template <class T >
        /**
         * @brief 根据ID获取数据并强制转化为T类型
         * @param[i]  id            id
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        T* getTDataByID(const int id)
        {
            QMutexLocker locker(&m_mutex);
            auto v = _repoPrivate.getDataByID(id);
            return dynamic_cast<T*>(v);
        }

        template<class T>
        /**
         * @brief 获取T类型的全部数据对象
         * @return QList<T*>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        QList<T*> getData()
        {
            QMutexLocker locker(&m_mutex);

            QList<T*> d;
            const int n = _repoPrivate.getDataCount();
            for (int i =0;i<n; ++i)
            {
                auto v = _repoPrivate.getDataByIndex(i);
                auto tv = dynamic_cast<T*>(v);
                if (tv) d.append(tv);
            }
            return d;
        }
        template <class T>
        /**
        * @brief 根据名称和类型获取数据ID
        * @param[i]  name            名称
        * @param[i]  compSens        是否大小写敏感
        * @return int                数据ID 
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-06-18
        */
        int getTDataID(const QString& name, bool compSens = false)
        {
            QMutexLocker locker(&m_mutex);
            QList<Core::FITKAbstractDataObject*> data = _repoPrivate.getDataObjectsByName(name, compSens);
            for (Core::FITKAbstractDataObject* d : data )
            {
                T* td = dynamic_cast<T*>(d);
                if (td)
                    return d->getDataObjectID();
            }
            return -1;
        }

        /**
        * @brief 清空重置仓库
        * @param[i]  save            需要保存的数据ID列表
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-06-18
        */
        void resetRepo(QList<int> save = { -1 });
        /**
        * @brief 获取迭代器器
        * @return   QListIterator<Core::FITKAbstractDataObject*>
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-03-06
        */
        QListIterator<Core::FITKAbstractDataObject*> getIterator();
      

    private:
        /**
         * @brief 初始化仓库
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        void initialize();
        /**
         * @brief 结束，析构函数调用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void finalize();
        /**
         * @brief 追加数据，FITKAbstractDataObject构造函数调用
         * @param[i]  obj            数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        void addDataObj(FITKAbstractDataObject* obj);

        /**
         * @brief 移除数据，FITKAbstractDataObject构造函数调用
         * @param[i]  obj            数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        void removeDataObj(FITKAbstractDataObject* obj);

    private:
        /**
         * @brief 数据管理
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        FITKDataRepoPrivate _repoPrivate{};
    };

    class FITKDataRepoDeleator
    {
    public:
        explicit FITKDataRepoDeleator() = default;
        virtual ~FITKDataRepoDeleator();
    };

}

#ifndef FITKDATAREPO
#define FITKDATAREPO   (Core::FITKDataRepo::getInstance())
#endif

#endif
