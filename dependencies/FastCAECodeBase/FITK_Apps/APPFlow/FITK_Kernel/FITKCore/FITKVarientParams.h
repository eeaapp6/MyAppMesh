/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKVarientParams.h
 * @brief 封装QVarient，实现参数数据的管理
 * @author libaojun (libaojunqd@Foxmail.com)
 * @date 2024-04-10
 *
 */
#ifndef __FITKVARIENTPARAMS__H__
#define __FITKVARIENTPARAMS__H__

#include "FITKCoreAPI.h"
#include <QHash>
#include <QString>
#include <QVariant>

namespace Core
{
    /**
     * @brief 参数管理
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-04-10
     */
    class  FITKCoreAPI FITKVarientParams
    {
    public:
        /**
         * @brief Construct a new FITKRunTimeSetting object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        explicit FITKVarientParams() = default;
        /**
         * @brief Destroy the FITKRunTimeSetting object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        virtual ~FITKVarientParams() = default;
        /**
         * @brief 设置参数
         * @param[i]  k              关键字
         * @param[i]  v              值
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        void setValue(const QString& k, const QVariant  v);
        /**
         * @brief 获取参数
         * @param[i]  k              关键字
         * @return QVariant          值
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        QVariant getValue(const QString& k) const;

        template <typename T>
        /**
         * @brief 获取参数
         * @param[i]  k              关键字
         * @return T                 值
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        T getValueT(const QString& k) const
        {
            QVariant v = this->getValue(k);
            return v.value<T>();
        }

        /**
         * @brief 移除参数
         * @param[i]  k              关键字
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-18
         */
        void removeVarient(const QString & k);
        /**
         * @brief 清空参数
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-18
         */
        void clear();

    private:
        /**
         * @brief 参数链表
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        QHash<QString, QVariant> _settings;

    };

}



#endif
