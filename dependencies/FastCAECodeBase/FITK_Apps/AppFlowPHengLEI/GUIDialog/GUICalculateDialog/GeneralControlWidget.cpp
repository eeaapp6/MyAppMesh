/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GeneralControlWidget.h"
#include "ui_GeneralControlWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIGeneralControl.h"
#include <QToolBox>

namespace GUI
{
    GeneralControlWidget::GeneralControlWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::GeneralControlWidget();
        _ui->setupUi(this);

        init();
    }

    GeneralControlWidget::~GeneralControlWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void GeneralControlWidget::init()
    {
		if (_data_PHengLEI == nullptr) return;
		_generalControl = _data_PHengLEI->getGeneralControl();
		if (_generalControl == nullptr) return;

		Core::FITKParameter* paraData = _generalControl->getGeneralControlAdditionalData();
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

