/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKThreadPoolPrivate.h"
#include "FITKThreadTask.h"

#ifdef FITKUseAppFramework
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#endif

namespace Core
{
    //
    FITKThreadPoolPrivate::~FITKThreadPoolPrivate()
    {
    }

    void FITKThreadPoolPrivate::addTask(FITKThreadTask* task)
    {
        if (task == nullptr) return;

        //关联进度信号
        #ifdef FITKUseAppFramework
        if (_sigTransfer)
            connect(task, SIGNAL(sendProcessSig(QObject*, int)), _sigTransfer, SIGNAL(sendProcessSig(QObject*,int)));
        #endif

        //追加到列表
        _taskList.append(task);
        //线程池启动任务
        _threadPool.start(task);
    }

    int FITKThreadPoolPrivate::taskCount()
    {
        return _taskList.size();
    }

    void FITKThreadPoolPrivate::waitForFinished()
    {
        //等待线程池全部结束
        _threadPool.waitForDone();
    }
#ifdef FITKUseAppFramework
    void FITKThreadPoolPrivate::setSignalTransfer(AppFrame::FITKSignalTransfer* sigTransfer)
    {
        _sigTransfer = sigTransfer;
    }
#endif

    bool FITKThreadPoolPrivate::isContains(FITKThreadTask* task)
    {
        return _taskList.contains(task);
    }

    void FITKThreadPoolPrivate::removeTask(FITKThreadTask* task)
    {
        if (task == nullptr) return;
        //从列表移除
        _taskList.removeOne(task);
    }

}