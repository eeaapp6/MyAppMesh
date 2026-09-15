/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractTextReader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <thread>
namespace Interface
{
    bool FITKAbstractTextReader::loadFile()
    {
        QFile f(_fileName);
        //文件存在性
        if (f.exists() && f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while (true)
            {
                //支持两种换行符
                QByteArray context = f.readAll();
                _totalContext = context.split('\n');
                _totalLine = _totalContext.size();
                if (_totalLine > 2) break;

                _totalContext = context.split('\r\n');
                _totalLine = _totalContext.size();
                break;
            }
        }
        f.close();
        //分割失败
        if (_totalLine < 2) return false;
        return true;
    }

    QString FITKAbstractTextReader::readLine()
    {
        return this->getLine(_currentLine++);
    }



    void FITKAbstractTextReader::sendCurrentPercent(int p /*= -1*/)
    {
        if (p < 0)
        {
            //计算百分比
            const float pro = _currentLine / (float)_totalLine;
            p = pro * 100;
        }
        if (_provisionalProgress == p) return;
        _provisionalProgress = p;
        //延迟信号2秒
        if (p == 100) std::this_thread::sleep_for(std::chrono::seconds(2));
        emit sendProcessSig(this, p);
    }

    void FITKAbstractTextReader::backLine()
    {
        //回退行
        --_currentLine;
        if (_currentLine < 0) _currentLine = 0;
    }

    QString FITKAbstractTextReader::nextLine()
    {
        return this->getLine(_currentLine + 1);
    }

    QString FITKAbstractTextReader::previousLine()
    {
        return this->getLine(_currentLine - 1);
    }

    bool FITKAbstractTextReader::atEnd()
    {
        //结束判断
        return _currentLine >= _totalLine;
    }

    int FITKAbstractTextReader::getCurrentLineNum()
    {
        //当前行号，从1开始
        return _currentLine + 1;
    }

    QString FITKAbstractTextReader::getCurrentLine()
    {
        return this->getLine(_currentLine);
    }

    QString FITKAbstractTextReader::getLine(int index)
    {
        //获取行
        if (index < 0 || index >= _totalLine)
            return QString();
        return _totalContext[index];
    }

}

