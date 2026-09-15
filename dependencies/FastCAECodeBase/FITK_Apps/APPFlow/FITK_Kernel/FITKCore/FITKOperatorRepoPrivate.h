/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKOperatorRepoPrivate.h
 * @brief 创建并维护操作器类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-04
 *
 */
#ifndef _FITK_OPERATOR_REPO_PRIVATE_H__
#define _FITK_OPERATOR_REPO_PRIVATE_H__

#include "FITKAbstractObject.hpp"
#include <QObject>
#include <QHash>
#include <QString>
#include <functional>

namespace Core
{
    class FITKAbstractOperator;

    /**
     * @brief 创建操作器与维护操作器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-04
     */
    class FITKOperatorRepoPrivate : public QObject, public FITKAbstractObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKOperatorRepoPrivate object
         * @param[i]  funs         创建函数的Hash，引用，实际对象在FITKOperatorRepo
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        explicit FITKOperatorRepoPrivate(QHash<QString,std::function<FITKAbstractOperator*()>>& funs);
        /**
         * @brief Destroy the FITKOperatorRepoPrivate object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual ~FITKOperatorRepoPrivate();

        /**
         * @brief 获取operator，不存在则创建
         * @param[i]  key            创建对象的关键字
         * @return FITKAbstractOperator*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        FITKAbstractOperator* getOperator(const QString & key);
        /**
         * @brief 移除操作器
         * @param[i]  key           操作器键值
         * @param[i]  removeFun      是否移除创建函数
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-15
         */
        void removeOperator(const QString &key, bool removeFun = true);
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
         * @brief 创建操作器
         * @param[i]  key           关键字
         * @return FITKAbstractOperator*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        FITKAbstractOperator* createOperator(const QString & key);

    private slots:
        /**
         * @brief operator被释放槽函数
         * @param[i]  oper          被释放的槽函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        void operatorDestoryedSlot(FITKAbstractOperator* oper);

    private:
        /**
         * @brief 创建operator函数列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        QHash<QString, std::function<FITKAbstractOperator*()>>&  _createFuns;
        /**
         * @brief 已经创建的operator
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        QHash<QString, FITKAbstractOperator*> _existOperators{};
    };
}



#endif
