/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractTextStreamReader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <thread>
namespace Interface
{
    FITKAbstractTextStreamReader::~FITKAbstractTextStreamReader()
    {
        if (_textStream)
        {
            delete _textStream;
            _textStream = nullptr;
        }
        if (_file)
        {
            if (_file->isOpen())
                _file->close();
            delete _file;
            _file = nullptr;
        }   
    }

    bool FITKAbstractTextStreamReader::loadFile()
    {
        _file = new QFile(_fileName);
        
        //文件存在性
        if (_file->exists() && _file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            _textStream = new QTextStream(_file);
            _totalSize = _file->size();
            _linePositions.clear();
            _linePositions.append(0);
        }
        return true;
    }



    QString FITKAbstractTextStreamReader::readLine()
    {
        
        _currentLine = this->getLine(_currentLineNum);
        _currentLineNum++;
        return _currentLine;
    }

    void FITKAbstractTextStreamReader::sendCurrentPercent(int p /*= -1*/)
    {
        if (p < 0)
        {
            //计算百分比
            const float curpos = static_cast<float>(_textStream->pos());
            const float pro = curpos / _totalSize;
            p = pro * 100;
        }
        
        //延迟信号2秒
        if (p >= 100)
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            p = 100;
        }
        if (p <= _percentRecord) return;
        _percentRecord = p;
        emit sendProcessSig(this, p);
    }

    void FITKAbstractTextStreamReader::backLine()
    {
        _currentLineNum--; 
        _currentLine = this->getLine(_currentLineNum);
    }

    QString FITKAbstractTextStreamReader::nextLine()
    {
        QString line = this->getLine(_currentLineNum + 1);
        return line;
    }

    QString FITKAbstractTextStreamReader::previousLine()
    {
        QString line = this->getLine(_currentLineNum - 1);
        return line;
    }

    bool FITKAbstractTextStreamReader::atEnd()
    {
        //结束判断
        if (!_textStream) return true;
        return _textStream->atEnd() && _currentLineNum == _readMaxLine;
    }

    int FITKAbstractTextStreamReader::getCurrentLineNum()
    {
        return _currentLineNum;
    }

    QString FITKAbstractTextStreamReader::getCurrentLine()
    {
        return _currentLine;
    }

    QString FITKAbstractTextStreamReader::getLine(int index)
    {
        while (index >= _linePositions.size()-1 && !_textStream->atEnd())
        {
            this->readALine();
        }
        if (index >= _linePositions.size())  return QString();

        const int pos = _linePositions.at(index);
        _textStream->seek(pos);
        const QString line = _textStream->readLine();

        return line;
    }

    void FITKAbstractTextStreamReader::readALine()
    {
        if (!_textStream || _textStream->atEnd()) return ;
        //读取一行
        _currentLine = _textStream->readLine();
        //记录行首的位置
        _readMaxLine < _linePositions.size() ? _linePositions.append(_textStream->pos()) : void();
        _readMaxLine++;
    }

}
