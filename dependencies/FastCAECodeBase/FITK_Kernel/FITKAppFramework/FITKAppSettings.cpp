/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAppSettings.h"
#include <QSettings>
#include <QCoreApplication>

namespace AppFrame
{
    FITKAppSettings::FITKAppSettings()
    {
        //获取可执行程序路径
        _appDir = QCoreApplication::applicationDirPath();
    }

    FITKAppSettings::~FITKAppSettings()
    {
        if(_settings)
            delete _settings;
    }

    void FITKAppSettings::setValue(const QString& key, const QVariant& varient)
    {
        // 写入
        _settings->setValue(key, varient);
    }

    void FITKAppSettings::read()
    {
        //读取
    }

    void FITKAppSettings::write()
    {
        //同步缓存
        if(_settings)
            _settings->sync();

    }

    QVariant FITKAppSettings::getVarient(const QString& k)
    {
        //读取参数
        if (_settings == nullptr)
            return QVariant();
        return _settings->value(k);
    }

    QString FITKAppSettings::getWorkingDir()
    {
        //工作目录
        return this->getValue<QString>("WorkingDir");
    }

    void FITKAppSettings::setWorkingDir(const QString& wd)
    {
        _settings->setValue("WorkingDir", wd);
    }

    void FITKAppSettings::setPlugins(const QStringList& pathList)
    {
        _settings->setValue("Plugins", pathList);
    }

   QStringList FITKAppSettings::getPlugins()
    {
       return this->getValue<QStringList>("Plugins");

    }

    void FITKAppSettings::setLoadZHCN(bool load)
    {
        _settings->setValue("LoadZHCN", load);
    }

    bool FITKAppSettings::isLoadZHCN() const
    {
        return _settings->value("LoadZHCN", false).toBool();
    }

   QSettings* FITKAppSettings::getSettings()
   {
       return _settings;
   }

   bool FITKAppSettings::isContains(const QString &key) const
   {
       if (_settings == nullptr)
           return false;
       return _settings->contains(key);
   }
}


