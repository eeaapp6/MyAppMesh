/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractQtXMLWriter.h"
#include <QFile>
#include <QDomDocument>


namespace Interface
{

    FITKAbstractQtXMLWriter::~FITKAbstractQtXMLWriter()
    {
        // Close file
        this->closeFile();
        //释放
        if (_document) delete _document;
        if (_file) delete _file;
    }

    bool FITKAbstractQtXMLWriter::createContent()
    {
        //打开文件
        QFile *_file = new QFile(_fileName); 
        if (!_file->open(QFile::WriteOnly | QFile::Truncate)) return false;
        //创建对象
        _document = new QDomDocument;

        return _document != nullptr;
    }


    void FITKAbstractQtXMLWriter::closeFile()
    {
        if (_file) _file->close();
    }

}



