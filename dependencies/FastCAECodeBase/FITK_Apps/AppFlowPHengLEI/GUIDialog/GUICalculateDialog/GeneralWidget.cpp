/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GeneralWidget.h"
#include "ui_GeneralWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIGeneral.h"
#include <QToolBox>

namespace GUI
{
    GeneralWidget::GeneralWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::GeneralWidget();
        _ui->setupUi(this);

        init();
    }

    GeneralWidget::~GeneralWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void GeneralWidget::init()
    {
		if (_data_PHengLEI == nullptr) return;
		_general = _data_PHengLEI->getGeneral();
		if (_general == nullptr) return;

		QVBoxLayout* vLayoutMeshScale = new QVBoxLayout;
		Core::FITKParameter* convertUnits = _general->getMeshScale();
		if (convertUnits)
		{
			for (auto d : convertUnits->getParameter())
			{
				if(d == nullptr) continue;
				vLayoutMeshScale->addWidget(new Core::FITKWidgetComLine(d, this));
			}
			_ui->group_MeshScale->setLayout(vLayoutMeshScale);
		}


		QVBoxLayout *vLayoutAxisRotate = new QVBoxLayout;
		Core::FITKParameter* axisRotateData = _general->getAxisRotateData();
		if (axisRotateData)
		{
			for (auto d : axisRotateData->getParameter())
			{
				if(d == nullptr) continue;
				vLayoutAxisRotate->addWidget(new Core::FITKWidgetComLine(d, this));
			}
			_ui->group_AxisRotate->setLayout(vLayoutAxisRotate);
		}

		QGridLayout* gLayoutSolver = dynamic_cast<QGridLayout *>(_ui->group_Solver->layout());
		Core::FITKParameter* transientData = _general->getTransientData();
		if (transientData)
		{
			for (auto d : transientData->getParameter())
			{
				if (d == nullptr) continue;

				gLayoutSolver->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}
    }
}

