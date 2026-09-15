/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorContactMultiUsageImpact.h
 * @brief  多用途冲击连接
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-03
 *********************************************************************/
#ifndef __OPERATORCONTACT_MULTIUSAGEIMPACT_H__
#define __OPERATORCONTACT_MULTIUSAGEIMPACT_H__

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
    class OperatorContactMultiUsageImpact :public Core::FITKActionOperator
    {
    public:
        explicit OperatorContactMultiUsageImpact() = default;
        virtual ~OperatorContactMultiUsageImpact() = default;
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
         * @brief    创建接触MultiUsageImpact
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-25
         */
        bool operCreateContactMultiUsageImpact();
        /**
         * @brief    编辑接触MultiUsageImpact
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-25
         */
        bool operEditContactMultiUsageImpact();

        /**
         * @brief    获取相互作用数据
         * @return   Radioss::FITKAbstractInteraction *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-29
         */
        Radioss::FITKAbstractInteraction* getInteractionData();
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionContactMultiUsageImpactCreate, OperatorContactMultiUsageImpact);
    Register2FITKOperatorRepo(actionContactMultiUsageImpactEdit, OperatorContactMultiUsageImpact);

}

#endif
