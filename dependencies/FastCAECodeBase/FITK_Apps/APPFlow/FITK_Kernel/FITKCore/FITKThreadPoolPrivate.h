/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKThreadPoolPrivate.h
 * @brief  封装线程池
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-02
 *
 */
#ifndef __FITKTHREADPOOL_PRIVATE_H___
#define __FITKTHREADPOOL_PRIVATE_H___

#include "FITKAbstractObject.hpp"
#include <QObject>
#include <QList>
#include <QThreadPool>


namespace AppFrame
{
    class FITKSignalTransfer;
}

namespace Core
{
    class FITKThreadTask;
    /**
     * @brief 封装qt线程池
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    class FITKThreadPoolPrivate : public QObject, public FITKAbstractObject
    {
        Q_OBJECT
        friend FITKThreadTask;

    public:
       /**
        * @brief Construct a new FITKThreadPoolPrivate object
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-02
        */
        explicit FITKThreadPoolPrivate() = default;
        /**
         * @brief Destroy the FITKThreadPoolPrivate object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        ~FITKThreadPoolPrivate();
        /**
         * @brief 在线程池中执行任务
         * @param[i]  task           需要执行的任务
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void addTask(FITKThreadTask* task);
       /**
        * @brief 获取任务数量
        * @return int
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-02
        */
        int taskCount();
        /**
         * @brief 等待全部任务结束
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void waitForFinished();
        #ifdef FITKUseAppFramework
        /**
         * @brief  设置信号转接器，用于转接进度信号
         * @param  sigTransfer
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void setSignalTransfer(AppFrame::FITKSignalTransfer* sigTransfer);
        #endif
        /**
         * @brief  是否包含任务
         * @param[i]  task 线程任务
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        bool isContains(FITKThreadTask* task);

    private:
       /**
        * @brief 移除任务，任务结束之后自动调用
        * @param[i]  task           需要移除的任务
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-02
        */
        void removeTask(FITKThreadTask* task);

    private:
        /**
         * @brief 线程任务列表，正在执行或排队的任务
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        QList<FITKThreadTask*> _taskList{};
        /**
         * @brief 线程池
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        QThreadPool _threadPool{};
        #ifdef FITKUseAppFramework
        /**
         * @brief  信号转接器，用于转接进度信号 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        AppFrame::FITKSignalTransfer* _sigTransfer{};
        #endif
    };
}


#endif
