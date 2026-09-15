/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIExecProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include <QProcess>

namespace Interface
{
    FITKPHengLEIExecProgramDriver::FITKPHengLEIExecProgramDriver()
    {
        //创建进程对象
        _process = new QProcess;
        // _process->setReadChannel(QProcess::StandardOutput);

        //消息输出
        connect(_process, &QProcess::readyRead, this, &FITKPHengLEIExecProgramDriver::sendMessageSlot);
    }

    FITKPHengLEIExecProgramDriver::~FITKPHengLEIExecProgramDriver()
    {
        //杀死进程
        if (!_process) return;
        disconnect(_process, &QProcess::readyRead, this, &FITKPHengLEIExecProgramDriver::sendMessageSlot);
        disconnect(_process, SIGNAL(finished(int)), this, SIGNAL(sig_Finish()));
        _process->kill();
        _process->waitForFinished();
       // 释放进程对象 
 //       delete _process;
 //       _process = nullptr;
    }

    void FITKPHengLEIExecProgramDriver::setExecProgram(const QString & program)
    {
        _program = program;
    }

    QString FITKPHengLEIExecProgramDriver::getExecProgram() const
    {
        return _program;
    }

    void FITKPHengLEIExecProgramDriver::setWorkingDirectory(const QString & path)
    {
        _workDir = path;
    }

    QString FITKPHengLEIExecProgramDriver::getWorkingDirectory() const
    {
        return _workDir;
    } 

    void FITKPHengLEIExecProgramDriver::start()
    {
        //启动进程
        if (_process == nullptr) return;
        QStringList args;
        if (_inputInfo != nullptr)
            args = _inputInfo->args();

        // 设置工作路径。
        if (!_workDir.isEmpty())
        {
            _process->setWorkingDirectory(_workDir);
        }
        //信号关联
        connect(_process, SIGNAL(finished(int)), this, SIGNAL(sig_Finish()));

        _process->start(_program, args);
    }

    void FITKPHengLEIExecProgramDriver::stop()
    {
        //终止进程
        if (_process == nullptr) return;
        _process->kill();
        _process->waitForFinished();
    }

    void FITKPHengLEIExecProgramDriver::enableSendMessage(bool s)
    {
        _sendMessage = s;
    }

    
    bool FITKPHengLEIExecProgramDriver::isSendMessage() const
    {
        return _sendMessage;
    }

    QProcess::ProcessState FITKPHengLEIExecProgramDriver::getProcessState() const
    {
        if (_process == nullptr) {
            return QProcess::NotRunning;
        }
        return _process->state();
    }

    bool FITKPHengLEIExecProgramDriver::isProcessRunning() const
    {
        return getProcessState() == QProcess::Running;
    }

    void FITKPHengLEIExecProgramDriver::sendMessageSlot()
    {
        //读取信息
        if (!_sendMessage ||  _process == nullptr) return;

        QString mess = _process->readAllStandardOutput();
        if (!mess.isEmpty())
        {
            this->sendMessage(0, mess);
        }

        QString err = _process->readAllStandardError();
        if (!err.isEmpty())
        {
            this->sendMessage(0, err);
        }
    }
}
