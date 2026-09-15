/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsCoupling.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/PropertyWidget.h"
#include "GUIDialog/GUICalculateDialog/DealCouplingWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace ModelOper 
{    
    OperatorsCoupling::OperatorsCoupling()
    {

    }

    OperatorsCoupling::~OperatorsCoupling()
    {

    }

    bool OperatorsCoupling::execGUI()
    {
        GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWindow == nullptr)return false;
        GUI::PropertyWidget* propertyWidget = mainWindow->getPropertyWidget();
        if (propertyWidget == nullptr)return false;

        GUI::DealCouplingWidget* widget = new GUI::DealCouplingWidget(this, FITKAPP->getGlobalData()->getMainWindow());
        propertyWidget->setWidget(widget);

        return false;
    }

    bool OperatorsCoupling::execProfession()
    {
        return false;
    }
}

