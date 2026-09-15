/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsOutputSetting.h"

#include "GUIFrame/PropertyWidget.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIDialog/GUICalculateDialog/OutputSettingWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIerDriver.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIParaWriterKey.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDHypersonsic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDSubsonic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDSupersonic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDTransonic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIAbsSolver.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"


#include <QDir>

namespace ModelOper
{
    OperatorsOutputSetting::OperatorsOutputSetting()
    {

    }

    OperatorsOutputSetting::~OperatorsOutputSetting()
    {

    }

    bool OperatorsOutputSetting::execGUI()
    {
		GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
		if (mainWindow == nullptr)return false;
		GUI::PropertyWidget* propertyWidget = mainWindow->getPropertyWidget();
		if (propertyWidget == nullptr)return false;

		GUI::OutputSettingWidget* widget = new GUI::OutputSettingWidget(this, propertyWidget);
		propertyWidget->setWidget(widget); 
        return false;
    }

    bool OperatorsOutputSetting::execProfession()
    {
        return true;
    }
	void OperatorsOutputSetting::eventProcess(int index, QVariant value)
	{
	}
}

