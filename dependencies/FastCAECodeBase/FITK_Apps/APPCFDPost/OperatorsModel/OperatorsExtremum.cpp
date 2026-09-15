/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsExtremum.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/MainTreeWidget.h"
#include "GUIWidget/TreeWidget.h"
#include "GUIDialog/ExtremumDialog.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace OperModel
{
    bool OperatorsExtremum::execGUI()
    {
        GUI::MainWindow* mainWin = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWin == nullptr)return false;
        GUI::TreeWidget* treeWidget = mainWin->getTreeWidget()->getTreeWidget();
        if (treeWidget == nullptr)return false;

        GUI::ExtremumDialog* dialog = new GUI::ExtremumDialog(treeWidget->getCurrentDataID(), this);
        dialog->show();
        return false;
    }

    bool OperatorsExtremum::execProfession()
    {
        return false;
    }
}

