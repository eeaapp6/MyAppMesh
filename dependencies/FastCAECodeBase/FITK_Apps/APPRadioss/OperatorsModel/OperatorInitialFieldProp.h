/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    OperatorInitialFieldProp.h
 * @brief   初始化场操作器
 * @author  lilongyuan(lilongyuan@diso.cn)
 * @date    2025-09-02
 *********************************************************************/
#ifndef __OPERATOR_INITIAL_FIELD_PROP_H__
#define __OPERATOR_INITIAL_FIELD_PROP_H__

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace OperModel
{
    /**
     * @brief   初始化场操作器
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-02
     */
    class OperatorInitialFieldProp :public Core::FITKActionOperator
    {
    public:
        /**
         * @brief   构造函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        explicit OperatorInitialFieldProp() = default;
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        virtual ~OperatorInitialFieldProp() = default;
        /**
         * @brief   执行界面操作
         * @return  执行结果
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        virtual bool execGUI();
        /**
         * @brief   执行业务处理逻辑
         * @return  执行结果
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        virtual bool execProfession();
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionInitialFieldCreate, OperatorInitialFieldProp);
    Register2FITKOperatorRepo(actionInitialFieldEdit, OperatorInitialFieldProp);
}
#endif  // __OPERATOR_INITIAL_FIELD_PROP_H__