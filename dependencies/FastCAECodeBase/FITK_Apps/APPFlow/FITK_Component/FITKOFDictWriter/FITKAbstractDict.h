/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbstractDict.h
 * @brief 字典抽象数据声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-18
 */
#ifndef _FITKABSTRACTDICT_H__
#define _FITKABSTRACTDICT_H__

#include "FITKOFDictWriterAPI.h"
#include <QString>
#include <QList>

class QTextStream;

namespace DICT
{
    class FITKDictValue;
    /**
     * @brief 字典抽象基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFDictWriterAPI FITKAbstractDict
    {
        friend class FITKDictValue;
    public:
        /**
         * @brief 字典类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-15
         */
        enum DictType
        {
            DictNone,    //空，普通的键值对
            DictBool,    //字典数值
            DictInt,     //字典数值
            DictDouble,  //字典数值
            DictString,  //字典字符串
            DictStrList, //字典字符串链表
            DictVector,  //字典数组
            DictArray,   //字典数组
            DictGroup,  //字典组件
            DictInclude,//字典引用文件声明
        };

        explicit FITKAbstractDict() = default;
        virtual ~FITKAbstractDict() = 0;

        /**
         * @brief 纯虚函数-获取字典数据类型
         * @return DICT::FITKAbstractDict::DictType
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        virtual DictType getDictType() = 0;
    protected:
        /**
         * @brief 纯虚函数-写出字典数据文件
         * @param[i] stream
         * @param[i] lineStart
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-16
         */
        virtual void writeDict(QTextStream* stream, QString lineStart) = 0;

    protected:

    };
}


#endif
