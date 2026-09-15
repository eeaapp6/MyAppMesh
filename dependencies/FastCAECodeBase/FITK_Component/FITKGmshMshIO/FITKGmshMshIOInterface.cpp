/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshMshIOInterface.h"
#include "FITKGmshMshIOReader.h"
#include "FITKGmshMshDataProcessor.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
#include <QThread>

FITKLIBINFOREGISTER(FITKGmshMshIO, FITKGMSHMSHIOVERSION);

namespace Gmsh
{
    FITKGmshMshIOInterface::~FITKGmshMshIOInterface()
    {
        //析构
        this->clearGmshMshData();
    }
    QString FITKGmshMshIOInterface::getComponentName()
    {
        //获取组件名称
        return "GmshMshIO";
    }

    void FITKGmshMshIOInterface::setFileName(const QString& fileName)
    {
        //设置文件名称
        _fileName = fileName;
    }

    FITKGmshMshData * FITKGmshMshIOInterface::getGmshMshData()
    {
        //获取Gmsh msh数据
        return _mshDataGmsh;
    }

    void FITKGmshMshIOInterface::setReaderConfig(const ReaderGmshMshConfig & config)
    {
        _readConfig = config;
    }

    bool FITKGmshMshIOInterface::exec(const int indexPort)
    {
        //如果正在执行，则退出，不能同时执行多个任务
        _success = false;
        if (this->isRunning()) return false;
        this->clearGmshMshData();

        switch (indexPort)
        {
        case 1:  //读取
        {
            //创建Gmsh msh数据结构
            FITKGmshMshDataReaderProcessor* mshData = new FITKGmshMshDataReaderProcessor;
            mshData->setReaderConfig(_readConfig);
            //创建读取IO
            FITKGmshMshIOReader* readerIO = new FITKGmshMshIOReader;
            readerIO->setReaderMshData(dynamic_cast<FITKGmshMshDataReaderProcessor*>(mshData));
            readerIO->setComponentManager(dynamic_cast<Interface::FITKComponentManager*>(this->getDataObject("ComponentManager")));
            _io = readerIO;
            _mshDataGmsh = mshData;
            break;
        } 
        case 2:  //写出
            
        default: break;
        }
        //参数错误，直接退出
        if (_io == nullptr) return false;
        _io->setFileName(_fileName);
        _io->setDataObject(this->getDataObject("MeshData"));
        _io->setResultMark(&_success);
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


    void FITKGmshMshIOInterface::runInThread(const bool & inThread /*= true*/)
    {
        _runInThread = inThread;
    }

    bool FITKGmshMshIOInterface::isRunning()
    {
        return _io != nullptr;
    }

    bool FITKGmshMshIOInterface::isSuccess()
    {
        return _success;
    }

    void FITKGmshMshIOInterface::clearGmshMshData()
    {
        //清理Gmsh msh数据
        if (_mshDataGmsh) delete _mshDataGmsh;
        _mshDataGmsh = nullptr;
    }
}


