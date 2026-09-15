/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file OperatorsDelete.h
 * @brief 删除事件操作器
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-09-04
 * 
 */
#ifndef _OperatorsDelete_H_
#define _OperatorsDelete_H_

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"
#include "OperatorsBase.h"

namespace OperModel
{
    /**
     * @brief 打开事件操作器
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-09-04
     */
    class OperatorsDelete : public OperatorsBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Operators Open object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
        explicit OperatorsDelete();
        /**
         * @brief Destroy the Operators Open object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
        ~OperatorsDelete( )  = default;
        /**
         * @brief ui事件
         * @return true 成功
         * @return false 失败
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
         virtual bool execGUI( ) override;
        /**
         * @brief 执行结果处理
         * @return true 成功
         * @return false 失败
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
        virtual bool execProfession();
    };

    //根据按钮的ObjectName进行注册按钮事件
    Register2FITKOperatorRepo(actionGraphDelete, OperatorsDelete);
}

#endif