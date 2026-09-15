/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsResidaul.h"

#include "GUIFrame/PropertyWidget.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIDialog/GUICalculateDialog/ResidaulWidget.h"
#include "GUIDialog/GUICalculateDialog/SwitchMonitorDialog.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"


namespace ModelOper
{
	OperatorsResidaul::OperatorsResidaul()
    {

    }

    OperatorsResidaul::~OperatorsResidaul()
    {

    }

    bool OperatorsResidaul::execGUI()
    {
        GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWindow == nullptr)return false;
        GUI::PropertyWidget* propertyWidget = mainWindow->getPropertyWidget();
        if (propertyWidget == nullptr)return false;

        //GUI::ResidaulWidget* widget = new GUI::ResidaulWidget(this, propertyWidget);
        //propertyWidget->setWidget(widget);

		GUI::SwitchMonitorDialog* dialog = new GUI::SwitchMonitorDialog(this);
		dialog->show();

        return true;
    }

    bool OperatorsResidaul::execProfession()
    {
        return true;
    }
}

