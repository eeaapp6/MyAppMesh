/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file OperatorsPHengLEIBoundaryConditions.h
 * @brief 网格边界操作器
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _OperatorsPHengLEIBoundaryConditions_H
#define _OperatorsPHengLEIBoundaryConditions_H

#include "OperPHengLEIManagerBase.h"

namespace ModelOper
{
    /**
     * @brief 网格边界操作器
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class OperatorsPHengLEIBoundaryConditions :public OperPHengLEIManagerBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Operators Boundary object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        OperatorsPHengLEIBoundaryConditions();
        /**
         * @brief Destroy the Operators Boundary object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~OperatorsPHengLEIBoundaryConditions();
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
    Register2FITKOperatorRepo(actionPHengLEIBoundaryConditionsCreate, OperatorsPHengLEIBoundaryConditions);
    Register2FITKOperatorRepo(actionPHengLEIBoundaryConditionsDelete, OperatorsPHengLEIBoundaryConditions);
    Register2FITKOperatorRepo(actionPHengLEIBoundaryConditionsEdit, OperatorsPHengLEIBoundaryConditions);
}

#endif