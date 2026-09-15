/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKWorkBenchHandler.h
 * @brief workbench处理器
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-08-19
 * 
 */
#ifndef __FITK_WORKBENCH_HANDLER_H___
#define __FITK_WORKBENCH_HANDLER_H___
 
#include "FITKAppFrameworkAPI.h"
#include "FITKAppSettings.h"
#include <QList>

namespace AppFrame
{
    /**
     * @brief 输入输出文件信息
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-26
     */
    struct IOFileInfo 
    {
        QString _path{}; // 文件路径
        QString _name{};  // 文件名
        QString _describe{}; // 文件描述
        QString _suffix{}; // 文件后缀
        bool _isFile{ true }; //是否是文件，false表示是folder文件夹
    };

    /**
     * @brief workbench处理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-19
     */
    class FITKAppFrameworkAPI FITKWorkBenchHandler : public FITKAppSettings
    {
    public:
        /**
         * @brief Construct a new FITKWorkBenchHandler object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        explicit FITKWorkBenchHandler();
        /**
         * @brief Destroy the FITKWorkBenchHandler object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        virtual ~FITKWorkBenchHandler() = default;
        /**
         * @brief 是否在workbench中工作
         * @return true 
         * @return false 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        virtual bool workingInWorkBench();
        /**
         * @brief 执行处理器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        virtual void execHandler() = 0;
        /**
         * @brief 程序结束写出文件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-27
         */
        virtual void execOutput() = 0;
        /**
         * @brief 获取INI文件路径
         * @return QString 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        QString getIniFile() const;
        /**
         * @brief 获取输入文件数量
         * @return int 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        int getInputFilesCount();
        /**
         * @brief 获取输出文件数量
         * @return int 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        int getOutputFilesCount();
        /**
         * @brief 获取第index 个输入文件信息
         * @param[i]  index       索引
         * @return IOFileInfo 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        IOFileInfo getInputFileInfo(const int index);
        /**
         * @brief 获取第index 个输入文件信息
         * @param[i]  index       索引
         * @return IOFileInfo 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        IOFileInfo getOututFileInfo(const int index);
        /**
         * @brief 获取第index 个输出文件信息
         * @return QString 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        QString  getOutputPath();
        /**
         * @brief 是否是工程文件模式
         * @return bool
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        bool isProjectFileMode();
        /**
         * @brief 添加houutput文件信息
         * @param[i]  info           文件信息
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-05
         */
        void appendOutputInfo(const IOFileInfo& info);
        /**
         * @brief 获取输出信息数量
         * @return int 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-05
         */
        int getOutputInfoCount();
        /**
         * @brief 获取第index个输出文件信息
         * @param[i]  index          索引
         * @return IOFileInfo  index越界返回空对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-05
         */
        IOFileInfo getOutputInfoAt(const int index);
        /**
         * @brief  移除输出文件信息
         * @param[i]  index          输出文件信息
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-05
         */
        void removeOutputInfo(int index);
        /**
         * @brief  清空输出文件信息
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-05
         */
        void clearOutputInfo();
        /**
         * @brief 写出INI文件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-05
         */
        virtual void write() override;

    protected:
        /**
         * @brief 读取ini文件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        void read() override;

    protected:
        /**
         * @brief 是否在workbench中工作
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-26
         */
        bool _workingInWorkBench{ false };
       /**
        * @brief ini文件
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-08-26
        */
        QString _iniFile{};
        /**
        * @brief 输出文件信息
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-08-26
        */
        QList<IOFileInfo> _outputInfos{};

    };
}


#endif
