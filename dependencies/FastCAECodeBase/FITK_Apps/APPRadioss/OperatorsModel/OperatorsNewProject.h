/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorsNewProject.h
 * @brief  新建工程操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2026-01-20
 *********************************************************************/
#ifndef _OperatorsNewProject_H
#define _OperatorsNewProject_H

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    /**
     * @brief  新建工程操作器
     * @author guqingtao (15598887859@163.com)
     * @date   2026-01-20
     */
    class OperatorsNewProject :public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        /**
         * @brief  Construct a new Operators Save object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-01-20
         */
        explicit OperatorsNewProject() = default;
        /**
         * @brief    Destroy the Operators Save object
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-20
         */
        virtual ~OperatorsNewProject() = default;
        /**
         * @brief    UI相关操作
         * @return   bool true:执行成功 false:执行失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-20
         */
        virtual bool execGUI() override;
        /**
         * @brief    执行业务操作
         * @return   bool true:执行成功 false:执行失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-20 
         */
        virtual bool execProfession() override;
    private:
        void clearProject();
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionNew, OperatorsNewProject);
}

#endif