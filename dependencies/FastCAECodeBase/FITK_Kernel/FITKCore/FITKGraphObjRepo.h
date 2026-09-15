/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKGraphObjRepo.h
 * @brief 图对象仓库声明，所有的 FITKAbstractGraphObject 派生类对象都可以通过该类访问
 * @author libaojunAI
 * @date 2026-04-22
 *
 */
#ifndef __FITK_GRAPH_OBJ_REPO_H__
#define __FITK_GRAPH_OBJ_REPO_H__

#include "FITKCoreAPI.h"
#include "FITKCoreMacros.h"
#include "FITKAbstractObject.hpp"
#include <QList>
#include <QListIterator>
#include <QMutex>
#include <QMutexLocker>

namespace Core
{
    class FITKAbstractGraphObject;
    class FITKGraphObjRepoDeleator;

    /**
     * @brief 图对象管理类的实例化实现
     * @author libaojunAI
     * @date 2026-04-22
     */
    class FITKGraphObjRepoPrivate
    {
    public:
        explicit FITKGraphObjRepoPrivate() = default;
        virtual ~FITKGraphObjRepoPrivate() = default;

        /**
         * @brief 追加图对象
         * @param[i]  item            图对象
         * @author libaojunAI
         * @date 2026-04-22
         */
        void appendGraphObj(FITKAbstractGraphObject* item);
        /**
         * @brief 只从列表中移除
         * @param[i]  obj            图对象
         * @author libaojunAI
         * @date 2026-04-22
         */
        void remveFromListOnly(FITKAbstractGraphObject* obj);

        /**
         * @brief 获取图对象数量
         * @return int
         * @author libaojunAI
         * @date 2026-04-22
         */
        int getGraphObjCount() const;
        /**
         * @brief 根据索引获取图对象
         * @param[i]  index          索引
         * @return FITKAbstractGraphObject*
         * @author libaojunAI
         * @date 2026-04-22
         */
        FITKAbstractGraphObject* getGraphObjAt(const int index);
        /**
         * @brief 获取迭代器
         * @return QListIterator<Core::FITKAbstractGraphObject*>
         * @author libaojunAI
         * @date 2026-04-22
         */
        QListIterator<Core::FITKAbstractGraphObject*> getIterator();
        /**
         * @brief 清空仓库
         * @author libaojunAI
         * @date 2026-04-22
         */
        void clear();

    private:
        /**
         * @brief 图对象列表
         * @author libaojunAI
         * @date 2026-04-22
         */
        QList<FITKAbstractGraphObject*> _graphObjList{};
    };

    class FITKGraphObjRepoDeleator;

    /**
     * @brief 图对象仓库声明，所有的 FITKAbstractGraphObject 派生类对象都可以通过该类访问
     * @author libaojunAI
     * @date 2026-04-22
     */
    class FITKCoreAPI FITKGraphObjRepo
    {
        DeclSingleton(FITKGraphObjRepo);
        friend FITKGraphObjRepoDeleator;
        friend FITKAbstractGraphObject;

    public:
        /**
         * @brief 获取图对象数量
         * @return int
         * @author libaojunAI
         * @date 2026-04-22
         */
        int getGraphObjCount();
        /**
         * @brief 根据索引获取图对象
         * @param[i]  index          索引
         * @return FITKAbstractGraphObject*
         * @author libaojunAI
         * @date 2026-04-22
         */
        FITKAbstractGraphObject* getGraphObjAt(const int index);

        template <class T>
        /**
         * @brief 根据索引获取图对象并强制转化为 T 类型
         * @param[i]  index          索引
         * @return T*
         * @author libaojunAI
         * @date 2026-04-22
         */
        T* getGraphObjTAt(const int index)
        {
            QMutexLocker locker(&m_mutex);
            return dynamic_cast<T*>(_repoPrivate.getGraphObjAt(index));
        }

        template<class T>
        /**
         * @brief 获取 T 类型的全部图对象
         * @return QList<T*>
         * @author libaojunAI
         * @date 2026-04-22
         */
        QList<T*> getGraphObj()
        {
            QMutexLocker locker(&m_mutex);

            QList<T*> objs;
            const int n = _repoPrivate.getGraphObjCount();
            for (int i = 0; i < n; ++i)
            {
                auto obj = _repoPrivate.getGraphObjAt(i);
                auto tobj = dynamic_cast<T*>(obj);
                if (tobj)
                    objs.append(tobj);
            }
            return objs;
        }

        /**
         * @brief 获取迭代器
         * @return QListIterator<Core::FITKAbstractGraphObject*>
         * @author libaojunAI
         * @date 2026-04-22
         */
        QListIterator<Core::FITKAbstractGraphObject*> getIterator();
        /**
         * @brief 清空仓库
         * @author libaojunAI
         * @date 2026-04-22
         */
        void clear();
        /**
         * @brief 是否包含图对象
         * @param[i]  obj            图对象
         * @return true
         * @return false
         * @author libaojunAI
         * @date 2026-04-22
         */
        bool isContains(FITKAbstractGraphObject* obj);

    private:
        /**
         * @brief 初始化仓库
         * @author libaojunAI
         * @date 2026-04-22
         */
        void initialize();
        /**
         * @brief 结束，析构函数调用
         * @author libaojunAI
         * @date 2026-04-22
         */
        void finalize();
        /**
         * @brief 追加图对象，FITKAbstractGraphObject 构造函数调用
         * @param[i]  obj            图对象
         * @author libaojunAI
         * @date 2026-04-22
         */
        void addGraphObj(FITKAbstractGraphObject* obj);
        /**
         * @brief 移除图对象，FITKAbstractGraphObject 析构函数调用
         * @param[i]  obj            图对象
         * @author libaojunAI
         * @date 2026-04-22
         */
        void removeGraphObj(FITKAbstractGraphObject* obj);

    private:
        /**
         * @brief 图对象管理
         * @author libaojunAI
         * @date 2026-04-22
         */
        FITKGraphObjRepoPrivate _repoPrivate{};
    };

    class FITKGraphObjRepoDeleator
    {
    public:
        explicit FITKGraphObjRepoDeleator() = default;
        virtual ~FITKGraphObjRepoDeleator();
    };
}

/**
 * @brief 定义快速访问
 * @author libaojunAI
 * @date 2026-04-22
 */
#ifndef FITKGRAPHOBJREPO
#define FITKGRAPHOBJREPO (Core::FITKGraphObjRepo::getInstance())
#endif

#endif