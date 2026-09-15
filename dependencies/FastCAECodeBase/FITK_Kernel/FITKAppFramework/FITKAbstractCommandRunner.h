/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file    FITKAbstractCommandRunner
 * @brief   命令运行抽象类
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-07-19
 *
 */
#ifndef FITKAbstractCommandRunner_H
#define FITKAbstractCommandRunner_H

#include "FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"
#include <QObject>
#include <QString>

#ifdef Q_OS_LINUX

#include <sys/types.h>

#endif

namespace AppFrame
{   
    /**
     * @brief 运行状态
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-07-23
     */
    enum class RunStatus
    {
        None = 0,
        Run = 1,
        Stop = 2,
        RunError = 3,
    };

    /**
     * @brief   命令运行抽象类
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-07-19
     */
    class FITKAppFrameworkAPI FITKAbstractCommandRunner : public Core::FITKThreadTask
    {
        Q_OBJECT
    public:
        explicit FITKAbstractCommandRunner(QObject *parent = nullptr);

        virtual ~FITKAbstractCommandRunner() = default;

        /**
         * @brief 判断路径下是否存在文件
         * @param dicFilePaths   文件路径
         * @return  true 存在，false 不存在
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        virtual bool isExistDictionary(const QStringList &FilePaths);

        /**
         * @brief 判断路径下是否存在文件
         * @param dicFilePaths   文件路径
         * @param indexError     路径中错误下标的文件索引
         * @return  true 存在，false 不存在
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        virtual bool isExistDictionary(const QStringList &FilePaths, int &indexError);

        /**
         * @brief   检查进程是否在运行 需要在子类重写
         * @param   pid   进程id号
         * @return  true 运行，false停止运行
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        virtual bool isProcessRunning(qint64 pid) { return false; };

        /**
         * @brief   杀死正在执行的进程 需要在子类重写
         * @param   pid  进程ID
         * @return  true 结束成功
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        virtual bool killProcess(qint64 pid) { return false; };

        /**
         * @brief   设置待执行命令
         * @param   command   命令
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-29
         */
        virtual void setExecuteCommand(const QString &command);

        /**
         * @brief   获取进程号
         * @return  进程号
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-29
         */
        qint64 getID();

        /**
         * @brief   获取完整输出
         * @return  输出
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-29
         */
        QString getOutput();

        /**
         * @brief   推送到线程池执行
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-29
         */
        void push2ThreadPool();

        /**
         * @brief   执行命令并捕获其输出
         * @param   command   命令
         * @param   pid       用于存储子进程的进程ID的引用
         * @return  命令的输出
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        virtual void run() override;

        /**
         * @brief   执行命令并捕获其输出 需要在子类重写
         * @param   command   命令
         * @param   pid       用于存储子进程的进程ID的引用
         * @param   runStatus 运行状态
         * @return  命令的输出
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        virtual QString executeCommand(const QString &command, qint64 &pid, RunStatus &runStatus) { return ""; };

    signals:
        /**
         * @brief   命令输出信息
         * @param   output    输出信息
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        void commandOutput(const QString &output);

        /**
         * @brief   命令错误信号
         * @param   error     错误信息
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        void commandError(const QString &error);

    protected:
        /**
         * @brief   设置工作目录
         * @param   path  工作路径
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-01
         */
        virtual void setWorkDirect(const QString &path);

        /**
         * @brief   改变工作目录
         * @param   path  工作目录
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        virtual void changeDirectory(const QString &path);

    protected:

        /**
         * @brief   进程ID号
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        qint64 processId = -1;

        /**
         * @brief  工作目录
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-01
         */
        QString _workDirect{};

        /**
         * @brief   命令
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-29
         */
        QString _command;

        /**
         * @brief   执行命令的完整输出
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-29
         */
        QString _output;

    };

    /**
     * @brief   Linux命令运行实现
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-07-19
     */
    class FITKAppFrameworkAPI FITKLinuxCommandRunner : public FITKAbstractCommandRunner
    {
        Q_OBJECT
    public:
        explicit FITKLinuxCommandRunner(QObject *parent = nullptr);

        virtual ~FITKLinuxCommandRunner() = default;
#ifdef Q_OS_LINUX
        /**
         * @brief   检查进程是否在运行
         * @param   pid   进程id号
         * @return  true 运行，false停止运行FITKAbstractCommandRunner::
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        virtual bool isProcessRunning(qint64 pid) override;

        /**
         * @brief   杀死正在执行的进程
         * @param   pid  进程ID
         * @return  true 结束成功
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        virtual bool killProcess(qint64 pid) override;

        /**
        * @brief   执行命令并捕获其输出
        * @param   command   要执行的命令FITKAbstractCommandRunner::
        * @param   pid       用于存储子进程的进程ID的引用
        * @param   runStatus
        * @return  命令的输出
        * @author  fulipeng (fulipengqd@yeah.net)
        * @date    2024-07-19
        */
        virtual QString executeCommand(const QString &command, qint64 &pid, RunStatus &runStatus) override;

    protected:

        /**
         * @brief   进程ID号
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-19
         */
        qint64 processId = -1;

        /**
         * @brief  工作目录
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-01
         */
        QString _workDirect{};

#endif

    };
}


#endif // FITKAbstractCommandRunner_H
