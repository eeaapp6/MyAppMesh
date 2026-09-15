/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file    FITKOFInputInfo
 * @brief   openFoam输入参数类
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-07-24
 *
 */
#ifndef FITKOFINPUTINFO_H
#define FITKOFINPUTINFO_H
#include "FITKOFDriverAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include <QStringList>

namespace FoamDriver
{
    class FITKOFDRIVERAPI FITKOFInputInfo : public AppFrame::FITKProgramInputInfo
    {
    public:
        explicit FITKOFInputInfo() = default;
        virtual ~FITKOFInputInfo() = default;

        /**
         * @brief   设置blockMesh启动参数
         * @param   args  启动参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-02
         */
        void setArgs(const QStringList &args) override;

        /**
         * @brief   获取启动参数
         * @return  QStringList
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-02
         */
        QStringList args() override;


        /**
         * @brief 设置监控文件
         * @return QStringList
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        virtual void setMonitorPath(QStringList &monitorpath) override;


        /**
         * @brief   获取监控的目录
         * @return  QStringList
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-02
         */
        QStringList getMonitorPath() override;

        /**
         * @brief 设置监控文件
         * @return QStringList
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-15
         */
        virtual void setMonitorFiles(QStringList &monitorFiles) override;

        /**
         * @brief   获取监控文件
         * @return  QStringList
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-02
         */
        QStringList getMonitorFiles() override;

    private:
        /**
        * @brief 启动参数
        * @author  fulipeng (fulipengqd@yeah.net)
        * @date    2024-07-24
        */
        QStringList _args;

        /**
         * @brief 监控文件
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-24
         */
        QStringList _monitorFiles;

        /**
         * @brief 监控的目录
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-24
         */
        QStringList _monitorPaths;
    };
}

#endif // FITKOFInputInfo_H
