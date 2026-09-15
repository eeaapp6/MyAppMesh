/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file OperatorsImportModel.h
 * @brief 导入模型
 * @author wangning (2185896382@qq.com)
 * @date 2025-05-23
 *
 */
#ifndef _OperatorsImportModel_H
#define _OperatorsImportModel_H

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"

namespace ModelOper
{
    /**
    * @brief 导入模型操作器
    * @author wangning (2185896382@qq.com)
    * @date 2025-05-23
    */
    class   OperatorsImportGeometry :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        explicit OperatorsImportGeometry();
        virtual ~OperatorsImportGeometry();

        virtual bool execGUI();

        virtual bool execProfession();
    private slots:
        /**
        * @brief 导入完成槽函数
        * @param[i]  result         是否成功
        * @param[i]  objID          几何id
        * @author shenzhuowei (2931275006@qq.com)
        * @date 2024-12-18
        */
        void slotImportFinish(bool result, int objID);
    };

    /**
    * @brief 导入文件读取线程
    * @author BaGuijun (baguijun@163.com)
    * @date 2024-08-14
    */
    class ImportReadThread :public Core::FITKThreadTask
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Import Read Thread object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ImportReadThread() = default;
        /**
         * @brief Destroy the Import Read Thread object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~ImportReadThread() = default;
        /**
         * @brief 执行函数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void run();
    signals:
        ;
        /**
         * @brief 导入完成信号
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void sigImportFinish(bool, int);
    public:
        /**
         * @brief 文件名称
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        QString _fileName = "";
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionImportGeometry, OperatorsImportGeometry);
}

#endif