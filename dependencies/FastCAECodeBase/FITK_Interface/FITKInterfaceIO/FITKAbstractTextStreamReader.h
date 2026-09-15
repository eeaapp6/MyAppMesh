/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractTextReader.h
 * @brief 读取文本文件类，基于文件流，适用于大文件
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2026-02-02
 *
 */
#ifndef  _FITKABSTRACTTEXTSTREAMREADER_H__
#define  _FITKABSTRACTTEXTSTREAMREADER_H__

#include "FITKAbstractIO.h"
#include <QStringList>

class QTextStream;
class QFile;

namespace Interface
{
    /**
     * @brief 读取文本文件，基于文件流
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2026-02-02
     */
    class  FITKInterfaceIOAPI FITKAbstractTextStreamReader :public FITKAbstractIO
    {
    public:
        explicit FITKAbstractTextStreamReader() = default;
        virtual  ~FITKAbstractTextStreamReader();
        /**
         * @brief 加载文件
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-02-02
         */
        virtual bool loadFile();
        /**
         * @brief 读取行
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-02-02
         */
        virtual QString readLine();
        /**
         * @brief 发送百分比 
         * param p -1为自动计算，否则直接发送百分比 0-100
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-02-02
         */
        void sendCurrentPercent(int p = -1);
        /**
         * @brief 回退一行
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-02-02
         */
        virtual void backLine();
        /**
         * @brief 下一行的值
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-02-02
         */
        virtual QString nextLine();
        /**
         * @brief 上一行的值
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-02-02
         */
        virtual QString previousLine();
        /**
         * @brief 是否结束
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-02-02
         */
        bool atEnd();
         /**
          * @brief 当前行号获取，从1开始
          * @return int
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2026-02-02
          */
        int getCurrentLineNum();
        /**
         * @brief 获取当前行
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-02-02
         */
        QString getCurrentLine();

    protected:
        /**
         * @brief 获取第index行
         * @param[i]  index         行索引
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-02-02
         */
        QString getLine(int index);

        /**
         * @brief 读取一行
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-02-02
         */
        void readALine();

    protected:
        /**
         * @brief 文件指针
         */
        QFile* _file{nullptr};
        /**
         * @brief 文本流
         */
        QTextStream* _textStream{nullptr};
        /**
         * @brief 文件总大小
         */
        qint64 _totalSize{0};
        /**
         * @brief 当前行号
         */
        int _currentLineNum{0};
        /**
         * @brief 当前行内容
         */
        QString _currentLine{};
        /**
         * @brief 百分比记录
         */
        int _percentRecord{ 0 };
        /**
         * @brief 行首位置
         */
        QList<qint64> _linePositions;
        /**
         * @brief 读取的最大行数
         */
        int _readMaxLine{ 0 };
    };
}

#endif