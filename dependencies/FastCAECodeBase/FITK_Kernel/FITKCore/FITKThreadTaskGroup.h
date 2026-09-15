/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKThreadTaskGroup.h
 * @brief 定义多线程任务组
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-13
 *
 */
#ifndef __FITKTHREADTASK_GROUP_H__
#define __FITKTHREADTASK_GROUP_H__

#include "FITKCoreAPI.h"
#include <QObject>
#include "FITKAbstractObject.hpp"
#include <QMutex>
#include <QList>

namespace Core
{
    class FITKThreadTask;
    /**
     * @brief 多线程任务组
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-13
     */
    class FITKCoreAPI FITKThreadTaskGroup : public QObject, public Core::FITKAbstractObject
    {
        Q_OBJECT
        friend FITKThreadTask;

    public:
        /**
         * @brief Construct a new FITKThreadTaskGroup object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        explicit FITKThreadTaskGroup() = default;
        /**
         * @brief Destroy the FITKThreadTaskGroup object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        virtual ~FITKThreadTaskGroup() = default;
        /**
         * @brief 追加任务 推送后不能再追加任务
         * @param[i]  task           任务指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        virtual void appendThreadTask(FITKThreadTask* task);
        /**
         * @brief 获取任务数量
         * @return int 任务数量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        int getTaskCount();
        /**
         * @brief 获取任务
         * @param[i]  index           任务索引
         * @return FITKThreadTask* 任务指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        FITKThreadTask* getTaskAt(int index);
        /**
         * @brief 推送任务到线程池 推送后不能再追加任务
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        void push2ThreadPool();
        /**
         * @brief 等待任务结束
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        void wait();

    signals:
        /**
         * @brief 任务组结束信号
         * @param[o]  group           任务组指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        void threadTaskGroupFinishedSig(FITKThreadTaskGroup* group);

    private:
        /**
         * @brief 移除任务
         * @param[i]  task           任务指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        void removeThreadTask(FITKThreadTask* task);

    protected:
        /**
         * @brief 任务列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        QList<FITKThreadTask*> _taskList{};
        /**
         * @brief 是否已经推送到线程池,推送后不能再追加任务
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        bool _puhsed2ThreadPool{ false };
    };

}


#endif
