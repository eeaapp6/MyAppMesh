/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKThreadTask.h
 * @brief 线程任务抽象类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-02
 *
 */
#ifndef __FITKTHREAD_H____
#define __FITKTHREAD_H____

#include "FITKCoreAPI.h"
#include "FITKAbstractObject.hpp"
#include <QRunnable>
#include <QObject>
#include <QString>

namespace Core
{
    class FITKThreadTaskGroup;

    /**
     * @brief 线程任务抽象类声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    class FITKCoreAPI FITKThreadTask : public QObject, public QRunnable, public FITKAbstractObject
    {
        Q_OBJECT
            friend FITKThreadTaskGroup;
    public:
        /**
         * @brief Construct a new FITKThreadTask object
         * @param[i]  parent         父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        explicit FITKThreadTask(QObject* parent = nullptr);
        virtual ~FITKThreadTask() = 0;
        /**
         * @brief 设置是否成功的标记
         * @param[o]  success       成功标记
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void setResultMark(bool *success);

    signals:
        /**
         * @brief 线程进度信号 0~100
         * @param[o]  sender         发送的进程任务
         * @param[o]  process        线程进度 0-100
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void sendProcessSig(QObject* sender, int process);
//         /**
//          * @brief 线程发送出的信息
//          * @param[o]  message       发送的信息
//          * @author LiBaojun (libaojunqd@foxmail.com)
//          * @date 2024-03-02
//          */
//         void sendMessageSig(QString message);
        /**
         * @brief 线程结束信号，析构自动触发
         * @param[o]  ta            线程结束的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        void taskFinishedSig(FITKThreadTask* ta);

    private:
        /**
         * @brief 设置所属的组，添加到组的时候自动调用
         * @param[i]  group          任务组
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        void setTaskGroup(FITKThreadTaskGroup* group);

    protected:
        /**
         * @brief 结果是否成功标记
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-18
         */
        bool* _resultMark{ nullptr };
        /**
         * @brief 任务属于的任务组
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-18
         */
        FITKThreadTaskGroup* _taskGroup{ nullptr };
    };

}
#endif
