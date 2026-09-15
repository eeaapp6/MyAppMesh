/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKCommandLineHandler.h
 * @brief 命令行处理器声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-06
 *
 */
#ifndef __FITKCOMMANDLINE_HANDLER_H___
#define __FITKCOMMANDLINE_HANDLER_H___

#include "FITKAppFrameworkAPI.h"
#include <QStringList>

namespace AppFrame
{
    /**
     * @brief 命令行处理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-06
     */
    class FITKAppFrameworkAPI FITKCommandLineHandler
    {
    public:
        /**
         * @brief Construct a new FITKCommandLineHandler object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-06
         */
        explicit FITKCommandLineHandler();
        /**
         * @brief Destroy the FITKCommandLineHandler object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-06
         */
        virtual ~FITKCommandLineHandler() = default;
        /**
         * @brief 执行处理命令行命令
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-06
         */
        virtual void exec();
        /**
         * @brief 是否无gui运行
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-06
         */
        virtual bool isNoGUI();
        /**
         * @brief 是否打印库信息
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-06-26
         */
        virtual bool isPrintLibInfo();
        /**
         * @brief 是否显示帮助信息
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-06-26
         */
        virtual bool isHelp();
        /**
         * @brief 获取第index个参数
         * @param[i]  index          索引
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-06
         */
        QString argValue(const int index);
        /**
         * @brief 是否包含变量
         * @param[i] v 变量名称 
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-06
         */
        bool containsValue(QString v);
        /**
         * @brief 打印帮助信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-06-26
         */
        virtual void printHelp();

    protected:
        /**
         * @brief 全部命令行参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-06
         */
        QStringList _args{};

    };
}


#endif
