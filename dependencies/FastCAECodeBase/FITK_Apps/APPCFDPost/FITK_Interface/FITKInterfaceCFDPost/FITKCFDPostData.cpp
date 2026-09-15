/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostData.h"
#include "FITKCFDPost3DManager.h"
#include "FITKCFDPostLightManager.h"
#include "FITKCFDPostColorLibaryDataManager.h"

#include <QDir>
#include <QCoreApplication>

namespace Interface
{
    FITKCFDPostData::FITKCFDPostData()
    {
        _manager = new FITKCFDPost3DManager();
        _postLightManager = new FITKCFDPostLightManager();
        _colorLibManager = new FITKCFDPostColorLibaryDataManager();

        init();
    }

    FITKCFDPostData::~FITKCFDPostData()
    {
        if (_manager) {
            delete _manager;
            _manager = nullptr;
        }
        if (_postLightManager) {
            delete _postLightManager;
            _postLightManager = nullptr;
        }
        if (_colorLibManager) {
            delete _colorLibManager;
            _colorLibManager = nullptr;
        }
    }

    FITKCFDPost3DManager* FITKCFDPostData::getPostDataManager()
    {
        return _manager;
    }

    FITKCFDPostLightManager* FITKCFDPostData::getPostLightManager()
    {
        return _postLightManager;
    }

    FITKCFDPostColorLibaryDataManager * FITKCFDPostData::getPostColorLibManager()
    {
        return _colorLibManager;
    }

    void FITKCFDPostData::init()
    {
        QString directoryPath = QCoreApplication::applicationDirPath()+"/ColorLibary";
        QDir directory(directoryPath);

        QStringList filters;
        filters << "*.json";
        directory.setNameFilters(filters);

        // 获取所有符合条件的文件
        QStringList jsonFiles = directory.entryList(filters, QDir::Files);

        for (QString json : jsonFiles) {
            Interface::FITKCFDPostColorLibaryData* colorData = new Interface::FITKCFDPostColorLibaryData();
            if (!colorData->readColorJson(directoryPath + "/" + json)) {
                delete colorData;
                continue;
            }
            _colorLibManager->appendDataObj(colorData);
        }
    }
}

