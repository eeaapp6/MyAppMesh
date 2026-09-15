/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperSolverSettings.h
 * @brief  求解器设置操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-05
 *********************************************************************/
#ifndef __OPERSOLVERSETTINGS__H__
#define __OPERSOLVERSETTINGS__H__

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    /**
     * @brief  求解器设置操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-05
     */
    class OperSolverSettings :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-05
         */
        explicit OperSolverSettings() = default;
        /**
         * @brief    析构函数
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-05
         */
        virtual ~OperSolverSettings() = default;

        /**
         * @brief  执行界面操作
         * @return true     执行成功
         * @return false     执行失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-05
         */
        virtual bool execGUI() override;
        /**
         * @brief  执行读入操作
         * @return true     执行成功
         * @return false     执行失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-05
         */
        virtual bool execProfession() override;

    private:

    };
    Register2FITKOperatorRepo(actionSolverSettings, OperSolverSettings);

}
#endif