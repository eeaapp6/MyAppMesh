/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file GeneralWidget.h
 * @brief 通用控制参数界面
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _GeneralWidget_H
#define _GeneralWidget_H

#include "GUICalculateDialogAPI.h"
#include "GUICalculateWidgetBase.h"

namespace Ui {
    class GeneralWidget;
}

namespace Interface {
	class FITKPHengLEIGeneral;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    /**
     * @brief 通用控制参数界面
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class GUICalculateDialogAPI GeneralWidget :public GUICalculateWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Transport Widget object
         * @param[i]  oper           操作器
         * @param[i]  parent         父对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        GeneralWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy the Transport Widget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~GeneralWidget();
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
    private:
        /**
         * @brief ui
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        Ui::GeneralWidget* _ui = nullptr;
        /**
         * @brief  通用控制参数数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-26
         */
		Interface::FITKPHengLEIGeneral * _general = nullptr;
    };
}
#endif
