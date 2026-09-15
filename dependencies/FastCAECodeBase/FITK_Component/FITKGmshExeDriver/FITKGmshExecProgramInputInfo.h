/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshExecProgramInputInfo.h
 * @brief  Gmsh 程序输入信息
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-28
 *********************************************************************/
#ifndef _FITK_GMESHEXEC_PROGRAMER_INPUTINFO_H___
#define _FITK_GMESHEXEC_PROGRAMER_INPUTINFO_H___
 
#include "FITKGmshExeDriverAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"

namespace GmshExe
{
    /**
     * @brief Gmsh可执行程序驱动输入信息
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-27
     */
    class FITKGmshExeDriverAPI FITKGmshExecProgramInputInfo : public AppFrame::FITKProgramInputInfo
    {
    public:
        /**
         * @brief Construct a new FITKGmshExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        explicit FITKGmshExecProgramInputInfo();
        /**
         * @brief Destroy the FITKGmshExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual ~FITKGmshExecProgramInputInfo();
        /**
         * @brief    设置geo脚本路径
         * @param[i] workDir 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        void setGeoScriptFile(QString geoScriptFile);
        /**
         * @brief    设置输出的网格文件
         * @param[i] type 网格文件类型
         * @param[i] meshFile 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        void setMeshFile(QString type, QString meshFile);
        /**
         * @brief    设置gmsh网格划分线程数
         * @param[i] threadNum 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        void setThreadNumber(int threadNum);
        /**
         * @brief 获取启动参数
         * @return QStringList
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual QStringList args() override;
        /**
         * @brief 设置监控文件
         * @return QStringList
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual void setMonitorFiles(QStringList &monitorFiles) override;
        /**
         * @brief 设置监控文件
         * @return QStringList
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual void setMonitorPath(QStringList &monitorpath) override;

    private:
        /**
         * @brief  脚本路径
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        QString _geoScriptFile{};
        /**
         * @brief  网格文件类型，网格文件路径
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        QPair<QString, QString> _meshFile{};
        /**
         * @brief  线程数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        int _threadNumber{ 1 };
        /**
         * @brief  维度
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        int _dim{ 3 };
    };
}


#endif
