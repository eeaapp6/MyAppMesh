/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file    FITKPHengLEIInputInfo
 * @brief   PHengLEI输入参数
 * @author  WangKai (wang_starry@outlook.com)
 * @date    2024-07-24
 *
 */
#ifndef FITKPHengLEIInputInfo_H
#define FITKPHengLEIInputInfo_H
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include <QStringList>

namespace PHengLEIDriver
{
    class FITKFlowPHengLEIAPI FITKPHengLEIInputInfo : public AppFrame::FITKProgramInputInfo
    {
    public:
        explicit FITKPHengLEIInputInfo() = default;
        virtual ~FITKPHengLEIInputInfo() = default;

        /**
         * @brief   设置PHengLEI启动参数
         * @param   args  启动参数
         * @author  WangKai (wang_starry@outlook.com)
         * @date    2024-08-22
         */
        void setArgs(const QStringList &args) override;

        /**
         * @brief   获取启动参数
         * @return  QStringList
         * @author  WangKai (wang_starry@outlook.com)
         * @date    2024-08-22
         */
        QStringList args() override;


        /**
         * @brief 设置监控文件
         * @return QStringList
         * @author WangKai (wang_starry@outlook.com)
         * @date 2024-08-22
         */
        virtual void setMonitorPath(QStringList &monitorpath) override;


        /**
         * @brief   获取监控的目录
         * @return  QStringList
         * @author  WangKai (wang_starry@outlook.com)
         * @date    2024-08-22
         */
        QStringList getMonitorPath() override;

        /**
         * @brief 设置监控文件
         * @return QStringList
         * @author WangKai (wang_starry@outlook.com)
         * @date 2024-08-22
         */
        virtual void setMonitorFiles(QStringList &monitorFiles) override;

        /**
         * @brief   获取监控文件
         * @return  QStringList
         * @author  WangKai (wang_starry@outlook.com)
         * @date    2024-08-22
         */
        QStringList getMonitorFiles() override;

    private:
        /**
        * @brief 启动参数
        * @author  WangKai (wang_starry@outlook.com)
        * @date    2024-07-24
        */
        QStringList _args;

        /**
         * @brief 监控文件
         * @author  WangKai (wang_starry@outlook.com)
         * @date    2024-07-24
         */
        QStringList _monitorFiles;

        /**
         * @brief 监控的目录
         * @author  WangKai (wang_starry@outlook.com)
         * @date    2024-07-24
         */
        QStringList _monitorPaths;
    };
}

#endif // FITKPHengLEIInputInfo_H
