/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @brief 操作器基类
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 */
#ifndef __OperatorsBase_H__
#define __OperatorsBase_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "GUIFrame/MainWindow.h"
#include "OperatorsModelAPI.h"
#include <QStringList>

namespace OperModel
{
    /**
     * @brief 操作器基类
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class OperatorsModelAPI OperatorsBase : public EventOper::ParaWidgetInterfaceOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Oper Manager Base object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        OperatorsBase();
        /**
         * @brief Destroy the Oper Manager Base object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        virtual ~OperatorsBase();
    protected:
        /**
         * @brief 参数预处理
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void preArgs() override;
        /**
         * @brief 主界面
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        GUI::MainWindow* _mainWindow = nullptr;
        /**
         * @brief 触发对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        QString _senderName = "";
        /**
         * @brief  是否点击
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        bool _isChecked = false;
    };
} // namespace ModelOper
#endif //!__OperatorsBase_H__
