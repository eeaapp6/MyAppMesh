/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKLinuxShellDriver.h"
#include "FITKAppFramework.h"
#include "FITKAbstractCommandRunner.h"
#include <QFileSystemWatcher>
#include <QDebug>
namespace AppFrame
{
    FITKLinuxShellDriver::FITKLinuxShellDriver()
    {
        if (_commandRunner != nullptr) return;
        //创建指针 绑定信号
        _commandRunner = new AppFrame::FITKLinuxCommandRunner();
        connect(_commandRunner, &FITKAbstractCommandRunner::commandOutput, this, &FITKLinuxShellDriver::sendOutputMessage);
    }

    FITKLinuxShellDriver::~FITKLinuxShellDriver()
    {
        if (!_commandRunner || _processId == -1) return;
        if (_commandRunner->isProcessRunning(_processId))
        {
            _commandRunner->killProcess(_processId);
        }
        //断开槽 销毁
        disconnect(_commandRunner);
        delete _commandRunner;
        _commandRunner = nullptr;
    }

    void FITKLinuxShellDriver::sendOutputMessage(const QString &str)
    {
        qDebug() << "100101 :" << str;
        //发送消息到日志输出
        sendMessage(100101, str);
    }

}

