/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file OperScript.h
 * @brief  脚本操作声明
 * @author wangning (2185896382@qq.com)
 * @date 2025-07-03
 *
 */
#ifndef __OPER_SCRIPT_H___
#define  __OPER_SCRIPT_H___ 


#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUIOper
{
    /**
     * @brief 脚本相关操作器
     * @author wangning (2185896382@qq.com)
     * @date 2025-07-03
     */
    class OperScript :public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        /**
         * @brief 脚本相关操作器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-05
         */
        explicit OperScript() = default;
        /**
         * @brief Destroy the Oper Working Dir object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        virtual ~OperScript() = default;
        /**
         * @brief 执行GUI操作程序，选择文件对话框
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        virtual bool execGUI() override;

        /**
         * @brief       执行业务逻辑。[重写]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-13
         */
        bool execProfession() override;

    private:
        /**
         * @brief 记录脚本
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-05
         */
        void recordScript();
        /**
         * @brief 结束脚本记录
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-05
         */
        void endScript();
        /**
         * @brief 执行脚本文件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-05
         */
        void runScript();
        /**
         * @brief    打开脚本文件
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-21
         */
        void openScript();
        /**
         * @brief  清理变量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date   2024-11-05
         */
        void clearVariable();
    };

    // 按钮注册相关操作

    Register2FITKOperatorRepo(actionRecordScript, OperScript);
    Register2FITKOperatorRepo(actionEndScript, OperScript);
    Register2FITKOperatorRepo(actionRunScript, OperScript);
    Register2FITKOperatorRepo(actionClearVariable, OperScript);
    Register2FITKOperatorRepo(actionfileOpenScript, OperScript);
}


#endif
