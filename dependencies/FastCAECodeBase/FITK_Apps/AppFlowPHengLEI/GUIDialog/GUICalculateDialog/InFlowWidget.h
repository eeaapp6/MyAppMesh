/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file InFlowWidget.h
 * @brief 来流参数界面
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _InFlowWidget_H
#define _InFlowWidget_H

#include "GUICalculateDialogAPI.h"
#include "GUICalculateWidgetBase.h"

namespace Ui {
	class InFlowWidget;
}

namespace Core {
	class FITKParameter;
}

namespace Interface {
    class FITKPHengLEIInFlow;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
	class CompSelectComBoxWidget;
    /**
     * @brief 来流参数界面
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class GUICalculateDialogAPI InFlowWidget:public GUICalculateWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Solution Widget object
         * @param[i]  oper           操作器对象
         * @param[i]  parent         父对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        InFlowWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy the Solution Widget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~InFlowWidget();
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
    protected:
    private:
		void updateInFlow();
    private:
        /**
         * @brief ui
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        Ui::InFlowWidget* _ui = nullptr;
        /**
         * @brief  来流参数对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-23
         */
        Interface::FITKPHengLEIInFlow* _inFlow = nullptr;
    };
}
#endif
