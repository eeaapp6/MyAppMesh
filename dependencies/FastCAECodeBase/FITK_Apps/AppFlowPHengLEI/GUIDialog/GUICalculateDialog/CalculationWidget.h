/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file CalculationWidget.h
 * @brief 网格相关参数界面
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _CalculationWidget_H
#define _CalculationWidget_H

#include "GUICalculateDialogAPI.h"
#include "GUICalculateWidgetBase.h"

namespace Ui {
    class CalculationWidget;
}

namespace Interface {
	class FITKPHengLEICalculation;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
	class PlotMonitorWidget;
    /**
     * @brief 网格相关参数界面
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class GUICalculateDialogAPI CalculationWidget :public GUICalculateWidgetBase
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
        CalculationWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy the Transport Widget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~CalculationWidget();
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
	public slots:
		/**
		 * @briefStart按钮点击事件
		 * @author BaGuijun (baguijun@163.com)
		 * @date 2024-08-14
		 */
		void on_btn_Start_clicked();
		/**
		 * @briefStop按钮点击事件
		 * @author BaGuijun (baguijun@163.com)
		 * @date 2024-08-14
		 */
		void on_btn_Stop_clicked();
    private:
		PlotMonitorWidget* _plotMonitorWidget = nullptr;
        /**
         * @brief ui
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        Ui::CalculationWidget* _ui = nullptr;
        /**
         * @brief  网格相关参数对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-26
         */
		Interface::FITKPHengLEICalculation * _calculation = nullptr;
    };
}
#endif
