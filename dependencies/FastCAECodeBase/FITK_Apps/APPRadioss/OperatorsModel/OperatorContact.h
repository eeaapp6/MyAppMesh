/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorContact.h
 * @brief  创建接触操作器
 * @author wangning (2185896382@qq.com)
 * @date   2026-02-06
 *********************************************************************/
#ifndef ___OPERATORCONTACT__H___
#define ___OPERATORCONTACT__H___

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace Radioss
{
    class FITKAbstractInteraction;
}

namespace OperModel
{
    /**
     * @brief  创建接触操作器
     * @author wangning (2185896382@qq.com)
     * @date   2026-02-06
     */
    class OperatorContact :public Core::FITKActionOperator
    {
    public:
        explicit OperatorContact() = default;
        virtual ~OperatorContact() = default;
        /**
         * @brief    执行界面操作
         * @author wangning (2185896382@qq.com)
         * @date   2026-02-06
         */
        virtual bool execGUI();
        /**
         * @brief    执行业务处理逻辑
         * @author wangning (2185896382@qq.com)
         * @date   2026-02-06
         */
        virtual bool execProfession();

    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionContactCreate, OperatorContact);

}

#endif
