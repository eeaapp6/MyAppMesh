/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKDirFileTools.h
 * @brief 文件与路径操作
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-13
 *
 */
#ifndef __FITKDIRFILE_TOOLS_H__
#define __FITKDIRFILE_TOOLS_H__

#include "FITKCoreAPI.h"
#include <QString>

namespace Core
{
    /**
     * @brief
     * @brief 移除目录，子目录与文件将会全部删除，dir目录也会被删除
     * @param dir 需要移除的目录
     * @return true-成功，目录不存在了；false-清理失败
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2023-06-26
     */
    bool FITKCoreAPI RemoveDir(const QString& dir);

    /**
     * @brief 清理目录，子目录与文件将会全部删除，dir目录将不会删除,如果dir不存在则创建该目录
     * @param dir 需要清理的目录
     * @return true-成功；false-清理失败
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2023-06-26
     */
    bool FITKCoreAPI ClearDir(const QString& dir);

    /**
     * @brief 创建目录，创建dir目录，可以创建多级目录
     * @param dir 需要创建的目录
     * @return true-成功；false-失败
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2023-06-26
     */
    bool FITKCoreAPI CreateDir(const QString& dir);
    /**
     * @brief 创建空白的文件
     * @param[i] fileName 需要创建的文件,全路径
     * @return true-成功；false-失败
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-08-31
     */
    bool FITKCoreAPI CreateFile(const QString& fileName);

}  // namespace Common

#endif