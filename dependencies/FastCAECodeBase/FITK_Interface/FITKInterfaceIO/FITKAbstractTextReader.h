/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractTextReader.h
 * @brief 读取文本文件类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-29
 *
 */
#ifndef  _FITKABSTRACTTEXTREADER_H__
#define  _FITKABSTRACTTEXTREADER_H__

#include "FITKAbstractIO.h"
#include <QStringList>

namespace Interface
{
    /**
     * @brief 读取文本文件
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-29
     */
    class  FITKInterfaceIOAPI FITKAbstractTextReader :public FITKAbstractIO
    {
    public:
        explicit FITKAbstractTextReader() = default;
        virtual  ~FITKAbstractTextReader() = default;
        /**
         * @brief 加载文件
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        virtual bool loadFile();
        /**
         * @brief 读取行
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        virtual QString readLine();
        /**
         * @brief 发送百分比 
         * param p -1为自动计算，否则直接发送百分比 0-100
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        void sendCurrentPercent(int p = -1);
        /**
         * @brief 回退一行
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        virtual void backLine();
        /**
         * @brief 下一行的值
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        virtual QString nextLine();
        /**
         * @brief 上一行的值
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        virtual QString previousLine();
        /**
         * @brief 是否结束
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        bool atEnd();
         /**
          * @brief 当前行号获取，从1开始
          * @return int
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-03-29
          */
        int getCurrentLineNum();
        /**
         * @brief 获取当前行
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        QString getCurrentLine();

    protected:
        /**
         * @brief 获取第index行
         * @param[i]  index         行索引
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        QString getLine(int index);

    protected:
        /**
         * @brief 当前行标记
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        int _currentLine{ 0 };
         /**
         * @brief 总行数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        int _totalLine{ -1 };
        /**
         * @brief 临时进度
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-10-15
         */
        int _provisionalProgress{ -1 };
        /**
         * @brief 全部字符
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        QByteArrayList _totalContext{};
    };
}

#endif