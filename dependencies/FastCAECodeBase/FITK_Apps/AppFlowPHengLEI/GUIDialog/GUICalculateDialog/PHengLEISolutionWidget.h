/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file PHengLEISolutionWidget.h
 * @brief Solution参数界面
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _PHengLEISolutionWidget_H
#define _PHengLEISolutionWidget_H

#include "GUICalculateDialogAPI.h"
#include "GUICalculateWidgetBase.h"

namespace Ui {
    class PHengLEISolutionWidget;
}

namespace Core {
	class FITKParameter;
}

namespace Interface {
    class FITKPHengLEISolution;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    /**
     * @brief Solution参数界面
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class GUICalculateDialogAPI PHengLEISolutionWidget :public GUICalculateWidgetBase
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
        PHengLEISolutionWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy the Solution Widget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~PHengLEISolutionWidget();
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
    private:
        /**
         * @brief   更新动量方程
         * @author  WangKai (aurora_starry@outlook.com)
         * @date    2025-08-01
         */
        void updateMomentumEquation();
        /**
		 * @brief   更新压力方程
         * @author  WangKai (aurora_starry@outlook.com)
         * @date    2025-08-01
         */
		void updatePressureEquation();
        /**
         * @brief   更新能量方程
         * @author  WangKai (aurora_starry@outlook.com)
         * @date    2025-08-01
         */
		void updateEnergyEquation();
        /**
         * @brief   更新湍流方程
         * @author  WangKai (aurora_starry@outlook.com)
         * @date    2025-08-01
         */
		void updateTurbulenceEquation();
        /**
         * @brief   更新物种方程
         * @author  WangKai (aurora_starry@outlook.com)
         * @date    2025-08-01
         */
		void updateSpeciesEquation();
        /**
         * @brief   更新松弛因子
         * @author  WangKai (aurora_starry@outlook.com)
         * @date    2025-08-01
         */
		void updateRelaxation();

    private:
        /**
         * @brief ui
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        Ui::PHengLEISolutionWidget* _ui = nullptr;
        /**
         * @brief  Solution参数对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-23
         */
        Interface::FITKPHengLEISolution* _solution = nullptr;

		QList<QMetaObject::Connection> _senders = {};
    };
}
#endif
