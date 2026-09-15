/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKWorkbenchIniWriter.h
 * @brief workbench ini 文件写出器
 * @author libaojunAI
 * @date 2026-04-28
 *
 */
#ifndef __FITK_WORKBENCH_INI_WRITER_H___
#define __FITK_WORKBENCH_INI_WRITER_H___

#include "FITKAppFrameworkAPI.h"
#include "FITKWorkBenchHandler.h"

#include <QList>
#include <QString>

namespace AppFrame
{
    /**
     * @brief workbench ini 文件写出器
     * @author libaojunAI
     * @date 2026-04-28
     */
    class FITKAppFrameworkAPI FITKWorkbenchIniWriter
    {
    public:
        /**
         * @brief Construct a new FITKWorkbenchIniWriter object
         * @param[i] iniFile ini 文件路径
         * @author libaojunAI
         * @date 2026-04-28
         */
        explicit FITKWorkbenchIniWriter(const QString& iniFile = QString());
        /**
         * @brief Destroy the FITKWorkbenchIniWriter object
         * @author libaojunAI
         * @date 2026-04-28
         */
        virtual ~FITKWorkbenchIniWriter() = default;

        /**
         * @brief 设置 ini 文件路径
         * @param[i] iniFile ini 文件路径
         * @author libaojunAI
         * @date 2026-04-28
         */
        void setIniFile(const QString& iniFile);
        /**
         * @brief 获取 ini 文件路径
         * @return QString
         * @author libaojunAI
         * @date 2026-04-28
         */
        QString getIniFile() const;
        /**
         * @brief 追加输入文件信息
         * @param[i] info 文件信息
         * @author libaojunAI
         * @date 2026-04-28
         */
        void appendInputFileInfo(const IOFileInfo& info);
        /**
         * @brief 追加输出文件信息
         * @param[i] info 文件信息
         * @author libaojunAI
         * @date 2026-04-28
         */
        void appendOutputFileInfo(const IOFileInfo& info);
        /**
         * @brief 清空输入文件信息
         * @author libaojunAI
         * @date 2026-04-28
         */
        void clearInputFileInfos();
        /**
         * @brief 清空输出文件信息
         * @author libaojunAI
         * @date 2026-04-28
         */
        void clearOutputFileInfos();
        /**
         * @brief 写出 ini 文件中的输入输出文件信息
         * @return bool 是否写出成功
         * @author libaojunAI
         * @date 2026-04-28
         */
        bool write();

    private:
        QString _iniFile{};
        QList<IOFileInfo> _inputInfos{};
        QList<IOFileInfo> _outputInfos{};
    };
}

#endif