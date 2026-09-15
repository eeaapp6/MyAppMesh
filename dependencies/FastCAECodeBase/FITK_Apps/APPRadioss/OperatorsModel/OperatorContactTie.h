/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorContactTie.h
 * @brief  接触Tie(绑定)操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-25
 *********************************************************************/
#ifndef __OPERATORCONTACT_TIE_H__
#define __OPERATORCONTACT_TIE_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace Radioss
{
    class FITKAbstractInteraction;
}

namespace OperModel
{
    /**
     * @brief  接触Tie操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-25
     */
    class OperatorContactTie :public Core::FITKActionOperator
    {
    public:
        explicit OperatorContactTie() = default;
        virtual ~OperatorContactTie() = default;
        /**
         * @brief    执行界面操作
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-25
         */
        virtual bool execGUI();
        /**
         * @brief    执行业务处理逻辑
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-25
         */
        virtual bool execProfession();

    private:
        /**
         * @brief    创建接触Tie
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-25
         */
        bool operCreateContactTie();
        /**
         * @brief    编辑接触Tie
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-25
         */
        bool operEditContactTie();

        /**
         * @brief    获取相互作用数据
         * @return   Radioss::FITKAbstractInteraction *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-29
         */
        Radioss::FITKAbstractInteraction* getInteractionData();
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionContactTieCreate, OperatorContactTie);
    Register2FITKOperatorRepo(actionContactTieEdit, OperatorContactTie);

}

#endif
