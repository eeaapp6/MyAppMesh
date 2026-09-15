/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKDictGroup.h
 * @brief 字典组件 name {v1 v2 v3}
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-18
 */
#ifndef _FITKDICTGROUP_H__
#define _FITKDICTGROUP_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKAbstractDict.h"

namespace DICT
{
    class FITKDictValue;
    /**
     * @brief 字典组件
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFDictWriterAPI FITKDictGroup : public FITKAbstractDict
    {
    public:
        explicit FITKDictGroup() = default;
        FITKDictGroup(const FITKDictGroup &other);
        ~FITKDictGroup() override;
        FITKDictGroup &operator=(const FITKDictGroup &other);
        /**
         * @brief 获取类型
         * @return
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        DictType getDictType() override;
        /**
         * @brief 获取数量
         * @return int
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        int count() const;
        /**
         * @brief 通过名称获取字典数据
         * @param[i] key 名称或钥匙
         * @return DICT::FITKDictValue*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        FITKDictValue* value(const QString key);
        /**
         * @brief 通过下标获取字典数据
         * @param[i] index 下标
         * @return DICT::FITKDictValue*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        FITKDictValue* at(const int index);
        /**
        * @brief 追加字典数据
        * @param[i] key 名称或钥匙
        * @param[i] value 字典数据
        * @author liuzhonghua (liuzhonghuaszch@163.com)
        * @date 2024-07-17
        */
        void append(QString key, FITKAbstractDict::DictType value);
        /**
         * @brief 追加字典数据
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(QString key, bool value);
        /**
         * @brief 追加字典数据
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(QString key, double value);
        /**
         * @brief 追加字典数据
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(QString key, int value);
        /**
         * @brief 追加字典数据
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(QString key, const QString &value);
        /**
         * @brief 追加字典数据
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(QString key, const char *value);
        /**
         * @brief 追加字典数据
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(QString key, const QStringList &value);
        /**
         * @brief 追加字典数据
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(QString key, FITKAbstractDict* value);
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, QString key, FITKAbstractDict::DictType value);
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, QString key, bool value);
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, QString key, double value);
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, QString key, int value);
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, QString key, const QString &value);
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, QString key, const char *value);
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, QString key, const QStringList& value);
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] key 名称或钥匙
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, QString key, FITKAbstractDict* value);
        /**
         * @brief 移除字典数据
         * @param[i] i 标号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        void removeAt(int index);
        /**
         * @brief 移除字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        void removeKey(QString key);
    protected:
        /**
         * @brief 写出字典数据
         * @param[i] stream 文件数据流
         * @param[i] lineStart
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        void writeDict(QTextStream* stream, QString lineStart) override;

    private:
        /**
         * @brief 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        QList<FITKDictValue*> m_values{};
    };
}


#endif
