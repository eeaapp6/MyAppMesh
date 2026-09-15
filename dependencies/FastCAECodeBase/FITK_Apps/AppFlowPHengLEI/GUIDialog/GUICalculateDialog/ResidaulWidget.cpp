/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ResidaulWidget.h"
#include "ui_ResidaulWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIResidaulProcess.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"

#include <QToolBox>

namespace GUI
{
	ResidaulWidget::ResidaulWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
		GUICalculateWidgetBase(oper, parent)
	{
		_ui = new Ui::ResidaulWidget();
		_ui->setupUi(this);
	}

	ResidaulWidget::~ResidaulWidget()
	{
		if (_ui) {
			delete _ui;
			_ui = nullptr;
		}
	}



	//void ResidaulWidget::on_averagerRes_stateChanged(int arg1)
	//{
	//	if (arg1 == 1)
	//	{

	//	}
	//}

	//void ResidaulWidget::on_maxRes_stateChanged(int arg1)
	//{

	//}

	//void ResidaulWidget::on_maxResCoorX_stateChanged(int arg1)
	//{

	//}

	//void ResidaulWidget::on_maxResCoorY_stateChanged(int arg1)
	//{

	//}

	//void ResidaulWidget::on_maxResCoorZ_stateChanged(int arg1)
	//{

	//}

	void ResidaulWidget::on_OK_clicked()
	{
		Interface::FITKPHengLEIData* physicData = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		Interface::FITKPHengLEIResidaulProcess *Residaul = physicData->getResidaulProcess();
		if (_ui->averagerRes->isChecked())
		{
			Residaul->setName("averagerRes");
		}
		if (_ui->maxRes->isChecked())
		{
			Residaul->setName("maxRes");
		}
		if (_ui->maxResCoorX->isChecked())
		{
			Residaul->setName("maxResCoorX");
		}
		if (_ui->maxResCoorY->isChecked())
		{
			Residaul->setName("maxResCoorY");
		}
		if (_ui->maxResCoorZ->isChecked())
		{
			Residaul->setName("maxResCoorZ");
		}
	}

	void ResidaulWidget::on_Cancel_clicked()
	{

	}

	//void ResidaulWidget::on_Quit_clicked()
	//{

	//}
}
