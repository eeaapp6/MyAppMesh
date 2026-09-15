/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIInputInfo.h"

namespace PHengLEIDriver
{
    void FITKPHengLEIInputInfo::setArgs(const QStringList &args)
    {
        //设置参数列表
        _args = args;
    }

    QStringList FITKPHengLEIInputInfo::args()
    {
        //获取参数列表
        return _args;
    }

    void FITKPHengLEIInputInfo::setMonitorPath(QStringList & monitorpath)
    {
        //设置监控目录
        _monitorPaths = monitorpath;
    }

    QStringList FITKPHengLEIInputInfo::getMonitorPath()
    {
        //获取监控目录
        return _monitorPaths;
    }

    void FITKPHengLEIInputInfo::setMonitorFiles(QStringList & monitorFiles)
    {
        //设置监控文件列表
        _monitorFiles = monitorFiles;
    }

    QStringList FITKPHengLEIInputInfo::getMonitorFiles()
    {
        //获取监控文件列表
        return _monitorFiles;
    }
}
