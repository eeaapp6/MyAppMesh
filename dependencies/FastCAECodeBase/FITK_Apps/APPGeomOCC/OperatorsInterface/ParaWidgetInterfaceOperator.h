/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file ParaWidgetInterfaceOperator.h
 * @brief 与参数窗口相关的operator基类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-09
 *
 */
#ifndef _PARAWIDGET_OPERATOR_INTERFACE_H___
#define _PARAWIDGET_OPERATOR_INTERFACE_H___

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "OperatorsInterfaceAPI.h"

namespace EventOper
{
    /**
     * @brief 与参数窗口相关的widget
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-09
     */
    class OperatorsInterfaceAPI ParaWidgetInterfaceOper : public Core::FITKActionOperator
    {
    public:
        /**
         * @brief Construct a new Para Widget Interface Oper object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-09
         */
        explicit ParaWidgetInterfaceOper() = default;
        /**
         * @brief Destroy the Para Widget Interface Oper object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-09
         */
        virtual ~ParaWidgetInterfaceOper() = default;

        /**
         * @brief   结束操作。[虚函数]
         * @param   updateCommand：是否更新命令[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-23
         */
        virtual void finalize(bool updateCommand = false);

    protected:
        /**
         * @brief 当前步
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-09
         */
        int _currentStage{ 0 };
    };
}

#endif
