/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractRapidXMLReader.h" 
#include <QFile>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
 

namespace Interface
{
    FITKAbstractRapidXMLReader::~FITKAbstractRapidXMLReader()
    {
        if (_file) delete _file;
        if (_document) delete _document;
    }


    bool FITKAbstractRapidXMLReader::createContent(QString & errorMessage)
    {
        //文件存在
        if (!QFile::exists(_fileName)) return false;
        //错误处理
        if (_file) delete _file;
        if (_document) delete _document;

        //读取文件
        QByteArray ba = _fileName.toLocal8Bit();
       
        //生成树
        try
        {
            _file = new rapidxml::file<>(ba.data());
            _document = new rapidxml::xml_document<char>;
            _document->parse<0>(_file->data());
        }
        catch (rapidxml::parse_error e)
        {
            //错误信息
            errorMessage = QString(tr("XML File error : %1 at %2")).arg(e.what()).arg(e.where<char>());
            return false;
        }
        

        return _document != nullptr;
    }

    void FITKAbstractRapidXMLReader::sendCurrentPercent(int p /*= -1*/)
    {
      
        emit sendProcessSig(this, p);
    }

}



