/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKPHengLEIExecProgramDriver.h
 * @brief 可执行程序驱动器声明
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-08-19
 * 
 */
#ifndef _FITK_PHengLEI_EXEC_PROGRAMER_DRIVER_H___
#define _FITK_PHengLEI_EXEC_PROGRAMER_DRIVER_H___
 
#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include <QProcess>

class QProcess;

namespace Interface
{
    /**
     * @brief 可执行程序驱动
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-19
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIExecProgramDriver : public AppFrame::FITKAbstractProgramerDriver
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKExecProgramDriver object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        explicit FITKPHengLEIExecProgramDriver();
        /**
         * @brief Destroy the FITKExecProgramDriver object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        virtual ~FITKPHengLEIExecProgramDriver();
        /**
         * @brief  设置可执行程序的名称
         * @param[i]  program       程序的路径和名称
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        void setExecProgram(const QString & program);
        /**
         * @brief  获取可执行程序的名称
         * @return QString      程序路径和名称
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        QString getExecProgram() const;

        /**
         * @brief       设置可执行程序工作路径。
         * @param[in]   path：工作路径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        void setWorkingDirectory(const QString & path);

        /**
         * @brief       获取可执行程序工作路径。
         * @return      工作路径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        QString getWorkingDirectory() const;

         /**
         * @brief 启动程序
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        void start() override;
        /**
         * @brief 终止程序运行
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        void stop() override;
        /**
         * @brief 是否向外发送信息
         * @param[i]  s              是否发送
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        void enableSendMessage(bool s);
        /**
         * @brief 是否向外发送信息
         * @return true 
         * @return false 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        bool isSendMessage() const;
        /**
         * @brief 获取当前进程状态
         * @return QProcess::ProcessState 进程状态 (NotRunning/Starting/Running)
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-07-18
         */
        QProcess::ProcessState getProcessState() const;
        /**
         * @brief 判断进程是否正在运行
         * @return true 进程正在运行
         * @return false 进程未运行
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-07-18
         */
        bool isProcessRunning() const;
    private slots:
        /**
         * @brief 发送信息槽函数，procss触发
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        void sendMessageSlot();   
    
    private:
        /**
         * @brief 可执行程序名称与路径
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        QString _program{};

        /**
         * @brief       可执行程序工作路径。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        QString _workDir{};

        /**
         * @brief 进程对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        QProcess* _process{};
        /**
         * @brief 发送信息标记位
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        bool _sendMessage{true};
    };
}


#endif
