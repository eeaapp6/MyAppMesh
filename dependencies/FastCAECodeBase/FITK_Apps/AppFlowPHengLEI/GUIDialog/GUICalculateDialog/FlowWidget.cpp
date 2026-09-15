/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FlowWidget.h"
#include "ui_FlowWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIFlow.h"
#include <QToolBox>

namespace GUI
{
    FlowWidget::FlowWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::FlowWidget();
        _ui->setupUi(this);

        init();
    }

    FlowWidget::~FlowWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void FlowWidget::init()
    {
		if (_data_PHengLEI == nullptr) return;
		_Flow = _data_PHengLEI->getFlow();
		if (_Flow == nullptr) return;

		Core::FITKParameter* solvername = _Flow->getAdditionalSolverName();
		if (solvername)
		{
			for (auto d : solvername->getParameter())
			{
				if(d == nullptr) continue;
				_ui->verticalLayout->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		Core::FITKParameter* baseData = _Flow->getAdditionalInitialValue();
		if (baseData)
		{
			for (auto d : baseData->getParameter())
			{
				if (d == nullptr) continue;
				_ui->verticalLayout->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		Core::FITKParameter* mequation = _Flow->getAdditionalMomentumEquation();
		if (mequation)
		{
			for (auto d : mequation->getParameter())
			{
				if (d == nullptr) continue;
				QLayout*  meshLayout = _ui->_MEquation->layout();
				meshLayout->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		Core::FITKParameter* ppequation = _Flow->getAdditionalPressurePossionEquation();
		if (ppequation)
		{
			for (auto d : ppequation->getParameter())
			{
				if (d == nullptr) continue;
				QLayout*  meshLayout = _ui->_PPEquation->layout();
				meshLayout->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		Core::FITKParameter* surfaceflux = _Flow->getAdditionalSurfaceFlux();
		if (surfaceflux)
		{
			for (auto d : surfaceflux->getParameter())
			{
				if (d == nullptr) continue;
				QLayout*  meshLayout = _ui->_SurfaceFlux->layout();
				meshLayout->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}
    }
}

