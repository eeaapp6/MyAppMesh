/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PHengLEISolutionWidget.h"
#include "ui_PHengLEISolutionWidget.h"
#include "CompSelectComBoxWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetVBox.h"
#include "FITK_Kernel/FITKEasyParam/FITKEasyParamWidgetFactory.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"


#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIDataManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEISpatialManager.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEISolution.h"
#include "GUIFrame/PropertyWidget.h"
#include "GUIFrame/MainWindow.h"

#include <QToolBox>

namespace GUI
{
	//Core::FITKParameter* SpatialTypeGetSubData(const QString & type, CompSelectComBoxWidget* widget)
	//{
	//	if (widget == nullptr) return nullptr;
	//	Interface::FITKPhysicsHandlerFactory* dataFactory = FITKAPP->getComponents()->getComponentTByName<Interface::FITKPhysicsHandlerFactory>("FITKPHengLEIPhysicsHandlerFactory");
	//	if (!dataFactory) return nullptr;

	//	Interface::FITKPHengLEIData* data = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();
	//	if (!data) return nullptr;

	//	QString solverName = data->getSolver()->getDataObjectName();

	//	dataFactory->setPHengLEIDiscretisationType(type, solverName);

	//	Interface::FITKPHengLEIDiscretisation* PHengLEIDiscretisation = data->getDiscretisation();
	//	Interface::FITKPHengLEIAbsSpatialDiscretisation* PHengLEIDiscretisationType = PHengLEIDiscretisation->getSpatialDiscretisation();

	//	return PHengLEIDiscretisationType->getSchemeTypeParams();
	//}

	PHengLEISolutionWidget::PHengLEISolutionWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
		GUICalculateWidgetBase(oper, parent)
	{
		_ui = new Ui::PHengLEISolutionWidget();
		_ui->setupUi(this);

		init();
		
	}

	PHengLEISolutionWidget::~PHengLEISolutionWidget()
	{ 
		for (int i = 0; i < _senders.size(); i++) {
			auto sender = _senders[i];
			QObject::disconnect(sender);
		}

		if (_ui) delete _ui;
	}

	void PHengLEISolutionWidget::init()
	{
		if (_data_PHengLEI == nullptr) return;
		if (_dataManager_PHengLEI == nullptr) return;
		_solution = _data_PHengLEI->getSolution();

		updateMomentumEquation();
		updatePressureEquation();
		updateEnergyEquation();
		updateTurbulenceEquation();
		updateSpeciesEquation();
		updateRelaxation();

	}


	void PHengLEISolutionWidget::updateMomentumEquation()
	{
		Core::FITKParameter* addData = _solution->getAdditionalMomentumEquationData();
		QList<Core::FITKAbstractEasyParam*> paraList = addData->getParameter();
		if (addData)
		{
			for (auto d : paraList)
			{
				if (d != nullptr)
				{
					_ui->verticalLayout1->addWidget(new Core::FITKWidgetComLine(d, this));
				}
			}
		}
	}

	void PHengLEISolutionWidget::updatePressureEquation()
	{
		Core::FITKParameter* addData = _solution->getAdditionalPressurePossionEquation();
		QList<Core::FITKAbstractEasyParam*> paraList = addData->getParameter();
		if (addData)
		{
			for (auto d : paraList)
			{
				if (d != nullptr)
				{
					_ui->verticalLayout_2->addWidget(new Core::FITKWidgetComLine(d, this));
				}
			}
		}
	}

	void PHengLEISolutionWidget::updateEnergyEquation()
	{
		Core::FITKParameter* addData = _solution->getAdditionalEnergyEquationData();
		QList<Core::FITKAbstractEasyParam*> paraList = addData->getParameter();
		if (addData)
		{
			for (auto d : paraList)
			{
				if (d != nullptr)
				{
					_ui->verticalLayout_5->addWidget(new Core::FITKWidgetComLine(d, this));
				}
			}
		}
	}

	void PHengLEISolutionWidget::updateTurbulenceEquation()
	{
		Core::FITKParameter* addData = _solution->getAdditionalTurbulenceEquationData();
		QList<Core::FITKAbstractEasyParam*> paraList = addData->getParameter();
		if (addData)
		{
			for (auto d : paraList)
			{
				if (d != nullptr)
				{
					_ui->verticalLayout3->addWidget(new Core::FITKWidgetComLine(d, this));
				}
			}
		}
	}

	void PHengLEISolutionWidget::updateSpeciesEquation()
	{
		Core::FITKParameter* addData = _solution->getAdditionalSpeciesEquationData();
		QList<Core::FITKAbstractEasyParam*> paraList = addData->getParameter();
		if (addData)
		{
			for (auto d : paraList)
			{
				if (d != nullptr)
				{
					_ui->verticalLayout4->addWidget(new Core::FITKWidgetComLine(d, this));
				}
			}
		}
	}

	void PHengLEISolutionWidget::updateRelaxation()
	{
		Core::FITKParameter* addData = _solution->getAdditionalRelaxationData();
		QList<Core::FITKAbstractEasyParam*> paraList = addData->getParameter();
		if (addData)
		{
			for (auto d : paraList)
			{
				if (d != nullptr)
				{
					_ui->verticalLayout_6->addWidget(new Core::FITKWidgetComLine(d, this));
				}
			}
		}
	}
}

