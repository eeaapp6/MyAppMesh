/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorGravityProp.h
 * @brief  重力场操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-08-27
 *********************************************************************/
#ifndef __OperatorGravityProp_H__
#define __OperatorGravityProp_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"


namespace OperModel
{
    /**
     * @brief  重力场操作器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-08-27
     */
    class OperatorGravityProp :public Core::FITKActionOperator
    {
    public:
        explicit OperatorGravityProp() = default;
        virtual ~OperatorGravityProp() = default;
        /**
         * @brief    执行界面操作
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-27
         */
        virtual bool execGUI();
        /**
         * @brief    执行业务处理逻辑
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-27
         */
        virtual bool execProfession();
    private:
        /**
         * @brief    重命名重力场
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-27
         */
        bool gravityPropRenameOper();
        /**
         * @brief    删除重力场
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-27
         */
        bool deleteGravity();
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionGravityCreate, OperatorGravityProp);
    Register2FITKOperatorRepo(actionGravityEdit, OperatorGravityProp);

}

#endif
