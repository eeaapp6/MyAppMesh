/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file EnergyModelWidget.h
 * @brief Energy模型参数界面
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _EnergyModelWidget_H
#define _EnergyModelWidget_H

#include "GUICalculateDialogAPI.h"
#include "GUICalculateWidgetBase.h"

namespace Ui {
    class EnergyModelWidget;
}

namespace Interface {
	class FITKPHengLEIEnergyModel;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace Core {
    class FITKWidgetComLine;
}

namespace GUI
{
    /**
     * @brief Energy模型参数界面
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class GUICalculateDialogAPI EnergyModelWidget :public GUICalculateWidgetBase
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
        EnergyModelWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy the Transport Widget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~EnergyModelWidget();
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
	private slots:
        /**
         * @brief   能源模型状态改变
         * @author  WangKai (aurora_starry@outlook.com)
         * @date    2025-08-10
         */
		void onEnergyWidgetStateChanged(int state);

    private:
        void updateEnergyStateChanged(int state);
    private:
        /**
         * @brief ui
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        Ui::EnergyModelWidget* _ui = nullptr;
        /**
         * @brief  Energy模型参数数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-26
         */
		Interface::FITKPHengLEIEnergyModel * _energyModel = nullptr;
        /**
		 * @brief   启动Energy模型控件
         * @author  WangKai (aurora_starry@outlook.com)
         * @date    2025-08-10
         */
        Core::FITKWidgetComLine* _energyWidget{};
    };
}
#endif
