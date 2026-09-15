/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsSlice.h"

#include "GUIDialog/SliceDialog.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/MainTreeWidget.h"
#include "GUIWidget/TreeWidget.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostIsosurface.h"

namespace OperModel
{
    OperatorsSlice::OperatorsSlice()
    {

    }

    bool OperatorsSlice::execGUI()
    {
        GUI::MainWindow* mainWin = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWin == nullptr)return false;
        GUI::TreeWidget* treeWidget = mainWin->getTreeWidget()->getTreeWidget();
        if (treeWidget == nullptr)return false;
        //获取树节点当前数据对象id
        if (_senderName == "actionSliceCreate") {
            GUI::SliceDialog* dialog = new GUI::SliceDialog(treeWidget->getCurrentDataID(), this, true);
            dialog->show();
        }
        else if (_senderName == "actionSliceEdit") {
            int objID = -1;
            argValue(objID_String, objID);
            if (objID <= 0)return false;

            GUI::SliceDialog* dialog = new GUI::SliceDialog(objID, this, false);
            dialog->show();
        }
        return false;
    }

    bool OperatorsSlice::execProfession()
    {
        Interface::FITKCFDPost3DManager* manager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (manager == nullptr)return false;
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper == nullptr)return false;
        EventOper::ParaWidgetInterfaceOperator* treeOper = FITKOPERREPO->getOperatorT<EventOper::ParaWidgetInterfaceOperator>("ModelTreeEvent");
        if (treeOper == nullptr)return false;

        int objID = -1;
        argValue(objID_String, objID);
        Interface::FITKAbstractCFDPostData* data = manager->getDataByID(objID);
        if (data) {
            graphOper->updateSingle(objID);
        }

        treeOper->updateTree();
        return false;
    }
}