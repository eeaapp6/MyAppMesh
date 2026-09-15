/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "InFlowWidget.h"
#include "ui_InFlowWidget.h"
#include "CompSelectComBoxWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetVBox.h"
#include "FITK_Kernel/FITKEasyParam/FITKEasyParamWidgetFactory.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFSolver.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFSolution.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsManager.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFSolutionSolverManager.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFSolutionSolver.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIDataManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIInFlowTypeManager.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIInFlow.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIAbstractInFlowType.h"

#include <QToolBox>

namespace GUI
{
    Core::FITKParameter* InFLowTypeGetSubData(const QString & type, CompSelectComBoxWidget* widget)
    {
		if (widget == nullptr) return nullptr;
		Interface::FITKPhysicsHandlerFactory* dataFactory = FITKAPP->getComponents()->getComponentTByName<Interface::FITKPhysicsHandlerFactory>("FITKPHengLEIPhysicsHandlerFactory");
		if (!dataFactory) return nullptr;
		Interface::FITKPHengLEIData* data = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();
		if (!data) return nullptr;

		QString solverName =  data->getSolver()->getDataObjectName();

		dataFactory->setInflowType(type, solverName);

		Interface::FITKPHengLEIInFlow* inFlow = data->getInFlow();
		Interface::FITKPHengLEIAbstractInFlowType* inFLowType = inFlow->getInFlowType();

		return inFLowType->getInFlowTypeParams();
    }

    InFlowWidget::InFlowWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::InFlowWidget();
        _ui->setupUi(this);

        init();
    }

    InFlowWidget::~InFlowWidget()
    {
        if (_ui) delete _ui;
    }

    void InFlowWidget::init()
    {
		if (_data_PHengLEI == nullptr) return;
		if (_dataManager_PHengLEI == nullptr) return;

		_inFlow =_data_PHengLEI->getInFlow();

		QString inFLowType = _inFlow->getInFlowType()->getDataObjectName();

		Core::FITKParameter* parameter = _inFlow->getInFlowType()->getInFlowTypeParams();

		updateInFlow();
    }



	void InFlowWidget::updateInFlow()
	{
		if (_dataManager_PHengLEI == nullptr) return;
		Interface::FITKPHengLEIInFlowTypeManager* inFlowManager =_dataManager_PHengLEI->getInFlowManager();
		if (inFlowManager == nullptr) return;
		if (_physicHandleFactory_PHengLEI == nullptr) return;
		if (_inFlow == nullptr) return;

		QList<QWidget*> widgetList{};
		Core::FITKParameter* additionalData = _inFlow->getInFlowAdditionalData();
		if (additionalData)
		{
			for (auto d : additionalData->getParameter())
			{
				if(d == nullptr) continue;
				QWidget* w = new Core::FITKWidgetComLine(d, this);
				widgetList.append(w);
			}
		}
		//InFlowType数据添加
		Interface::FITKPHengLEIAbstractInFlowType* inFLowType = _inFlow->getInFlowType();
		if (inFLowType)
		{
			QStringList options = inFlowManager->filterTurbence(_data_PHengLEI->getSolver()->getSolverType());
			CompSelectComBoxWidget* comp = new CompSelectComBoxWidget("Inflow Para Type", this);
			comp->setFunction(&InFLowTypeGetSubData);
			comp->setOptions(options);
			comp->setCurrentText(inFLowType->getDataObjectName());
			comp->setSubWidgetData(inFLowType->getInFlowTypeParams());
			widgetList.append(comp);
		}
		for (auto w : widgetList)
		{
			_ui->verticalLayout->addWidget(w);
		}
	}
}

