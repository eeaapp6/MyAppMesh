/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKDictValue.h
 * @brief 字典数据适配器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-15
 */
#ifndef _FITKDICTVALUE_H__
#define _FITKDICTVALUE_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKAbstractDict.h"
#include <QStringList>

namespace DICT
{
    class FITKDictArray;
    class FITKDictGroup;
    class FITKDictVector;
    class FITKDictObject;
    /**
     * @brief 字典数据类 适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFDictWriterAPI FITKDictValue
    {
        friend class FITKDictObject;
        friend class FITKDictArray;
        friend class FITKDictGroup;
        friend class FITKDictVector;
    public:
        /**
         * @brief create a new FITKDictValue
         * @param type
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        FITKDictValue(FITKAbstractDict::DictType type);
        FITKDictValue(bool b);
        FITKDictValue(double n);
        FITKDictValue(int n);
        FITKDictValue(const QString &s, bool isInclude = false);
        FITKDictValue(const char *s);
        FITKDictValue(const QStringList &s);
        FITKDictValue(FITKAbstractDict* s);
        /**
         * @brief delete a FITKDictValue
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-25
         */
        ~FITKDictValue();
        /**
         * @brief 拷贝构造
         * @param other
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        FITKDictValue(const FITKDictValue &other);
        FITKDictValue &operator=(const FITKDictValue &other);
        /**
         * @brief 获取类型
         * @return 
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        FITKAbstractDict::DictType getDictType();
        /**
         * @brief 设置key值
         * @param[i] key
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        void setKey(const QString key);
        /**
         * @brief 获取key值
         * @return 
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        QString getKey() const;
        /**
         * @brief 转换bool
         * @param[o] ok 状态
         * @return bool
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        bool toBool(bool& ok) const;
        /**
         * @brief 转换double
         * @param[o] ok 状态
         * @return double
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        double toDouble(bool& ok) const;
        /**
         * @brief 转换int
         * @param[i] ok 状态
         * @return  int
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        int toInt(bool& ok) const;
        /**
         * @brief 转换QString
         * @param[i] ok 状态
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        QString toString(bool& ok) const;
        /**
         * @brief 转换QStringList
         * @param[i] ok 状态
         * @return QStringList
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-25
         */
        QStringList toStringList(bool& ok) const;
        /**
         * @brief    转换Include
         * @param[i] ok 
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        QString toInclude(bool& ok) const;
        /**
         * @brief 转换FITKDictArray*
         * @param[i] ok 状态
         * @return FITKDictArray*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        FITKDictArray* toArray(bool& ok) const;
        /**
         * @brief 转换FITKDictModule*
         * @param[i] ok 状态
         * @return FITKDictModule*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        FITKDictGroup* toGroup(bool& ok) const;
        /**
         * @brief 转换FITKDictVector*
         * @param[i] ok 状态
         * @return FITKDictVector*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        FITKDictVector* toVector(bool& ok) const;
        /**
         * @brief 是否为空数据
         * @return 状态
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        bool isNull() const;
        /**
         * @brief 是否是简单数据类型
         * @return 
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        bool isSimpleType() const;

        /**
         * @brief 获取key为空值时，简单数据下的数据
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        QString getDataKeyEmpty();
        /**
         * @brief    适配字典文件中输出double的工具
         * @param[i] v 
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-28
         */
        static QString doubleAdaptationTool(double v);

    protected:
        /**
         * @brief 写出字典文件
         * @param stream
         * @param lineStart
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        void writeDict(QTextStream* stream, QString lineStart);
        
        

    private:
        /**
         * @brief value
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-25
         */
        union
        {
            int m_value_Int;
            bool m_value_Bool;
            double m_value_Double;
            QString* m_value_String;
            QStringList* m_value_StrList;
            FITKAbstractDict* m_value_Data;
        };
        /**
         * @brief key值
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-25
         */
        QString m_key{};
        /**
         * @brief 类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-25
         */
        FITKAbstractDict::DictType m_type{ FITKAbstractDict::DictType::DictNone };
    };
}


#endif
