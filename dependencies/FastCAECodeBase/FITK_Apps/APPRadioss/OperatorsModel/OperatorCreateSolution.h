/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _OperatorCreateSolution_H_
#define _OperatorCreateSolution_H_


#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    class OperatorCreateSolution : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        explicit OperatorCreateSolution() = default;
        virtual ~OperatorCreateSolution() = default;
        /**
         * @brief    执行界面操作
         * @return true     执行成功
         * @return false     执行失败
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-17
         */
        virtual bool execGUI() override;
        /**
         * @brief    执行写出操作
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-17
         */
        virtual bool execProfession() override;

    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionSolutionCreate, OperatorCreateSolution);
}

#endif