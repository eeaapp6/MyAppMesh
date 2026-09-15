/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file OperatorsImport.h
 * @brief 打开事件操作器
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-09-04
 * 
 */
#ifndef _OperatorsImport_H_
#define _OperatorsImport_H_

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"
#include "OperatorsBase.h"

namespace OperModel
{
    /**
     * @brief 打开事件操作器
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-09-04
     */
    class OperatorsImport : public OperatorsBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Operators Open object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
        explicit OperatorsImport();
        /**
         * @brief Destroy the Operators Open object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
        ~OperatorsImport( )  = default;
        /**
         * @brief ui事件
         * @return true 成功
         * @return false 失败
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
         virtual bool execGUI( ) override;
        /**
         * @brief 执行结果处理
         * @return true 成功
         * @return false 失败
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
        virtual bool execProfession();
    private slots:
        ;
        /**
         * @brief    文件导入槽函数
         * @param[i] isOK       是否成功
         * @param[i] objID      数据对象id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        void slotImportFinish(bool isOK, int objID);
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

    //根据按钮的ObjectName进行注册按钮事件
    Register2FITKOperatorRepo(actionOpen, OperatorsImport);
}

#endif