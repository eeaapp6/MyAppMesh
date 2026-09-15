/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file OperImportMesh.h
 * @brief  网格导入操作类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-10-21
 * 
 */
#ifndef _OperatorImportMESH_H_
#define _OperatorImportMESH_H_


#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"

namespace AbaqusData
{
    class FITKDataCase;
}
namespace ModelOper
{
    /**
     * @brief 网格导入操作类声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-10-21
     */
    class FITKOperatorImportMesh : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKOperatorImportMesh object
         * @author libaojun
         * @date   2025-10-21
         */ 
        explicit FITKOperatorImportMesh() = default;
        /**
         * @brief Destroy the FITKOperatorImportMesh object
         * @author libaojun
         * @date   2025-10-21
         */
        virtual ~FITKOperatorImportMesh() = default;
 /**
          * @brief 执行界面操作
          * @return true     执行成功
          * @return false     执行失败
          * @author libaojun
          * @date   2025-10-21
          */
         virtual bool execGUI() override;
         /**
          * @brief 执行读入操作
          * @return true     执行成功
          * @return false     执行失败
          * @author libaojun
          * @date   2025-10-21
          */
         virtual bool execProfession() override;
 
    private slots:
        /**
         * @brief 读取线程结束执行该槽函数
         * @author libaojun
         * @date 2025-10-21
         */
        void ioThreadFinishedSlot();

        /**
        * @brief 读取线程结束执行该槽函数（针对INP文件）
        * @author wangning 
        * @date 2026-06-17
        */
        void ioThreadFinishedSlot_inp();

    private:
        /**
         * @brief 读取的abaqus装配
         * @author wangning 
         * @date 2026-06-17
         */
        AbaqusData::FITKDataCase* _abaqusDataCase{ nullptr };
 
    };
    
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionImportMesh, FITKOperatorImportMesh);
}


namespace ModelOper
{
    /**
     * @brief 抽象网格导入器类
     * @author libaojun
     * @date   2025-10-21
     */
    class FITKAbstractMeshImporter : public Core::FITKThreadTask
    {

    public:
        /**
         * @brief Construct a new FITKAbstractMeshImporter object
         * @param[i]  parent         父对象
         * @author libaojun
         * @date   2025-10-21
         */
        explicit FITKAbstractMeshImporter(QObject* parent = nullptr);
        /**
         * @brief Destroy the FITKAbstractMeshImporter object
         * @author libaojun
         * @date   2025-10-21
         */
        virtual ~FITKAbstractMeshImporter() = 0;

        /**
         * @brief 设置文件名称
         * @param[i]  fileName      文件名称
         * @author libaojun
         * @date   2025-10-21
         */
        void setFileName(const QString& fileName);


    protected:        
    /**
         * @brief 文件名称
         * @author libaojun
         * @date   2025-10-21
         */
        QString _fileName{};  
    };
}

#endif