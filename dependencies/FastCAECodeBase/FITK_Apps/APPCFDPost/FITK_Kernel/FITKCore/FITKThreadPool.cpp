/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKThreadPool.h"
#include "FITKThreadPoolPrivate.h"
#include <QMutex>
#include "FITKAbstractObjectContainer.hpp"
#include "FITKThreadTask.h"

namespace Core
{
    //静态变量初始化
    static FITKThreadPoolDeletor FITKTPDeletor;
    FITKThreadPool* FITKThreadPool::_instance = nullptr;
    QMutex FITKThreadPool::m_mutex;

    FITKThreadPoolDeletor::~FITKThreadPoolDeletor()
    {
        //回收代理器内存
        auto d = FITKThreadPool::getInstance()->_threadPoolAgent;
        delete d;
        FITKThreadPool::getInstance()->_threadPoolAgent = nullptr;
    }

    void FITKThreadPool::execTask(FITKThreadTask* task)
    {
        //错误判断
        if (task == nullptr || _threadPoolAgent == nullptr) return;

        _threadPoolAgent->addTask(task);
    }

    int FITKThreadPool::getTaskCount() const
    {
        return _threadPoolAgent->taskCount();
    }

    void FITKThreadPool::wait()
    {
        //等待线程全部结束
        _threadPoolAgent->waitForFinished();
    }

    void FITKThreadPool::wait(FITKThreadTask* task)
    {
        if (task == nullptr || !_threadPoolAgent->isContains(task)) return;
        FITKAbstractObjectContainer<FITKThreadTask> c(task);
        while (c.getObjectPtr() != nullptr)
        {
            QThread::msleep(50);
        }
    }

    void FITKThreadPool::init(AppFrame::FITKSignalTransfer* sigTransfer)
    {
        if(_threadPoolAgent)
            _threadPoolAgent->setSignalTransfer(sigTransfer);
    }

    void FITKThreadPool::initialize()
    {
        //初始化
        _threadPoolAgent = new FITKThreadPoolPrivate;
    }

    void FITKThreadPool::finalize()
    {

    }

    FITKThreadPoolPrivate* FITKThreadPool::getAgent()
    {
        return _threadPoolAgent;
    }

}



