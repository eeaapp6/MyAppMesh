/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKThreadPool.h
 * @brief 线程池接口声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-02
 *
 */
#ifndef __FITKTHREADPOOL_H___
#define __FITKTHREADPOOL_H___

#include "FITKCoreAPI.h"
#include "FITKCoreMacros.h"
#include "FITKAbstractObject.hpp"
#include <QMutex>

namespace AppFrame
{
    class FITKSignalTransfer;
}

namespace Core
{
    class FITKThreadPoolPrivate;
    class FITKThreadPoolDeletor;
    class FITKThreadTask;
    /**
     * @brief 线程池声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    class FITKCoreAPI FITKThreadPool : public FITKAbstractObject
    {
        friend FITKThreadPoolDeletor;
        friend FITKThreadTask;
        DeclSingleton(FITKThreadPool);

    public:
        /**
         * @brief 执行任务
         * @param[i]  task          线程池执行任务
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void execTask(FITKThreadTask* task);
        /**
         * @brief 获取线程池中的任务数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        int getTaskCount() const;
        /**
         * @brief 等待全部任务完成
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void wait();
        /**
        * @brief 等待任务完成
        * @param[i] task 等待的任务
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-08-31
        */
        void wait(FITKThreadTask* task);
        /**
         * @brief 初始化，设置信号转接
         * @param[i]  task          信号转接
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void init(AppFrame::FITKSignalTransfer* sigTransfer);

    private:
        /**
         * @brief 初始化
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void initialize();
        /**
         * @brief 结束，析构函数调用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void finalize();
        /**
         * @brief 获取代理器
         * @return FITKThreadPoolPrivate*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        FITKThreadPoolPrivate* getAgent();

    private:
        /**
         * @brief 代理器 封装线程池
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        FITKThreadPoolPrivate* _threadPoolAgent{};
    };


    /**
     * @brief 清理器，软件执行结束自动执行清理
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    class FITKThreadPoolDeletor
    {
    public:
        explicit FITKThreadPoolDeletor() = default;
        ~FITKThreadPoolDeletor();
    };

}



#endif
