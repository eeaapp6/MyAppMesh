/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "EnergyModelWidget.h"
#include "ui_EnergyModelWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIModels.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIEnergyModel.h"
#include <QToolBox>
#include <QCheckBox>
#include <QLayoutItem>

namespace GUI
{
    EnergyModelWidget::EnergyModelWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::EnergyModelWidget();
        _ui->setupUi(this);

        init();

        // 连接信号槽，当_energyWidget存在且其参数控件是QCheckBox时
        if (_energyWidget && _energyWidget->getParamWidget()) {
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(_energyWidget->getParamWidget());
            if (checkBox) {
                connect(checkBox, &QCheckBox::stateChanged, this, &EnergyModelWidget::onEnergyWidgetStateChanged);
            }
        }
    }

    EnergyModelWidget::~EnergyModelWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void EnergyModelWidget::init()
    {
		if (_data_PHengLEI == nullptr) return;
		Interface::FITKPHengLEIModels* models = _data_PHengLEI->getModels();
        _energyModel = models->getEnergyModel();
		if (_energyModel == nullptr) return;

		Core::FITKParameter* paraData = _energyModel->getAdditionalData();
		if (paraData)
		{
            QList<Core::FITKAbstractEasyParam*> paraList = paraData->getParameter();
            for (int i = 0; i < paraList.count(); i++)
            {
				if (paraList[i] == nullptr) continue;
                Core::FITKWidgetComLine* comLine = new Core::FITKWidgetComLine(paraList[i], this);
                if (i == 0)
                {
                    _energyWidget = comLine;
                }
				_ui->verticalLayout->addWidget(comLine);
            }
		}

		if (_energyWidget && _energyWidget->getParamWidget()) {
			QCheckBox* checkBox = qobject_cast<QCheckBox*>(_energyWidget->getParamWidget());
			updateEnergyStateChanged(checkBox->checkState());
		}
    }

	void EnergyModelWidget::onEnergyWidgetStateChanged(int state)
	{
		updateEnergyStateChanged(state);
	}

	void EnergyModelWidget::updateEnergyStateChanged(int state)
	{
		if (state == Qt::Checked)
		{
			// 显示界面中除了_energyWidget以外，其它的界面控件
			for (int i = 0; i < _ui->verticalLayout->count(); ++i) {
				QLayoutItem* item = _ui->verticalLayout->itemAt(i);
				if (item && item->widget() && item->widget() != _energyWidget) {
					item->widget()->show();
				}
			}
		}
		else
		{
			// 隐藏界面上除了_energyWidget以外，其它的界面控件
			for (int i = 0; i < _ui->verticalLayout->count(); ++i) {
				QLayoutItem* item = _ui->verticalLayout->itemAt(i);
				if (item && item->widget() && item->widget() != _energyWidget) {
					item->widget()->hide();
				}
			}
		}
	}

}

