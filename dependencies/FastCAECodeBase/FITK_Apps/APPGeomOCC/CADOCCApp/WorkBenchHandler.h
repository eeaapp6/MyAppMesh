/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file WorkBenchHandler.h
 * @brief    工作台处理类
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-08-26
 * 
 */
#ifndef _FLOWAPP_WORKBENCH_HANDLER_H___
#define _FLOWAPP_WORKBENCH_HANDLER_H___
 
#include <QString>
#include "FITK_Kernel/FITKAppFramework/FITKWorkBenchHandler.h"

/**
 * @brief 工作台命令行处理器
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-08-26
 */
class OCCAppWorkBenchHandler : public AppFrame::FITKWorkBenchHandler
{
public:
     /**
      * @brief Construct a new Flow App Work Bench Handler object
      * @author libaojun (libaojunqd@foxmail.com)
      * @date 2024-08-26
      */
    explicit OCCAppWorkBenchHandler() = default;
    /**
     * @brief Destroy the Flow App Work Bench Handler object
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-26
     */
    virtual ~OCCAppWorkBenchHandler() = default;
     /**
      * @brief 处理命令行
      * @author libaojun (libaojunqd@foxmail.com)
      * @date 2024-08-26
      */
    virtual void execHandler() override;
    /**
    * @brief 程序结束写出文件
    * @author libaojun (libaojunqd@foxmail.com)
    * @date 2024-08-27
    */
    virtual void execOutput() override;
private:
     /**
      * @brief 导入几何文件
      * @param[i]  fileName       文件名称
      * @author libaojun (libaojunqd@foxmail.com)
      * @date 2024-08-26
      */
    void importGeoFile(const QString & fileName);
    /**
      * @brief 打开工程文件
      * @param[i]  fileName       文件名称
      * @author libaojun (libaojunqd@foxmail.com)
      * @date 2024-09-12
      */
    void openProjectFile(const QString & fileName);
    /**
      * @brief 保存工程文件
          * @param[i]  fileName       文件名称
      * @author libaojun (libaojunqd@foxmail.com)
      * @date 2024-09-12
      */
        void saveProjectFile(const QString & fileName);
    /**
          * @brief 导出几何文件
          * @param[i]  fileName       文件名称
      * @author libaojun (libaojunqd@foxmail.com)
      * @date 2024-09-12
      */
        void exportGeoFile(const QString & fileName);
};


#endif
