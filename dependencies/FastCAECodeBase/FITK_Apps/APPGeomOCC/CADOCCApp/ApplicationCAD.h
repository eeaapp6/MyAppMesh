/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    ApplicationCAD.h
 * @brief   应用程序类。

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-05
 *********************************************************************/

#ifndef __APPLICATIONCAD__
#define __APPLICATIONCAD__

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

/**
 * @brief   应用程序类。
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-05
 */
class ApplicationCAD : public AppFrame::FITKApplication
{
public:
    /**
     * @brief   构造函数。
     * @param   argc：程序启动命令行参数
     * @param   argv：程序启动命令行参数
     * @return  
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-05
     */
    explicit ApplicationCAD(int &argc, char **argv);

    /**
     * @brief   析构函数。[虚函数]
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-05
     */
    virtual ~ApplicationCAD() = default;

    /**
     * @brief   应用程序初始化。[重写]
     * @return  是否初始化成功
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-05
     */
    bool init() override;

};

#endif // __APPLICATIONCAD__
