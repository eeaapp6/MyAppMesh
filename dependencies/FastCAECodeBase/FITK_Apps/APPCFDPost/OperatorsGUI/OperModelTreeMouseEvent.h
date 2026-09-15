/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @brief  模型树点击事件处理器
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-07
 */
#ifndef _OperModelTreeMouseEvent_H_
#define _OperModelTreeMouseEvent_H_

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "OperatorsGUIAPI.h"

namespace GUIOper
{
    /**
     * @brief  模型树点击事件处理器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-07
     */
    class OperatorsGUIAPI OperModelTreeMouseEvent : public EventOper::ParaWidgetInterfaceOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        OperModelTreeMouseEvent();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        ~OperModelTreeMouseEvent();
        /**
         * @brief    更新树
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        void updateTree() override;
    };

    Register2FITKOperatorRepo(ModelTreeEvent, OperModelTreeMouseEvent);

}  // namespace GUIOper

#endif
