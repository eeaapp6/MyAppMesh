/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MeshProcessWidget.h"
#include "ui_MeshProcessWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetHBox.h"
#include "FITK_Kernel/FITKEasyParam/FITKEasyParamWidgetFactory.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIMeshProcess.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include <QToolBox>

namespace GUI
{
    MeshProcessWidget::MeshProcessWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::MeshProcessWidget();
        _ui->setupUi(this);

        init();
    }

    MeshProcessWidget::~MeshProcessWidget()
    {
		for (int i = 0; i < _senders.size(); i++) {
			auto sender = _senders[i];
			QObject::disconnect(sender);
		}

        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void MeshProcessWidget::init()
    {
		if (_data_PHengLEI == nullptr) return;
		_meshProcess = _data_PHengLEI->getMeshProcess();
		if (_meshProcess == nullptr) return;

		Core::FITKParameter* meshData = _meshProcess->getAdditionalMeshData();
		if (meshData)
		{
			for (auto d : meshData->getParameter())
			{
				if(d == nullptr) continue;
				if (d->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPRadioGroup) {
					//QWidget* widget = Core::FITKEasyParamWidgetFactory::createWidget(d, this);
					//_ui->verticalLayout->addWidget(widget);
				}
				else {
					_ui->verticalLayout->addWidget(new Core::FITKWidgetComLine(d, this));
				}
			}
		}

		Core::FITKParameter* meshconversion = _meshProcess->getAdditionalMeshConversion();
		if (meshconversion)
		{
			QString countName = "Axis Rotate Count";
			QString orderName = "Axis Rotate Order";
			QString anglesName = "Axis Rotate Angles";
			Core::FITKAbstractEasyParam* countData = nullptr;
			QList<QWidget*> widgets = {};
			for (auto d : meshconversion->getParameter())
			{
				if (d == nullptr) continue;
				if (countName == d->getDataObjectName()) countData = d;
				QLayout*  meshLayout =_ui->_gBoxmMeshConversion->layout();
				QWidget* widget = new Core::FITKWidgetComLine(d, this);
				if (d->getDataObjectName() == orderName || d->getDataObjectName() == anglesName)widgets.append(widget);
				meshLayout->addWidget(widget);
			}

			QMetaObject::Connection connection = connect(countData, &Core::FITKAbstractEasyParam::sigDataChange, [=]() {
				Core::FITKParamInt* countInt = dynamic_cast<Core::FITKParamInt*>(countData);
				int count = countInt->getValue();
				if (count == 0)
				{
					for (QWidget* widget : widgets) {
						widget->setVisible(false);
					}
				}
				else if (count == 1)
				{
					for (QWidget* widget : widgets) {
						QList<Core::FITKWidgetHBox*> widgetHBoxList = widget->findChildren<Core::FITKWidgetHBox*>();
						Core::FITKWidgetHBox* widgetHBox = widgetHBoxList[0];
						QList<QWidget*> widgetList = widgetHBox->getSubWidget();
						for (int i = 0; i < count; i++)
						{
							QWidget* widget = widgetList[i];
							widget->setVisible(true);
						}
						for (int i = count; i< widgetList.size(); i ++)
						{
							QWidget* widget = widgetList[i];
							widget->setVisible(false);
						}
						widget->setVisible(true);
					}
				}
				else if (count == 2)
				{
					for (QWidget* widget : widgets) {
						QList<Core::FITKWidgetHBox*> widgetHBoxList = widget->findChildren<Core::FITKWidgetHBox*>();
						Core::FITKWidgetHBox* widgetHBox = widgetHBoxList[0];
						QList<QWidget*> widgetList = widgetHBox->getSubWidget();
						for (int i = 0; i < count; i++)
						{
							QWidget* widget = widgetList[i];
							widget->setVisible(true);
						}
						for (int i = count; i < widgetList.size(); i++)
						{
							QWidget* widget = widgetList[i];
							widget->setVisible(false);
						}
						widget->setVisible(true);
					}
				}
				else if (count == 3)
				{
					for (QWidget* widget : widgets) {
						QList<Core::FITKWidgetHBox*> widgetHBoxList = widget->findChildren<Core::FITKWidgetHBox*>();
						Core::FITKWidgetHBox* widgetHBox = widgetHBoxList[0];
						QList<QWidget*> widgetList = widgetHBox->getSubWidget();
						for (auto subWidget : widgetList) {
							if (subWidget == nullptr)continue;
							subWidget->setVisible(true);
						}
						widget->setVisible(true);
					}
				}
			});
			_senders.append(connection);
		}

		Core::FITKParameter* meshpartition = _meshProcess->getAdditionalMeshPartion();
		if (meshpartition)
		{
			Core::FITKAbstractEasyParam* gridTypePara =meshData->getDataByName("Grid Type");
			QString numberOfMultigrid = "Multi-grid Count";
			Core::FITKAbstractEasyParam* multiGridNumberPara = nullptr;
			QWidget* multiGridNumberWidget = {};
			for (auto d : meshpartition->getParameter())
			{
				if (d == nullptr) continue;
				QLayout*  meshLayout = _ui->_gBoxMeshPartion->layout();
				QWidget* widget = new Core::FITKWidgetComLine(d, this);

				if (d->getDataObjectName() == numberOfMultigrid) multiGridNumberWidget = widget;
				meshLayout->addWidget(widget);
			}

			Core::FITKParamCombox* gridTypeCombox = dynamic_cast<Core::FITKParamCombox*>(gridTypePara);
			if (gridTypeCombox->getCurrentValue() == "Unstruct")
			{
				multiGridNumberWidget->setVisible(false);
			}
			else
			{
				multiGridNumberWidget->setVisible(true);
			}

			QMetaObject::Connection connection = connect(gridTypePara, &Core::FITKAbstractEasyParam::sigDataChange, [=]() {
				Core::FITKParamCombox* gridTypeCombox = dynamic_cast<Core::FITKParamCombox*>(gridTypePara);
				if (gridTypeCombox->getCurrentValue() == "Unstruct")
				{
					multiGridNumberWidget->setVisible(false);
				}
				else
				{
					multiGridNumberWidget->setVisible(true);
				}
				
			});
			_senders.append(connection);
		}
    }
}

