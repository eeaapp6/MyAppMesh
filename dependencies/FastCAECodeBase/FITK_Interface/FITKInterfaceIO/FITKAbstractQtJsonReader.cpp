/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractQtJsonReader.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>

namespace Interface
{

    FITKAbstractQtJsonReader::~FITKAbstractQtJsonReader()
    {
        this->closeFile();
        if (_document) delete _document;
        if (_file) delete _file;
    }

    bool FITKAbstractQtJsonReader::createContent()
    {
        if (_document)
        {
            delete _document;
            _document = nullptr;
        }
        if (_file)
        {
            delete _file;
            _file = nullptr;
        }

        _file = new QFile(_fileName);
        if (!_file->open(QFile::ReadOnly)) return false;

        QJsonParseError parseError{};
        _document = new QJsonDocument(QJsonDocument::fromJson(_file->readAll(), &parseError));
        if (parseError.error != QJsonParseError::NoError)
        {
            delete _document;
            _document = nullptr;
            this->closeFile();
            return false;
        }

        this->closeFile();
        return true;
    }

    void FITKAbstractQtJsonReader::closeFile()
    {
        if (_file) _file->close();
    }

}