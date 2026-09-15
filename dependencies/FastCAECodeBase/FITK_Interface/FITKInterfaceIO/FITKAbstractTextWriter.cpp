/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractTextWriter.h"
#include <iomanip>
#include <math.h>

namespace Interface
{
    //打开文件流
    bool FITKAbstractTextWriter::openFileStream()
    {
        _file = new QFile(_fileName);
        if (_file->exists())_file->remove();
        if (!_file->open(QIODevice::ReadWrite | QIODevice::NewOnly))
        {
            closeFileStream();
            return false;
        }
        _stream = new QTextStream(_file);
        return true;
    }
    //关闭文件流
    bool FITKAbstractTextWriter::closeFileStream()
    {
        if (_stream != nullptr)
        {
            delete _stream;
            _stream = nullptr;
        }

        if (_file != nullptr)
        {
            _file->close();
            delete _file;
            _file = nullptr;
        }
        return true;
    }
    
    QString FITKAbstractTextWriter::double2String(double d, int prec, int width1, char f)
    {
        QRegExp reg;
        QString strNumber = QString("%1").arg(d, 0, f, prec);
        QString rtStr = strNumber;
        //判断0 
        if (fabs(d) < 1e-16) 
        {
            strNumber = "0.";
        }
        else if (f == 'g' && (fabs(d) >= 1e+6 || fabs(d) <= 1e-5))
        {
            strNumber = QString("%1").arg(d, 0, 'e', prec - 1);
        }

        if (strNumber.contains("e"))
        {
            reg.setPattern("0+\\e");
            rtStr = strNumber.replace(reg, "e");
        }
        else if (strNumber.contains("E"))
        {
            reg.setPattern("0+\\E");
            rtStr = strNumber.replace(reg, "e");
        }
        else if (!strNumber.contains("."))
        {
            rtStr = strNumber + ".";
        }
        else
        {
            reg.setPattern("0+$");
            rtStr = strNumber.replace(reg, "");
        }

        rtStr = rtStr.replace(".e", "e");
        rtStr = rtStr.rightJustified(width1, ' ');
        return rtStr;
    }
    QString FITKAbstractTextWriter::int2String(int d, int width1)
    {
        QString rtStr = QString::number(d);
        return rtStr.rightJustified(width1, ' ');;
    }
}
