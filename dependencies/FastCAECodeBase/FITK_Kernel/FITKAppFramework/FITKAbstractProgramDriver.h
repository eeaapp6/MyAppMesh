/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractProgramDriver.h
 * @brief 第三方程序驱动抽象类声明
 * @author libaojun (libaojunqd@Foxmail.com)
 * @date 2024-06-09
 *
 */
#ifndef  __FITK_ABSTRACTPROGRAM_DRIVER_H__
#define  __FITK_ABSTRACTPROGRAM_DRIVER_H__

#include "FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
#include <QStringList>
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"

class QFileSystemWatcher;
namespace AppFrame
{
    /**
     * @brief 第三方程序驱动输入信息抽象类
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-06-09
     */
    class FITKAppFrameworkAPI FITKProgramInputInfo
        : public Core::FITKAbstractObject, public Core::FITKVarientParams
    {
    public:
        /**
         * @brief Construct a new FITKProgramInputInfo object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        explicit FITKProgramInputInfo() = default;
        /**
         * @brief Destroy the FITKProgramInputInfo object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        virtual ~FITKProgramInputInfo() = 0;
        /**
         * @brief 设置启动参数
         * @param[i]  args           启动参数
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        virtual void setArgs(const QStringList& args);
        /**
         * @brief 获取启动参数
         * @return QStringList
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        virtual QStringList args() = 0;

        /**
         * @brief 设置监控文件
         * @return QStringList
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        virtual void setMonitorFiles(QStringList &monitorFiles);

        /**
         * @brief 获取监控文件
         * @return QStringList
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        virtual QStringList getMonitorFiles();

        /**
         * @brief 设置监控文件
         * @return QStringList
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        virtual void setMonitorPath(QStringList &monitorpath) ;

        /**
         * @brief 获取监控的目录
         * @return QStringList
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        virtual QStringList getMonitorPath();
    };


    /**
     * @brief 第三方程序driver抽象类
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-06-09
     */
    class FITKAppFrameworkAPI FITKAbstractProgramerDriver
        : public Core::FITKAbstractDataObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKAbstractProgramerDriver object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        explicit FITKAbstractProgramerDriver() = default;
        /**
         * @brief Destroy the FITKAbstractProgramerDriver object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        virtual ~FITKAbstractProgramerDriver();
        /**
         * @brief 获取程序类型，抽象值
         * @return int
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        virtual int getProgramType() = 0;
        /**
         * @brief 获取程序名称，任务管理器的名称，不是文件路径
         * @return QString
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        virtual QString getProgramName() = 0;
        /**
         * @brief 设置输入信息
         * @param[i]  info           输入信息
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        virtual void setInputInfo(FITKProgramInputInfo* info);
            /**
         * @brief 获取输入信息
         * @return FITKProgramInputInfo*
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        FITKProgramInputInfo* getProgramInput();
        /**
         * @brief 启动程序
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        virtual void start() = 0;
        /**
         * @brief 终止程序运行
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-07-02
         */
        virtual void stop() = 0;

    signals:
        /**
         * @brief 执行完成
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-07-02
         */
        void sig_Finish();

        
    protected:
        /**
         * @brief 将信息发送到signal transfer
         * @param[i]  messageType    信息类型
         * @param[i]  message        消息内容
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        void sendMessage(int messageType, const QString& message);
        /**
         * @brief 是否开启监控文件与目录
         * @param[i]  m              是否开始文件监控
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        void monitorFileDirectories(bool m = true);

    private slots:
        /**
         * @brief 文件发生了变化处理槽函数
         * @param[i]  file           发生变化的文件
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        void fileChangedSlot(const QString& file);
        /**
         * @brief 文件目录发生变化处理槽函数
         * @param[i]  dir            变化的目录
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        void directoryChangedSlot(const QString& dir);

    protected:
        /**
         * @brief 程序输入
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        FITKProgramInputInfo* _inputInfo{};
        /**
         * @brief 文件监控器声明
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        QFileSystemWatcher* _fileWatcher{};

    };
}

#endif
