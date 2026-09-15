/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file DealCouplingWidget.h
 * @brief 计算设置界面
 * @author LXF (ButtersFLG@foxmail.com)
 * @date 2025-01-14
 *
 */
#ifndef _DealCouplingWidget_H
#define _DealCouplingWidget_H

#include "GUICalculateDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"
#include "GUICalculateWidgetBase.h"
#include <QProcess>

namespace Ui {
    class DealCouplingWidget;
}
namespace Interface {
	class FITKPHengLEICouplingControl;
}
namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    class GUICalculateDialogAPI DealCouplingWidget :public GUICalculateWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Mesh Base Widget object
         * @param[i]  oper           操作器对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        DealCouplingWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent);
        /**
         * @brief Destroy the Mesh Base Widget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~DealCouplingWidget();
        /**
         * @brief 初始化界面
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */






        void initWidget();

        void updateWidget();

    private slots:

        void onLineEditChanged();

        void onRadioButtonChanged();

		void onSaveButton();

    private:
        /**
         * @brief 操作器对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        EventOper::ParaWidgetInterfaceOperator* _oper = nullptr;
        /**
         * @brief ui对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        Ui::DealCouplingWidget* _ui = nullptr;
        /**
         * @brief  工作路径
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-31
         */

		Interface::FITKPHengLEICouplingControl * _couplingControl = nullptr;
    };
}

#endif
