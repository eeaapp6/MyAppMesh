/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file OperatorsResidaul.h
 
 */
#ifndef _OperatorsResidaul_H
#define _OperatorsResidaul_H

#include "OperManagerBase.h"

namespace ModelOper
{
   
    class OperatorsResidaul :public OperManagerBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Operators Transport object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        OperatorsResidaul();
        /**
         * @brief Destroy the Operators Transport object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~OperatorsResidaul();
        /**
         * @brief 执行
         * @return true 成功
         * @return false 失败
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        virtual bool execGUI();
        /**
         * @brief 执行结果处理
         * @return true 成功
         * @return false 失败
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        virtual bool execProfession();
    };

    // 按钮注册相关操作
	Register2FITKOperatorRepo(actionResidaulEdit, OperatorsResidaul);
}

#endif