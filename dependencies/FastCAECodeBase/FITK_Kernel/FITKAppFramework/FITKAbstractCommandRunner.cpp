/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractCommandRunner.h"

#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QString>

#ifdef Q_OS_LINUX

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdexcept>

#endif

namespace AppFrame
{
    FITKAbstractCommandRunner::FITKAbstractCommandRunner(QObject *parent) : Core::FITKThreadTask(parent) {}

    void FITKAbstractCommandRunner::setExecuteCommand(const QString & command)
    {
        _command = command;
    }

    qint64 FITKAbstractCommandRunner::getID()
    {
        return processId;
    }

    QString FITKAbstractCommandRunner::getOutput()
    {
        return _output;
    }

    void FITKAbstractCommandRunner::push2ThreadPool()
    {
        //直接在线程池运行
        Core::FITKThreadPool::getInstance()->execTask(this);
    }

    void FITKAbstractCommandRunner::run()
    {
        RunStatus runStatus;
        _output = executeCommand(_command, processId, runStatus);
    }

    void FITKAbstractCommandRunner::changeDirectory(const QString &path)
    {
        QDir dir(path);
        if (!dir.exists())
        {
            emit commandError("Directory does not exist: " + path);
            return;
        }
        if (!QDir::setCurrent(path))
        {
            emit commandError("Failed to change directory to: " + path);
        }
        else
        {
            emit commandOutput("Changed working directory to:" + path);
        }
    }

    void FITKAbstractCommandRunner::setWorkDirect(const QString &path)
    {
        _workDirect = path;
        changeDirectory(path);
    }

    bool FITKAbstractCommandRunner::isExistDictionary(const QStringList &dicFilePaths)
    {
        bool isExist = true;

        for (QString filePath : dicFilePaths)
        {
            // 使用 QFileInfo 判断是否存在
            QFileInfo checkFile(filePath);
            if (!checkFile.exists())
            {
                isExist = false;
            }
        }
        return isExist;
    }

    bool FITKAbstractCommandRunner::isExistDictionary(const QStringList &dicFilePaths, int &indexError)
    {
        //bool isExist = true;

        for (QString filePath : dicFilePaths)
        {
            // 使用 QFileInfo 判断是否存在
            QFileInfo checkFile(filePath);
            if (!checkFile.exists())
            {
                indexError = dicFilePaths.indexOf(filePath);
                return false;
                //isExist = false;
            }
        }
        return true;
    }

    FITKLinuxCommandRunner::FITKLinuxCommandRunner(QObject *parent) : FITKAbstractCommandRunner(parent) {}

#ifdef Q_OS_LINUX
    QString FITKLinuxCommandRunner::executeCommand(const QString &command, qint64 &pid, RunStatus &runStatus)
    {
        // 数组用于存储管道的文件描述符
        int pipefd[2];

        // 创建管道，如果失败则抛出异常
        if (pipe(pipefd) == -1)
        {
            return QString("Failed to create pipe");
            //throw std::runtime_error("Failed to create pipe");
        }

        // 创建子进程
        pid_t childPid = fork();

        if (childPid == -1)
        {
            // 如果fork失败，则抛出异常
            return QString("Failed to fork process");
            //throw std::runtime_error("Failed to fork process");
        }
        runStatus = RunStatus::Run;
        // 子进程执行的代码
        if (childPid == 0)
        {
            // 关闭管道的读端
            close(pipefd[0]);
            // 将标准输出重定向到管道的写端
            dup2(pipefd[1], STDOUT_FILENO);

            // 将标准错误重定向到管道的写端
            dup2(pipefd[1], STDERR_FILENO);

            // 关闭管道的写端
            close(pipefd[1]);
            // 使用/bin/sh来执行命令，如果execl返回，说明出错，退出子进程
            execl("/bin/sh", "sh", "-c", command.toStdString().c_str(), (char *) nullptr);

            // execl只有在出错时才会返回，因此在这里退出子进程，返回127表示命令未找到
            _exit(127);
        }
        else      // 父进程执行的代码
        {
            // 关闭管道的写端
            close(pipefd[1]);

            // 存储子进程的进程ID
            pid = childPid;
            // 用于存储命令输出
            QString output = nullptr;

            // 缓冲区用于读取管道的数据
            char buffer[256];

            // 读取的字节数
            ssize_t count;

            // 从管道中读取数据，直到读取完毕
            while ((count = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
            {
                // 将缓冲区的最后一个字节设置为字符串结束符
                buffer[count] = '\0';
                emit commandOutput(QString::fromLocal8Bit(buffer));
                // 将缓冲区内容添加到输出字符串
                output += QString::fromLocal8Bit(buffer);
            }

            // 关闭管道的读端
            close(pipefd[0]);

            // 存储子进程的退出状态
            int status;

            // 等待子进程结束
            waitpid(childPid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                runStatus = RunStatus::RunError;
                // 检查子进程是否正常退出并且退出状态不为0
                //qDebug() << QString("Command failed with exit code %1").arg(WEXITSTATUS(status));
                //throw std::runtime_error("Command failed with exit code " + std::to_string(WEXITSTATUS(status)));
            }
            else if (WIFSIGNALED(status))
            {
                runStatus = RunStatus::RunError;
                // 检查子进程是否被信号终止
                //qDebug() << QString("Command killed by signal %1").arg(WEXITSTATUS(status));
                //throw std::runtime_error("Command killed by signal " + std::to_string(WTERMSIG(status)));
            }
            runStatus = RunStatus::Stop;
            return output;
        }
    }

    bool FITKLinuxCommandRunner::isProcessRunning(qint64 pid)
    {
        if (kill(static_cast<pid_t>(pid), 0) == 0)
        {
            // kill 返回 0，表示进程存在
            return true;
        }
        else
        {
            // kill 返回非零值，表示存在错误
            if (errno == EPERM)
            {
                // 如果错误号是 EPERM，表示没有权限检查进程
                emit commandError("No permission to check process " + QString::number(pid));
            }
            else if (errno == ESRCH)
            {
                // 如果错误号是 ESRCH，表示进程不存在
                return false;
            }
            return false;
        }
    }

    bool FITKLinuxCommandRunner::killProcess(qint64 pid)
    {
        // 发送SIGTERM信号给指定进程
        if (kill(static_cast<pid_t>(pid), SIGTERM) == 0)
        {
            emit commandOutput("Successfully sent SIGTERM to process " + QString::number(pid));
            return true;
        }
        else
        {
            emit commandError("Error sending SIGTERM");

            // 如果需要强制终止，可以发送SIGKILL信号
            if (kill(static_cast<pid_t>(pid), SIGKILL) == 0)
            {
                emit commandOutput("Successfully sent SIGKILL to process " + QString::number(pid));
                return true;
            }
            else
            {
                emit commandError("Error sending SIGKILL");
                return false;
            }
        }
    }
#endif

}

