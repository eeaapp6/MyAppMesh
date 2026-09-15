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
 * @author wangning (2185896382@qq.com)
 * @date 2025-03-05
 *
 */
#ifndef __PYTHONINTERFACE_GLOBALFILES_H___
#define __PYTHONINTERFACE_GLOBALFILES_H___

#include <QString>
#include <QObject>
#include "PythonInterfaceAPI.h"

class Files;

/**
 * @brief 文件相关PYTHON接口装饰
 * @author wangning (2185896382@qq.com)
 * @date 2025-03-05
 */
class PythonInterfaceAPI FilesWapper : public QObject
{
    Q_OBJECT
public slots:
    /**
     * @brief 创建对象
     * @return Files*
    * @author wangning (2185896382@qq.com)
    * @date 2025-03-05
     */
    Files* new_Files();
    /**
     * @brief 删除对象
     * @param[i]  F              装饰器创建的对象
    * @author wangning (2185896382@qq.com)
    * @date 2025-03-05
     */
    void delete_Files(Files* F);
    /**
     * @brief 导入几何文件
     * @param[i]  fileName       文件名
    * @author wangning (2185896382@qq.com)
    * @date 2025-03-05
     */
    void static_Files_ImportGeoFile(const QString& fileName);
    /**
    * @brief 导入openFoam网格文件
    * @param[i]  fileName       文件名
    * @author wangning (2185896382@qq.com)
    * @date 2025-03-05
    */
    void static_Files_ImportOFMeshFile(const QString& fileName);
    /**
    * @brief 导入网格文件
    * @param[i]  fileName       文件名
    * @author wangning (2185896382@qq.com)
    * @date 2025-03-05
    */
    void static_Files_ImportMeshFile(const QString& fileName);

private:
    /**
    * @brief 等待线程池
    * @author wangning (2185896382@qq.com)
    * @date 2024-11-25
    */
    void initSleep();
};

/**
 * @brief 文件接口封装，供装饰器装饰
 * @author wangning (2185896382@qq.com)
 * @date 2025-03-05
 */
class Files
{
public:
    explicit Files() = default;
    virtual ~Files() = default;
   
};


#endif
