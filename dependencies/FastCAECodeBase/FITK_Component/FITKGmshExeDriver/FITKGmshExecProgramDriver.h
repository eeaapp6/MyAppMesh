/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshExecProgramDriver.h
 * @brief  Gmsh可执行程序驱动器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-27
 *********************************************************************/
#ifndef _FITK_GMESHEXEC_PROGRAMER_DRIVER_H___
#define _FITK_GMESHEXEC_PROGRAMER_DRIVER_H___
 
#include "FITKGmshExeDriverAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKExecProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramDriverFactory.h"

namespace GmshExe
{
    /**
     * @brief Gmsh可执行程序驱动
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-27
     */
    class FITKGmshExeDriverAPI FITKGmshExecProgramDriver : public AppFrame::FITKExecProgramDriver
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKGmshExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        explicit FITKGmshExecProgramDriver();
        /**
         * @brief Destroy the FITKGmshExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual ~FITKGmshExecProgramDriver();
        /**
         * @brief 获取程序类型，抽象值
         * @return int
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual int getProgramType() override;
        /**
         * @brief 获取程序名称，任务管理器的名称，不是文件路径
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual QString getProgramName() override;
        /**
         * @brief    是否存在可执行程序
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-31
         */
        bool isExistExeProgram();
        /**
         * @brief    通过FITK网格配置参数写出配置文件 - 脚本文件geo
         * @param[i] inputFile 输入文件    （用于网格划分的模型文件：几何,网格文件）
         * @param[i] geoScriptFile geo文件 （生成的脚本文件）
         * @param[i] meshFile mesh文件     （网格驱动划分生成的网格文件）
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        bool writeConfigFile(QString inputFile, QString geoScriptFile, QString meshFile);

    };
    //驱动器注册相关操作
    Register2FITKProgramDriverFactory(1, FITKGmshExecProgramDriver, FITKGmshExecProgramDriver);
}


#endif
