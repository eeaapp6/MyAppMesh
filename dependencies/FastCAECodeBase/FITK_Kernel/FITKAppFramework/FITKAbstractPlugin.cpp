/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractPlugin.h"

namespace AppFrame
{
    FITKAbstractPlugin::FITKAbstractPlugin(QLibrary* dylibrary)
        :_library(dylibrary)
    {
    }

    FITKAbstractPlugin::~FITKAbstractPlugin()
    {
    }

    void FITKAbstractPlugin::setLibraryPath(const QString& path)
    {
        //文件路径
        _libPath = path;
    }

    QString FITKAbstractPlugin::getLibraryPath() const
    {
        return _libPath;
    }

  
    void FITKAbstractPlugin::install()
    {
        //纯虚函数
    }

    void FITKAbstractPlugin::unInstall()
    {
        //纯虚函数
    }

    QLibrary* FITKAbstractPlugin::getLibrary()
    {
        return _library;
    }

    QString FITKAbstractPlugin::getPluginName()
    {
        return QString();
    }

    bool FITKAbstractPlugin::exec()
    {
        return false;
    }
    bool FITKAbstractPlugin::read(void *filePointer, QString &error, int fileType)
    {
        return false;
    }
    bool FITKAbstractPlugin::write(void *filePointer, QString &error, int fileType)
    {
        return false;
    }

}