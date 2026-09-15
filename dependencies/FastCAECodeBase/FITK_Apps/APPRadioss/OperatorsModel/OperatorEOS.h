/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file OperatorEOS.h
 * @brief EOS操作器接口
 * @author wangning (2185896382@qq.com)
 * @date 2025-10-30
 *
 */
#ifndef __OPERATOREOS_H__
#define __OPERATOREOS_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace Radioss
{
    class FITKAbstractEquationOfState;
    class FITKEquationOfStateManager;
}
namespace OperModel
{
    /**
     * @brief 状态方程操作器
     * @author wangning (2185896382@qq.com)
     * @date 2025-10-30
     */
    class OperatorEOS :public Core::FITKActionOperator
    {
    public:
        explicit OperatorEOS() = default;
        virtual ~OperatorEOS() = default;
        /**
         * @brief    执行界面操作
         * @author wangning (2185896382@qq.com)
         * @date 2025-10-30
         */
        virtual bool execGUI();
        /**
         * @brief    执行业务处理逻辑
         * @author wangning (2185896382@qq.com)
         * @date 2025-10-30
         */
        virtual bool execProfession();
    private:
        /**
         * @brief    获取状态方程对象
         * @author wangning (2185896382@qq.com)
         * @date 2025-10-30
         */
        Radioss::FITKAbstractEquationOfState* getSelectedEOS();
        /**
         * @brief    获取状态方程管理器
         * @author wangning (2185896382@qq.com)
         * @date 2025-10-30
         */
        Radioss::FITKEquationOfStateManager* getEOSManager();
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionEOSCreate, OperatorEOS);
    Register2FITKOperatorRepo(actionEOSRename, OperatorEOS);
    Register2FITKOperatorRepo(actionEOSEdit, OperatorEOS);
    Register2FITKOperatorRepo(actionEOSDelete, OperatorEOS);
}

#endif
