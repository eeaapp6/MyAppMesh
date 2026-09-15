/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorsExportManager.h
 * @brief  文件导出操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-08-05
 *********************************************************************/
#ifndef _OPERATORSEXPORTMANAGER_H
#define _OPERATORSEXPORTMANAGER_H

#include "OperManagerBase.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"
#include "FITK_Component/FITKCGNSIO/FITKCGNSIOInterface.h"

namespace ModelOper
{
    /**
     * @brief  导出类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-05
     */
    enum class ExportType {
        ExportNone,                ///空
        ExportGeo,                 ///几何
        ExportMesh,                ///网格
        ExportOpenFoamMesh,        ///OpenFoam网格
    };
    /**
     * @brief  文件导出操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-05
     */
    class OperatorsExportManager :public OperManagerBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief  Construct a new Operators Export Manager object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        OperatorsExportManager();
        /**
         * @brief  Destroy the Operators Export Manager object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        ~OperatorsExportManager();
        /**
         * @brief  执行
         * @return true 成功
         * @return false 失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        virtual bool execGUI();
        /**
         * @brief  执行结果处理
         * @return true 成功
         * @return false 失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        virtual bool execProfession();
    private slots:
    };

    /**
     * @brief  导出文件写出线程
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-05
     */
    class ExportWriteThread :public Core::FITKThreadTask
    {
        Q_OBJECT;
    public:
        /**
         * @brief  Construct a new Export Write Thread object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        ExportWriteThread() = default;
        /**
         * @brief  Destroy the Export Write Thread object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        ~ExportWriteThread() = default;
        /**
         * @brief  执行函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        void run();
    signals:
        ;
        /**
         * @brief  写出完成信号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        void sigExportFinish();

    private:
        /**
         * @brief    写出CGNS网格预处理
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-07
         */
        void writeCGNSMeshPreProcessing(IO::CGNSDataIO& data);
        /**
         * @brief    读取CGNS网格完成处理
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-07
         */
        void writeCGNSMeshFinishProcessing();

    public:
        /**
         * @brief  类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        ExportType _type = ExportType::ExportNone;
        /**
         * @brief  文件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        QString _fileName = "";
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionExportGeometry, OperatorsExportManager);
    Register2FITKOperatorRepo(actionExportMesh, OperatorsExportManager);
    Register2FITKOperatorRepo(actionExportOpenFoamMesh, OperatorsExportManager);
}

#endif