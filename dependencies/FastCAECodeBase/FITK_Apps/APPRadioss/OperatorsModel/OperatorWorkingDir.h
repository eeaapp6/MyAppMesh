/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file OperatorWorkingDir.h
 * @brief 设置工作路径操作类
 * @author wangning (2185896382@qq.com)
 * @date 2025-02-14
 *
 */
#ifndef _OPERATOR__WORKINGDIR_H___
#define _OPERATOR__WORKINGDIR_H___

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"


namespace GUIOper
{
    /**
     * @brief 设置工作路径操作器
     * @author wangning (2185896382@qq.com)
     * @date 2025-02-14
     */
    class OperatorWorkingDir :public Core::FITKActionOperator
    {

    public:

        explicit OperatorWorkingDir() = default;
        virtual ~OperatorWorkingDir() = default;
        /**
         * @brief 执行GUI操作程序，选择文件对话框
         * @return true
         * @return false
         * @author wangning (2185896382@qq.com)
         * @date 2025-02-14
         */
        virtual bool execGUI() override;

    };

    // 按钮注册相关操作

    Register2FITKOperatorRepo(actionWorkingDir, OperatorWorkingDir);
}


#endif
