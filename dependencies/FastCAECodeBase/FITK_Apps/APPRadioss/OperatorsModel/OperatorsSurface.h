/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorsSurface.h
 * @brief  表面创建
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-08-05
 *********************************************************************/
#ifndef _OPERATORSSURFACE_H__
#define _OPERATORSSURFACE_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace OperModel
{
    /**
     * @brief  表面创建
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-08-05
     */
    class OperatorsSurface :public Core::FITKActionOperator
    {
    public:
        explicit OperatorsSurface() = default;
        virtual ~OperatorsSurface() = default;
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
    Register2FITKOperatorRepo(actionSurfNodeCreate, OperatorsSurface);
    Register2FITKOperatorRepo(actionSurfElementCreate, OperatorsSurface);
}
#endif // !_OPERATORSSET_H__
