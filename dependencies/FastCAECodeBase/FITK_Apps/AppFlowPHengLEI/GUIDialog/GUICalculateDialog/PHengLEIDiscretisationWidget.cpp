/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PHengLEIDiscretisationWidget.h"
#include "ui_PHengLEIDiscretisationWidget.h"
#include "CompSelectComBoxWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetVBox.h"
#include "FITK_Kernel/FITKEasyParam/FITKEasyParamWidgetFactory.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"


#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIDataManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEISpatialManager.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIDiscretisation.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIAbsSpatialDiscretisation.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEISpatialStruct.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEISpatialUnStruct.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "GUIFrame/PropertyWidget.h"
#include "GUIFrame/MainWindow.h"

#include <QToolBox>

namespace GUI
{
	Core::FITKParameter* SpatialTypeGetSubData(const QString & type, CompSelectComBoxWidget* widget)
	{
		if (widget == nullptr) return nullptr;
		Interface::FITKPhysicsHandlerFactory* dataFactory = FITKAPP->getComponents()->getComponentTByName<Interface::FITKPhysicsHandlerFactory>("FITKPHengLEIPhysicsHandlerFactory");
		if (!dataFactory) return nullptr;

		Interface::FITKPHengLEIData* data = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();
		if (!data) return nullptr;

		QString solverName = data->getSolver()->getDataObjectName();

		dataFactory->setPHengLEIDiscretisationType(type, solverName);

		Interface::FITKPHengLEIDiscretisation* PHengLEIDiscretisation = data->getDiscretisation();
		Interface::FITKPHengLEIAbsSpatialDiscretisation* PHengLEIDiscretisationType = PHengLEIDiscretisation->getSpatialDiscretisation();

		return PHengLEIDiscretisationType->getSchemeTypeParams();
	}

	PHengLEIDiscretisationWidget::PHengLEIDiscretisationWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
		GUICalculateWidgetBase(oper, parent)
	{
		_ui = new Ui::PHengLEIDiscretisationWidget();
		_ui->setupUi(this);

		init();
	}

	PHengLEIDiscretisationWidget::~PHengLEIDiscretisationWidget()
	{ 
		for (int i = 0; i < _senders.size(); i++) {
			auto sender = _senders[i];
			QObject::disconnect(sender);
		}

		if (_ui) delete _ui;
	}

	void PHengLEIDiscretisationWidget::init()
	{
		if (_data_PHengLEI == nullptr) return;
		if (_dataManager_PHengLEI == nullptr) return;
		_solValue = _data_PHengLEI->getDiscretisation();
		updateSpatial();
		updateTemporal();
		updateFlow();
	}


	void PHengLEIDiscretisationWidget::updateSpatial()
	{
		if (_dataManager_PHengLEI == nullptr) return;
		Interface::FITKPHengLEISpatialManager* SpatialManager = _dataManager_PHengLEI->getSpatialManager();
		if (SpatialManager == nullptr) return;
		if (_physicHandleFactory_PHengLEI == nullptr) return;
		if (_solValue == nullptr) return;

		QList<QWidget*> widgetList{};

		Interface::FITKPHengLEIAbsSpatialDiscretisation* SpatialType = _solValue->getSpatialDiscretisation();
		if (SpatialType)
		{
			QStringList options = SpatialManager->filterSpatial();
			CompSelectComBoxWidget* comp = new CompSelectComBoxWidget("Spatial", this);

			SenderMeesage message;
			message.sender = "limiter";

			QHash<QString, QList<QPair<QString, bool>>> sub;
			QPair<QString, bool> s1;
			s1.first = "venkatCoeff";
			s1.second = true;
			QList<QPair<QString, bool>> ss1;
			ss1.append(s1);
			sub.insert("vencat", ss1);
		
			QPair<QString, bool> s2;
			s2.first = "venkatCoeff";
		 s2.second = false;
			QList<QPair<QString, bool>> ss2;
			ss2.append(s2);
			sub.insert("barth", ss2);

			QPair<QString, bool> s3;
			s3.first = "venkatCoeff";
		 s3.second = false;
			QList<QPair<QString, bool>> ss3;
			ss3.append(s3);
			sub.insert("nolim", ss3);

			message.sub = sub;
			comp->setMessage(message);
			comp->setFunction(&SpatialTypeGetSubData);
			comp->setOptions(options);
			comp->setCurrentText(SpatialType->getDataObjectName());
			comp->setSubWidgetData(SpatialType->getSchemeTypeParams());
			widgetList.append(comp);
		}
		if (widgetList.count() > 0)
		{
			for (auto w : widgetList)
			{
				_ui->verticalLayout1->addWidget(w);
			}
		}
		else
		{
			// 隐藏Flow的tabwidget中的Spatial tab页
			if (_ui && _ui->Discretisation) {
				int spatialTabIndex = _ui->Discretisation->indexOf(_ui->Spatial);
				if (spatialTabIndex != -1) {
					_ui->Spatial->setVisible(false);
					_ui->Discretisation->setTabEnabled(spatialTabIndex, false);
				}
			}
		}

	}

	void PHengLEIDiscretisationWidget::updateTemporal()
	{
		if (_dataManager_PHengLEI == nullptr) return;
		if (_physicHandleFactory_PHengLEI == nullptr) return;
		if (_solValue == nullptr) return;

		QString limiterName = "limiter";
		Core::FITKAbstractEasyParam* limiterData = nullptr;
		QWidget* vencatWidget = {};
		QList<QWidget*> widgetList{};
		Core::FITKParameter* additionalData = _solValue->getTemporalAdditionalData();
		if (additionalData)
		{
			for (auto d : additionalData->getParameter())
			{
				if (d == nullptr) continue;
				if (limiterName == d->getDataObjectName()) limiterData = d;
				QWidget* w = new Core::FITKWidgetComLine(d, this);
				if (d->getDataObjectName() == "venkatCoeff") vencatWidget = w;
				widgetList.append(w);
			}
		}
		if (widgetList.count() > 0)
		{
			for (auto w : widgetList)
			{
				_ui->verticalLayout2->addWidget(w);
			}
		}
		else
		{
			// 隐藏Flow的tabwidget中的Spatial tab页
			if (_ui && _ui->Discretisation) {
				int temporalTabIndex = _ui->Discretisation->indexOf(_ui->Temporal);
				if (temporalTabIndex != -1) {
					_ui->Temporal->setVisible(false);
					_ui->Discretisation->setTabEnabled(temporalTabIndex, false);
				}
			}
		}
	}

	void PHengLEIDiscretisationWidget::updateFlow()
	{
		Core::FITKParameter* addData = _solValue->getFlowDiscretisation();
		QList<Core::FITKAbstractEasyParam*> paraList = addData->getParameter();
		QList<QWidget*> widgetList{};
		if (addData)
		{
			for (auto d : paraList)
			{
				if (d != nullptr)
				{
					QWidget* w = new Core::FITKWidgetComLine(d, this);
					widgetList.append(w);
				}
			}

			if (widgetList.count() > 0)
			{
				for (auto w : widgetList)
				{
					_ui->verticalLayout3->addWidget(w);
				}
			}
			else
			{
				// 隐藏Flow的tabwidget中的Flow tab页
				if (_ui && _ui->Discretisation) {
					int flowTabIndex = _ui->Discretisation->indexOf(_ui->Flow);
					if (flowTabIndex != -1) {
						_ui->Flow->setVisible(false);
						_ui->Discretisation->setTabEnabled(flowTabIndex, false);
					}
				}
			}
		}
	}

	void PHengLEIDiscretisationWidget::VenkatCoeffWidgetChanged()
	{

	}
}

