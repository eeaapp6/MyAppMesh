/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ViscousModelWidget.h"
#include "ui_ViscousModelWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"

#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIViscousModel.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIViscousType.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIDataManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIViscousTypeManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIPhysicsHandlerFactory.h"
#include "CompSelectComBoxWidget.h"
#include <QToolBox>
#include <QList>
#include <QWidget>

namespace GUI
{
	Core::FITKParameter* ViscousTypeGetSubData(const QString & type, CompSelectComBoxWidget* widget)
	{
		if (widget == nullptr) return nullptr;
		Interface::FITKPhysicsHandlerFactory* dataFactory = FITKAPP->getComponents()->getComponentTByName<Interface::FITKPhysicsHandlerFactory>("FITKPHengLEIPhysicsHandlerFactory");
		if (!dataFactory) return nullptr;
		Interface::FITKPHengLEIData* data = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();
		if (!data) return nullptr;

		dataFactory->setViscousType(type);

		Interface::FITKPHengLEIViscousModel* viscousModel = data->getViscousModel();
		Interface::FITKPHengLEIViscousType* viscousType = viscousModel->getViscousType();

		return viscousType->getViscousTypeParams();
	}


    ViscousModelWidget::ViscousModelWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::ViscousModelWidget();
        _ui->setupUi(this);

        init();
    }

    ViscousModelWidget::~ViscousModelWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void ViscousModelWidget::init()
    {
		if (_data_PHengLEI == nullptr) return;
		if (_dataManager_PHengLEI == nullptr) return;

		_ViscousModel = _data_PHengLEI->getViscousModel();
		if (_ViscousModel == nullptr) return;

		Interface::FITKPHengLEIViscousTypeManager* viscousTypeManager = _dataManager_PHengLEI->getViscousTypeManager();


		// 添加DESType
		Core::FITKParameter* paraData = _ViscousModel->getAdditionalVicousModelData();
		if (paraData)//！
		{
			for (auto d : paraData->getParameter())
			{
				if(d == nullptr) continue;
				_ui->_vLayoutDESType->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		//InFlowType数据添加
		Interface::FITKPHengLEIViscousType* viscousType = _ViscousModel->getViscousType();
		
		QList<QWidget*> widgetList{};
		if (viscousType)
		{
			//QStringList options = viscousTypeManager->filterTurbence();
			//CompSelectComBoxWidget* comp = new CompSelectComBoxWidget("Viscous Type", this);
			//comp->setFunction(&ViscousTypeGetSubData);
			//comp->setOptions(options);
			//comp->setCurrentText(viscousType->getDataObjectName());
			//comp->setSubWidgetData(viscousType->getViscousTypeParams());
			//widgetList.append(comp);
		}
		for (auto w : widgetList)
		{
			_ui->_vLayoutViscousType->addWidget(w);
		}
    }
}

