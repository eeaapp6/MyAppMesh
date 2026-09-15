/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file GlobalFiles.h
 * @brief 文件相关的python接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2026-05-07
 *
 */
#ifndef __PYTHONINTERFACE_GLOBALFILES_H___
#define __PYTHONINTERFACE_GLOBALFILES_H___

#include <QString>
#include <QObject>
#include "PythonInterfaceGlobalAPI.h"

class Files;

/**
 * @brief 文件相关PYTHON接口装饰
 * @author LiBaoJun (libaojunqd@foxmail.com)
 * @date 2026-05-07
 */
class PythonInterfaceGlobalAPI FilesWapper : public QObject
{
    Q_OBJECT
public slots:
    /**
     * @brief 创建 Files 实例
     * @return 新分配的 Files 指针
     */
    Files* new_Files();

    /**
     * @brief 释放 Files 实例
     * @param F 需要释放的 Files 指针
     */
    void delete_Files(Files* F);

    /**
     * @brief 导入 rad 文件
     * @param fileName 待导入的 rad 文件路径
     */
    QString static_Files_ImportRadFile(const QString& fileName);

    /**
     * @brief 导出 rad 文件
     * @param fileName 导出目标 rad 文件路径
     */
    QString static_Files_ExportRadFile(const QString& fileName);

    /**
     * @brief 导入 网格（inp，bdf ，nas） 文件
     * @param fileName 待导入的 网格 文件路径
     */
    QString static_Files_ImportMeshFile(const QString& fileName);

    /**
     * @brief 打开工程文件
     * @param fileName 工程文件路径
     */
    QString static_Files_OpenProject(const QString& fileName);

    /**
     * @brief 保存工程文件
     * @param fileName 工程文件路径
     */
    QString static_Files_SaveProject(const QString& fileName);

    /**
    * @brief 获取 VTK 结果文件路径
    * @param fileName 工程文件路径
    */
    QString static_Files_GetVTKResultFilePath();

private:
    /**
     * @brief 等待线程池中的任务完成
     * @author libaojunAI (libaojunqd@foxmail.com)
     * @date 2026-05-07
     */
    void waitThreadPool();
};
/**
 * @brief 文件接口封装，供装饰器装饰
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-18
 */
class Files
{
public:
    explicit Files() = default;
    virtual ~Files() = default;
   
};
#endif
