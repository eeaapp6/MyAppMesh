/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   CommandLineHandler.h
 * @brief  命令处理器
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-11
 *********************************************************************/
#ifndef _CommandLineHandler_H_
#define _CommandLineHandler_H_

#include "FITK_Kernel/FITKAppFramework/FITKCommandLineHandler.h"

/**
 * @brief  命令处理器
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-11
 */
class CommandLineHandler : public AppFrame::FITKCommandLineHandler
{
public:
    /**
     * @brief    构造函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-09-11
     */
    explicit CommandLineHandler() = default;
    /**
     * @brief    析构函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-09-11
     */
    virtual ~CommandLineHandler() = default;
    /**
     * @brief    执行命令行
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-09-11
     */
    void exec() override;
private:
    /**
     * @brief    导入VTK
     * @param[i] index     索引
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-09-11
     */
    void importVTK(const int index);
    /**
     * @brief    获取路径下所有的当前文件
     * @param[i] path             路径
     * @param[i] fileType         文件类型
     * @return   QString          所有文件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-09-11
     */
    QString getFilesToPath(QString path, QString fileType);
};
#endif


