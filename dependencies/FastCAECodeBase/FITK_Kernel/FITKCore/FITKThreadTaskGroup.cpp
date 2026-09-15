/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKThreadTaskGroup.h"
#include "FITKThreadPool.h"
#include "FITKThreadTask.h"
//#include <QMutexLocker>
#include <QThread>

namespace Core
{

    void FITKThreadTaskGroup::appendThreadTask(FITKThreadTask* task)
    {
 //       QMutexLocker locker(&_mutex);
        // 判断是否已经加入线程池
        if (_puhsed2ThreadPool || task == nullptr) return;
        _taskList.append(task);
        task->setTaskGroup(this);
    }

    int FITKThreadTaskGroup::getTaskCount()
    {
 //       QMutexLocker locker(&_mutex);
        return _taskList.size();
    }

    FITKThreadTask* FITKThreadTaskGroup::getTaskAt(int index)
    {
 //       QMutexLocker locker(&_mutex);
        // 判断是否越界
        if (index > 0 && index < _taskList.size())
            return _taskList.at(index);
        return nullptr;
    }

    void FITKThreadTaskGroup::push2ThreadPool()
    {
//        QMutexLocker locker(&_mutex);
        auto threadPool = FITKThreadPool::getInstance();
        const int n = _taskList.size();
        //复制任务列表，防止短时间任务结束时size变化
        QList<FITKThreadTask*> tasks = _taskList;
        // 将任务加入线程池
        for (int i =0;i<n; ++i)
        {
            auto task = tasks.at(i);
            if(task == nullptr) continue;
            threadPool->execTask(task);
        }
        // 标记已经加入线程池
        _puhsed2ThreadPool = true;
    }

    void FITKThreadTaskGroup::wait()
    {
 //       QMutexLocker locker(&_mutex);
        while (!_taskList.isEmpty())
        {
            // 等待所有任务完成
            QThread::msleep(30);
        }

    }

    void FITKThreadTaskGroup::removeThreadTask(FITKThreadTask* task)
    {
//        QMutexLocker locker(&_mutex);
        _taskList.removeOne(task);
        if (!_taskList.isEmpty()) return;
        // 任务组结束
        emit threadTaskGroupFinishedSig(this);

    }

}

