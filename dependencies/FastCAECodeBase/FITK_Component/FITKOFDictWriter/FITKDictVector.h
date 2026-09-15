/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKDictVector.h
 * @brief 字典数组 (value value value) [value value value]
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-16
 */
#ifndef _FITKDICTVECTOR_H__
#define _FITKDICTVECTOR_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKAbstractDict.h"

namespace DICT
{
    class FITKDictValue;
    /**
     * @brief 字典数组类 -只能存储简单字典数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFDictWriterAPI FITKDictVector : public FITKAbstractDict
    {
    public:

        enum ParenthesesType
        {
            NoneParentheses,
            SmallParentheses,
            MiddleParentheses,
            LargeParentheses,
        };

        explicit FITKDictVector() = default;
        FITKDictVector(const FITKDictVector &other);
        ~FITKDictVector() override;
        FITKDictVector &operator=(const FITKDictVector &other);
        /**
         * @brief 获取类型
         * @return 
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        DictType getDictType() override;
        /**
         * @brief 设置括号类型
         * @param[i] type ParenthesesType
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        void setParenthesesType(ParenthesesType type);
        /**
         * @brief 获取括号类型
         * @return ParenthesesType
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        ParenthesesType getParenthesesType();
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
         * @brief 追加无名称的数据
         * @param value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        void append(FITKAbstractDict::DictType value);
        void append(bool value);
        void append(double value);
        void append(int value);
        void append(const QString &value);
        void append(const char *value);
        void append(FITKAbstractDict* value);
        /**
         * @brief 插入无名称的数据
         * @param[i] index 标号
         * @param[i] value 字典数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        void insert(int index, FITKAbstractDict::DictType value);
        void insert(int index, bool value);
        void insert(int index, double value);
        void insert(int index, int value);
        void insert(int index, const QString &value);
        void insert(int index, const char *value);
        void insert(int index, FITKAbstractDict* value);
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
        /**
         * @brief 括号类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        ParenthesesType m_parentheses{ ParenthesesType::SmallParentheses };
    };
}


#endif
