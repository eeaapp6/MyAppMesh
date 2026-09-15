/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file MultiGridWidget.h
 * @brief 多重网格参数界面
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _MultiGridWidget_H
#define _MultiGridWidget_H

#include "GUICalculateDialogAPI.h"
#include "GUICalculateWidgetBase.h"

namespace Ui {
    class MultiGridWidget;
}

namespace Interface {
	class FITKPHengLEIMultiGrid;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    /**
     * @brief 多重网格参数界面界面
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class GUICalculateDialogAPI MultiGridWidget :public GUICalculateWidgetBase
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
        MultiGridWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy the Transport Widget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~MultiGridWidget();
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
	private slots:
		void on_checkEnable_stateChanged();
    private:
        /**
         * @brief ui
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        Ui::MultiGridWidget* _ui = nullptr;
        /**
         * @brief  多重网格参数对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-26
         */
		Interface::FITKPHengLEIMultiGrid * _multiGrid = nullptr;
    };
}
#endif
