/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractTextWriter.h
 * @brief 文本文件写出基类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-13
 *
 */
#ifndef  _FITKABSTRACTTEXTWiter_H__
#define  _FITKABSTRACTTEXTWiter_H__

#include "FITKAbstractIO.h"
#include <QTextStream>
#include <QFile>
namespace Interface
{
    /**
     * @brief 文本文件写出基类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-13
     */
    class  FITKInterfaceIOAPI FITKAbstractTextWriter :public FITKAbstractIO
    {
    public:
        explicit FITKAbstractTextWriter() = default;
        virtual  ~FITKAbstractTextWriter() = default;

        /**
         * @brief 打开文件流
         * @return 状态 true 成功 false 失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-18
         */
        bool openFileStream();

        /**
         * @brief 关闭文件流
         * @return 状态 true 成功 false 失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-18
         */
        bool closeFileStream();

        /**
         * @brief 获取文件流
         * @return 文件流
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-18
         */
        QTextStream* getStream()
        {
            return _stream;
        }

        /**
         * @brief 格式化 转换 Double 转 String
         * @param d double数据
         * @param prec 对double的有效字符数
         * @param width1 字符宽度
         * @param f 表示科学计数法或xxx
         * @return 转换后的字符
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-18
         */
        static QString double2String(double d, int prec = 9, int width1 = 18, char f = 'g');

        /**
         * @brief 格式化 转换 int 转 String
         * @param i int 数据
         * @param width1 字符宽度
         * @return 转换后的字符
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-18
         */
        static QString int2String(int i, int width1);

    private:
        /**
         * @brief 文件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        QFile* _file{};
        /**
         * @brief 文本流
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        QTextStream* _stream{};

    };
}

#endif
