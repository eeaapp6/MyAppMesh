/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "InitialConditionWidget.h"
#include "ui_InitialConditionWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIInitialCondition.h"
#include <QToolBox>

namespace GUI
{
    InitialConditionWidget::InitialConditionWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::InitialConditionWidget();
        _ui->setupUi(this);

        init();
    }

    InitialConditionWidget::~InitialConditionWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void InitialConditionWidget::init()
    {
        if (!_data_PHengLEI) return;
		_initialCondition = _data_PHengLEI->getInitialCondition();
		if (_initialCondition == nullptr) return;

        Core::FITKParameter* paraData = _initialCondition->getInitialConditionAdditionalData();
        if (paraData)
        {
            for (auto d : paraData->getParameter())
			{
				if(d == nullptr) continue;
				_ui->verticalLayout->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}
    }
}

