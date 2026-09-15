/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoImportBase.h
 * @brief       几何导入命令抽象类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-27
 *********************************************************************/

#ifndef __FITKABSGEOIMPORTBASE_H__
#define __FITKABSGEOIMPORTBASE_H__
 
#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"
#include <QString>

namespace Interface
{
    /**
     * @brief       几何导入命令抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-05-15
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoImportBase : public FITKAbsGeoCommand
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        explicit FITKAbsGeoImportBase() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        virtual ~FITKAbsGeoImportBase() = default;

        /**
         * @brief       设置文件名称。
         * @param[in]   fileName：文件名称
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        void setFileName(const QString& fileName);

        /**
         * @brief       获取文件名称。
         * @return      文件名称
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        QString getFileName() const;

    protected:
        /**
         * @brief       文件名称。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        QString _importFileName{};

    };
}

#endif //!__FITKABSGEOIMPORTBASE_H__
