/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GlobalFiles.h
 * @brief  文件相关的python接口
 * @author guqingtao (15598887859@163.com)
 * @date   2026-06-02
 *********************************************************************/
#ifndef __PYTHONINTERFACE_GLOBALFILES_H___
#define __PYTHONINTERFACE_GLOBALFILES_H___

#include <QString>
#include <QObject>
#include "PythonInterfaceAPI.h"

class Files;

/**
 * @brief  文件相关PYTHON接口装饰
 * @author guqingtao (15598887859@163.com)
 * @date   2026-06-02
 */
class PythonInterfaceAPI FilesWapper : public QObject
{
    Q_OBJECT
public slots:
    /**
     * @brief  创建对象
     * @author guqingtao (15598887859@163.com)
     * @date   2026-06-02
     */
    Files* new_Files();
    /**
     * @brief 删除对象
     * @param[i]  F              装饰器创建的对象
     * @author guqingtao (15598887859@163.com)
     * @date   2026-06-02
     */
    void delete_Files(Files* F);
    /**
     * @brief 打开vtk文件
     * @param[i]  F              文件接口
     * @param[i]  fileName       文件名称
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-18
     */
    void static_Files_OpenVTKFiles(const QString& fileName);
private:
    /**
     * @brief    等待线程池中的任务完成
     * @return   void
     * @author   guqingtao (15598887859@163.com)
     * @date     2026-06-02
     */
    void waitThreadPool();
};

/**
 * @brief  文件接口封装，供装饰器装饰
 * @author guqingtao (15598887859@163.com)
 * @date   2026-06-02
 */
class Files
{
public:
    explicit Files() = default;
    virtual ~Files() = default;
   
};


#endif
