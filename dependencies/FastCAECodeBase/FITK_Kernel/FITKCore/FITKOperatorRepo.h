/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKOperatorRepo.h
 * @brief 操作器仓库声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-04
 *
 */
#ifndef __FITK_OPERATOR_REPO_H__
#define __FITK_OPERATOR_REPO_H__

#include "FITKCoreAPI.h"
#include "FITKCoreMacros.h"
#include "FITKAbstractObject.hpp"
#include <QMutex>
#include <QHash>
#include <functional>
#include <QString>

namespace Core
{
    class FITKAbstractOperator;
    class FITKOperatorRepoPrivate;
    class FITKOperatorDeletor;

    /**
     * @brief 操作器仓库声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-04
     */
    class FITKCoreAPI FITKOperatorRepo : public FITKAbstractObject
    {
        friend FITKOperatorDeletor;
        //声明为单例
        DeclSingleton(FITKOperatorRepo);
        /**
         * @brief 获取操作器，不存在的直接创建，存在的返回原有对象
         * @param[i]  key            Operator注册的关键字
         * @return FITKAbstractOperator*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        FITKAbstractOperator* getOperator(const QString& key);

        template <class T>
        /**
         * @brief 获取操作器并转换为T类型，不存在的直接创建，存在的返回原有对象
         * @param[i]  key            Operator注册的关键字
         * @return FITKAbstractOperator*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        T* getOperatorT(const QString& key)
        {
            return dynamic_cast<T*> (this->getOperator(key));
        }
        /**
         * @brief 移除操作器
         * @param[i]  key           操作器键值
         * @param[i]  removeFun      是否移除创建函数
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-15
         */
        void removeOperator(const QString &key, bool removeFun = true);
        /**
         * @brief 清空仓库中已经创建的操作器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2026-04-22
         */
        void clear();
        /**
         * @brief 注册操作器创建函数
         * @param[i]  key           操作器键值
         * @param[i]  fun           创建函数
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-15
         */
        void registerOperatorFunction(const QString& key, std::function<FITKAbstractOperator*()> fun);

    private:
        /**
         * @brief 初始化
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        void initialize();
        /**
         * @brief 结束，析构函数调用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void finalize();

    private:
        /**
         * @brief 注册进来的创建operator函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        QHash<QString, std::function<FITKAbstractOperator*()>> _createFuns{};
        /**
         * @brief 实际执行创建操作
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        FITKOperatorRepoPrivate* _private{};


    public:
        template < typename T >
        /**
         * @brief 注册器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        class FITKOperatorRepoRegHelper
        {
        public:
            FITKOperatorRepoRegHelper(QString key)
            {
                auto ins = FITKOperatorRepo::getInstance();
                // 注册构造函数，T类型必须有默认的无参构造函数
                ins->_createFuns.insert( key,[&] { return new T; });
            }
        };
    };
}

/**
 * @brief 声明注册宏
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-03
 */
#ifndef Register2FITKOperatorRepo
#define Register2FITKOperatorRepo(key,Class,...)          \
 static Core::FITKOperatorRepo::FITKOperatorRepoRegHelper< Class >   \
                _FITKDataIORegHelper_##key##_##Class##__VA_ARGS__(#key)
#endif

/**
 * @brief 访问单例宏
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-04
 */
#ifndef FITKOPERREPO
#define FITKOPERREPO  Core::FITKOperatorRepo::getInstance()
#endif // !FITKOPERREPO


#endif // !__FITK_OPERATOR_REPO_H__
