/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MeshParaWidget.h"
#include "ui_MeshParaWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIMeshPara.h"//
#include <QToolBox>

namespace GUI
{
    MeshParaWidget::MeshParaWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::MeshParaWidget();
        _ui->setupUi(this);

        init();
    }

    MeshParaWidget::~MeshParaWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void MeshParaWidget::init()
    {
		if (_data_PHengLEI == nullptr) return;
		_MeshPara = _data_PHengLEI->getMeshPara();
		if (_MeshPara == nullptr) return;

		Core::FITKParameter* paraData = _MeshPara->getAdditionalMeshData();
		if (paraData)
		{
			int index = 0;
			for (auto d : paraData->getParameter())
			{
				if(d == nullptr) continue;
				_ui->verticalLayout->insertWidget(index,new Core::FITKWidgetComLine(d, this));
			}

		}
		Core::FITKParameter* paraData1 = _MeshPara->getAdditionalForceReference();
		if (paraData1)
		{
			for (auto d : paraData1->getParameter())
			{
				if (d == nullptr) continue;
				_ui->verticalLayout_3->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}
		Core::FITKParameter* paraData2 = _MeshPara->getAdditionalReferencePoint();
		if (paraData2)
		{
			for (auto d : paraData2->getParameter())
			{
				if (d == nullptr) continue;
				_ui->verticalLayout_5->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}
    }
}

