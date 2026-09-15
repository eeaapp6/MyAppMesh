/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsSetupGeneral.h"

#include "GUIFrame/PropertyWidget.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIDialog/GUICalculateDialog/GeneralWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace ModelOper
{
    OperatorsSetupGeneral::OperatorsSetupGeneral()
    {

    }

    OperatorsSetupGeneral::~OperatorsSetupGeneral()
    {

    }

    bool OperatorsSetupGeneral::execGUI()
    {
        GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWindow == nullptr)return false;
        GUI::PropertyWidget* propertyWidget = mainWindow->getPropertyWidget();
        if (propertyWidget == nullptr)return false;

        GUI::GeneralWidget* widget = new GUI::GeneralWidget(this, propertyWidget);
        propertyWidget->setWidget(widget);

        return true;
    }

    bool OperatorsSetupGeneral::execProfession()
    {
        return true;
    }
}

