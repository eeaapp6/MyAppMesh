/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKDictArray.h
 * @brief 字典数组 name (value1, value2, value3, ...);
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-15
 */
#ifndef _FITKDICTARRAY_H__
#define _FITKDICTARRAY_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKAbstractDict.h"

namespace DICT
{
    class FITKDictValue;
    /**
     * @brief 字典数组
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFDictWriterAPI FITKDictArray : public FITKAbstractDict
    {
    public:
        explicit FITKDictArray() = default;
        FITKDictArray(const FITKDictArray &other);
        ~FITKDictArray() override;
        FITKDictArray &operator=(const FITKDictArray &other);
        /**
         * @brief 获取类型
         * @return 
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        DictType getDictType() override;
        /**
         * @brief 获取数组数量
         * @return int
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        int count() const;
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
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(FITKAbstractDict::DictType value, QString key = "");
        /**
         * @brief 追加字典数据
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(bool value, QString key = "");
        /**
         * @brief 追加字典数据
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(double value, QString key = "");
        /**
         * @brief 追加字典数据
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(int value, QString key = "");
        /**
         * @brief 追加字典数据
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(const QString &value, QString key = "");
        /**
         * @brief 追加字典数据
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(const char *value, QString key = "");
        /**
         * @brief 追加字典数据
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(const QStringList& value, QString key = "");
        /**
         * @brief 追加字典数据
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void append(FITKAbstractDict* value, QString key = "");
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, FITKAbstractDict::DictType value, QString key = "");
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, bool value, QString key = "");
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, double value, QString key = "");
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, int value, QString key = "");
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, const QString &value, QString key = "");
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, const char *value, QString key = "");
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, const QStringList& value, QString key = "");
        /**
         * @brief 插入字典数据
         * @param[i] index 位置标号
         * @param[i] value 字典数据
         * @param[i] key 名称或钥匙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        void insert(int index, FITKAbstractDict* value, QString key = "");
        /**
         * @brief 移除字典数据
         * @param[i] i 标号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        void removeAt(int i);

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
         * @brief 是否全是简单字典数据
         * @return 状态
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        bool isAllSimpleType();

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
