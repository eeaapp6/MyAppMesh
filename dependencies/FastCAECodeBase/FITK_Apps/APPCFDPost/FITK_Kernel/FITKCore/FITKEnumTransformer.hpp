/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKEnumTransformer.hpp
 * @brief  枚举值转换器
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-15
 *
 */
#ifndef __FITKENUMTRANSFORMER_H___
#define __FITKENUMTRANSFORMER_H___

#include <QMetaEnum>
#include <QStringList>
#include <QString>

namespace Core
{
    template <typename EnumType>
    /**
     * @brief 枚举值转换器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-15
     */
    class FITKEnumTransfer
    {
    public:
        /**
         * @brief Construct a new FITKEnumTransfer object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-15
         */
        FITKEnumTransfer()
        {
            _enumMeta = QMetaEnum::fromType<EnumType>();
        }
        ~FITKEnumTransfer() = default;
        /**
         * @brief 字符串转化为枚举值
         * @param[i]  string         字符串
         * @param[o]  ok             转换是否成功
         * @return EnumType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-15
         */
        EnumType fromString(const QString& string, bool& ok)
        {
            QByteArray ba = string.toLocal8Bit();
            EnumType resEnum = (EnumType)_enumMeta.keyToValue(ba.data(), &ok);
            return resEnum;
        }
        /**
         * @brief 枚举值转换为字符串
         * @param[i]  value          枚举值
         * @param[o]  ok             转化是否成功
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-15
         */
        QString toString(EnumType value, bool& ok)
        {
            ok = false;
            QString result = _enumMeta.valueToKey(value);
            if (!result.isEmpty()) ok = true;
            return result;
        }
        /**
         * @brief 获取枚举个数
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-15
         */
        int memberCount()
        {
            return _enumMeta.keyCount();
        }
        /**
         * @brief 字符串是否为有效的枚举值
         * @param[i]  v              字符串
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-15
         */
        bool isValid(const QString& v)
        {
            QByteArray ba = v.toLocal8Bit();
            bool ok = false;
            _enumMeta.keyToValue(ba.data(), &ok);
            return ok;
        }
        /**
         * @brief 获取全部枚举值
         * @return QStringList
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-15
         */
        QStringList members()
        {
            QStringList sl;
            const int n = this->memberCount();
            for (int i = 0; i< n ; ++i)
            {
                EnumType value = (EnumType)_enumMeta.value(i);
                bool ok = false;
                QString v = toStrting(value, ok);
                if(ok) sl.append(v);
            }
            return sl;
        }

    private:
    /**
     * @brief 枚举元对象
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-15
     */
        QMetaEnum _enumMeta;
    };
}

#endif
