/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   LightWidget.h
 * @brief  灯光界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-16
 *********************************************************************/
#ifndef _LightWidget_H
#define _LightWidget_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIWidgetBase.h"

namespace Ui{
    class LightWidget;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    class GUIDialogAPI LightWidget : public GUIWidgetBase
    {
        Q_OBJECT;
    public:
        LightWidget(EventOper::ParaWidgetInterfaceOperator* oper);
        ~LightWidget();

        void init();
    private slots:
        ;
        void on_pushButton_clicked();
    private:
        Ui::LightWidget* _ui = nullptr;
        EventOper::ParaWidgetInterfaceOperator* _oper = nullptr;
    };
}
#endif
