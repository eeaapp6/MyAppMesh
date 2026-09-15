/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossLSDynaIOInterface.h"
#include "FITKRadiossLSDynaReader.h"
#include "FITKRadiossLSDynaWriter.h"
#include <QThread>


namespace Radioss
{
    bool FITKRadiossLSDynaIOInterface::exec(const int indexPort)
    {
        //如果正在执行，则退出，不能同时执行多个任务
        _success = false;
        if (this->isRunning()) return false;

        switch (indexPort)
        {
        case 1:  //读取
            _io = new FITKRadiossLSDynaReader(_success);
            break;
        case 2:  //写出
            _io = new FITKRadiossLSDynaWriter(_success);
            break;
        default: break;
        }
        //参数错误，直接退出
        if (_io == nullptr) return false;
        _io->setDataObject(this->getDataObject("RadiossCaseData"));
        _io->setFileName(_fileName);
        //在线程中执行
        if (_runInThread)
        {
            connect(_io.getObjectPtr(), SIGNAL(taskFinishedSig(FITKThreadTask*)), this, SIGNAL(ioThreadFinishedSig()));
            _io->push2ThreadPool();
            QThread::msleep(100);
        }
        //不在线程执行
        else
        {
            _io->run();
            auto p = _io.getObjectPtr();
            if (p) delete p;
        }
        
        //参数传递完成，清空参数
        this->resetDataObjects();
        return _success;
    }

    void FITKRadiossLSDynaIOInterface::setFileName(const QString& fileName)
    {
        // Implementation to set the file name
        _fileName = fileName;
    }

    void FITKRadiossLSDynaIOInterface::runInThread(const bool & inThread)
    {
        // Implementation to set whether to run in a separate thread
        _runInThread = inThread;
    }

    bool FITKRadiossLSDynaIOInterface::isRunning()
    {
        // Implementation to check if it is running
        return _io != nullptr;
    }

    bool FITKRadiossLSDynaIOInterface::isSuccess()
    {
        // Implementation to check if the execution was successful
        return _success;
    }

    QString FITKRadiossLSDynaIOInterface::getComponentName()
    {
        return "FITKRadiossLSDynaIO"; // Return the component name
    }

}