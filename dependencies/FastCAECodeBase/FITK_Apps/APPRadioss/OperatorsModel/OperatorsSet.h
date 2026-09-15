/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorsSet.h
 * @brief  集合创建
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-08-05
 *********************************************************************/
#ifndef _OPERATORSSET_H__
#define _OPERATORSSET_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace Interface
{
    class FITKAbstractModel;
    class FITKModelSet;
}
namespace OperModel
{
    /**
     * @brief  集合创建
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-08-05
     */
   class OperatorsSet :public Core::FITKActionOperator
   {
   public:
       explicit OperatorsSet() = default;
       virtual ~OperatorsSet() = default;
       /**
        * @brief    执行界面操作
        * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
        * @date     2025-08-05
        */
       virtual bool execGUI() override;
       /**
        * @brief    业务逻辑执行
        * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
        * @date     2025-08-05
        */
       virtual bool execProfession() override;
   };
   Register2FITKOperatorRepo(actionSetNodesCreate,OperatorsSet);
   Register2FITKOperatorRepo(actionSetElementCreate,OperatorsSet);
}
#endif // !_OPERATORSSET_H__
