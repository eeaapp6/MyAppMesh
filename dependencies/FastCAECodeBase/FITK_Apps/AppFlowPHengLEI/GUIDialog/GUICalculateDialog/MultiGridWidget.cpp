/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MultiGridWidget.h"
#include "ui_MultiGridWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIMultiGrid.h"
#include <QToolBox>

namespace GUI
{
    MultiGridWidget::MultiGridWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::MultiGridWidget();
        _ui->setupUi(this);

        init();
    }

    MultiGridWidget::~MultiGridWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void MultiGridWidget::init()
    {
		if (_data_PHengLEI == nullptr) return;
		_multiGrid = _data_PHengLEI->getMultiGrid();
		if (_multiGrid == nullptr) return;

		Core::FITKParameter* paraData = _multiGrid->getAdditionalMultiGridData();
		if (paraData)
		{
			for (auto d : paraData->getParameter())
			{
				if(d == nullptr) continue;
				
				_ui->verticalLayout->addWidget(new Core::FITKWidgetComLine(d, this));
			}
			
		}

    }

	void MultiGridWidget::on_checkEnable_stateChanged()
	{
		bool enable = _ui->checkEnable->checkState();
		_multiGrid = _data_PHengLEI->getMultiGrid();
		if (_multiGrid == nullptr) return;
		_multiGrid->setEnableMultiGrid(enable);

		for (int i = 0; i < _ui->verticalLayout->count(); ++i) {
			QLayoutItem *item = _ui->verticalLayout->itemAt(i);
			if (item->widget()) {
				item->widget()->setVisible(enable);
			}
		}
	}
}

