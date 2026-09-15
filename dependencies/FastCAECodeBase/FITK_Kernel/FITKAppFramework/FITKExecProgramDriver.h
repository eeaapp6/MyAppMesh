/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKExecProgramDriver.h
 * @brief 可执行程序驱动器声明
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-08-19
 * 
 */
#ifndef _FITKEXEC_PROGRAMER_DRIVER_H___
#define _FITKEXEC_PROGRAMER_DRIVER_H___
 
#include "FITKAbstractProgramDriver.h"

class QProcess;

namespace AppFrame
{
    /**
     * @brief 可执行程序驱动
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-19
     */
    class FITKAppFrameworkAPI FITKExecProgramDriver : public FITKAbstractProgramerDriver
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKExecProgramDriver object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        explicit FITKExecProgramDriver();
        /**
         * @brief Destroy the FITKExecProgramDriver object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        virtual ~FITKExecProgramDriver();
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
         * @brief       获取可执行程序的进程ID。
         * @return      进程ID
         * @author      libaojun
         * @date        2026-04-23
         */
        int getProcessID() const;

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
        * @brief 等待程序启动完成
        * @param[i]  msecs          等待的最长时间，单位毫秒，默认30000毫秒（30秒）
        * @return true                程序成功启动
        * @return false               程序启动失败或超时
        * @author libaojun 
        * @date 2026-04-27
        */
        bool waittingForStarted(int msecs = 30000);
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

    private slots:
        /**
         * @brief 发送信息槽函数，procss触发
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        void sendMessageSlot();   
    
    protected:
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
