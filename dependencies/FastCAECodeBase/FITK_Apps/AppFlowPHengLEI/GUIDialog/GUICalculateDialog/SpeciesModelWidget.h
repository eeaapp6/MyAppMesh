/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file SpeciesModelWidget.h
 * @brief Species模型界面
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _SpeciesModelWidget_H
#define _SpeciesModelWidget_H

#include "GUICalculateDialogAPI.h"
#include "GUICalculateWidgetBase.h"

namespace Ui {
    class SpeciesModelWidget;
}

namespace Interface {
	class FITKPHengLEISpeciesModel;
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
     * @brief Species模型参数界面
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class GUICalculateDialogAPI SpeciesModelWidget :public GUICalculateWidgetBase
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
        SpeciesModelWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy the Transport Widget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~SpeciesModelWidget();
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
	private slots:
		void onSpeciesWidgetStateChanged(int state);
    private:
        void updateSpeciesStateChanged(int state);
    private:
        /**
         * @brief ui
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        Ui::SpeciesModelWidget* _ui = nullptr;
        /**
         * @brief  Species模型参数数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-26
         */
		Interface::FITKPHengLEISpeciesModel * _speciesModel = nullptr;
        /**
		 * @brief   启动Species模型控件
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-08-10
		 */
        Core::FITKWidgetComLine* _speciesWidget{};
    };
}
#endif
