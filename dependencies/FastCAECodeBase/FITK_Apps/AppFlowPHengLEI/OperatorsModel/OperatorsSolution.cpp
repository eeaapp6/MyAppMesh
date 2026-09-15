/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsSolution.h"

#include "GUIFrame/PropertyWidget.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIDialog/GUICalculateDialog/PHengLEISolutionWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace ModelOper
{
    OperatorsSolution::OperatorsSolution()
    {

    }

    OperatorsSolution::~OperatorsSolution()
    {

    }

    bool OperatorsSolution::execGUI()
    {
        GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWindow == nullptr)return false;
        GUI::PropertyWidget* propertyWidget = mainWindow->getPropertyWidget();
        if (propertyWidget == nullptr)return false;

        GUI::PHengLEISolutionWidget* widget = new GUI::PHengLEISolutionWidget(this, propertyWidget);
        propertyWidget->setWidget(widget);

        return true;
    }

    bool OperatorsSolution::execProfession()
    {
        return true;
    }
}

