/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OutputSettingWidget.h"
#include "ui_OutputSettingWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIOutputSetting.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include <QToolBox>

namespace GUI
{
    OutputSettingWidget::OutputSettingWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::OutputSettingWidget();
        _ui->setupUi(this);

        init();
    }

    OutputSettingWidget::~OutputSettingWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void OutputSettingWidget::init()
    {
		if (_data_PHengLEI == nullptr) return;
		_outputSetting = _data_PHengLEI->getOutputSetting();
		if (_outputSetting == nullptr) return;
		
		// 在verLayout_BasicFlow布局中添加BasicFlow参数
		Core::FITKParameter* basicFlowData = _outputSetting->getBasicFlowData();
		if (basicFlowData)
		{
			for (auto d : basicFlowData->getParameter())
			{
				if(d == nullptr) continue;
				_ui->verLayout_BasicFlow->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		// 在verLayout_Viscosity布局中添加viscosity参数
		Core::FITKParameter* viscosityData = _outputSetting->getViscosityData();
		if (viscosityData)
		{
			for (auto d : viscosityData->getParameter())
			{
				if (d == nullptr) continue;
				_ui->verLayout_Viscosity->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		// 在verLayout_Vorticity布局中添加vorticityData参数
		Core::FITKParameter* vorticityData = _outputSetting->getVorticityData();
		if (viscosityData)
		{
			for (auto d : vorticityData->getParameter())
			{
				if (d == nullptr) continue;
				_ui->verLayout_Vorticity->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		// 在verLayout_TurbulenceModel布局中添加turbulenceModelData参数
		Core::FITKParameter* turbulenceModelData = _outputSetting->getTurbulenceModelData();
		if (turbulenceModelData)
		{
			for (auto d : turbulenceModelData->getParameter())
			{
				if (d == nullptr) continue;
				_ui->verLayout_TurbulenceModel->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		// 在verLayout_NonEquilibrium布局中添加nonEquilibriumData参数
		Core::FITKParameter* nonEquilibriumData = _outputSetting->getNonEquilibriumData();
		if (nonEquilibriumData)
		{
			for (auto d : nonEquilibriumData->getParameter())
			{
				if (d == nullptr) continue;
				_ui->verLayout_NonEquilibrium->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		// 在verLayout_DerivedFlow布局中添加derivedFlowData参数
		Core::FITKParameter* derivedFlowData = _outputSetting->getDerivedFlowData();
		if (derivedFlowData)
		{
			for (auto d : derivedFlowData->getParameter())
			{
				if (d == nullptr) continue;
				_ui->verLayout_DerivedFlow->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		// 在verLayout_Dimensional布局中添加dimensionalData参数
		Core::FITKParameter* dimensionalData = _outputSetting->getDimensionalData();
		if (dimensionalData)
		{
			for (auto d : dimensionalData->getParameter())
			{
				if (d == nullptr) continue;
				_ui->verLayout_Dimensional->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		// 在verLayout_Dimensionless布局中添加dimensionlessData参数
		Core::FITKParameter* dimensionlessData = _outputSetting->getDimensionlessData();
		if (dimensionlessData)
		{
			for (auto d : dimensionlessData->getParameter())
			{
				if (d == nullptr) continue;
				_ui->verLayout_Dimensionless->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}


    }
}

