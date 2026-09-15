/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorsProbe.h
 * @brief  探针操作器
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-12
 *********************************************************************/
#ifndef _OperatorsProbe_H_
#define _OperatorsProbe_H_

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"
#include "OperatorsBase.h"

namespace OperModel
{
    /**
     * @brief  流线管理器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-12
     */
    class OperatorsProbe : public OperatorsBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Operators Open object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
        explicit OperatorsProbe() = default;
        /**
         * @brief Destroy the Operators Open object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
        ~OperatorsProbe() = default;
        /**
         * @brief ui事件
         * @return true 成功
         * @return false 失败
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
        virtual bool execGUI() override;
        /**
         * @brief 执行结果处理
         * @return true 成功
         * @return false 失败
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-04
         */
        virtual bool execProfession();
        /**
         * @brief    其他事件处理
         * @param[i] index       事件索引
         * @param[i] value       其他参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-21
         */
        virtual void eventProcess(int index, QHash<int, QVariant> value = QHash<int, QVariant>());
    private slots:
        ;
        /**
         * @brief    拾取点结束事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-21
         */
        void slotPickPointDataOver();
        /**
         * @brief    拾取单元结束事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-22
         */
        void slotPickCellDataOver();
    };

    //根据按钮的ObjectName进行注册按钮事件
    Register2FITKOperatorRepo(actionProbeCreate, OperatorsProbe);
    Register2FITKOperatorRepo(actionProbeEdit, OperatorsProbe);
}

#endif