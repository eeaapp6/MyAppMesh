/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsColor.h"

#include "GUIDialog/ColorMapperEditDialog.h"
#include "GUIDialog/ColorLibaryDialog.h"
#include "GUIDialog/ColorLibaryEditDialog.h"
#include "GUIWidget/TreeWidget.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/MainTreeWidget.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

namespace OperModel
{
    OperatorsColor::OperatorsColor()
    {

    }

    bool OperatorsColor::execGUI()
    {
        if (_senderName == "colorMapEdit") {
            int objID = -1;
            argValue(objID_String, objID);
            if (objID <= 0)return false;
            Interface::FITKCFDPost3DManager* dataManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
            if (dataManager == nullptr)return false;
            Interface::FITKAbstractCFDPostData* postData = dataManager->getDataByID(objID);
            if (postData == nullptr)return false;
            GUI::ColorMapperEditDialog* dialog = new GUI::ColorMapperEditDialog(postData, this);
            dialog->show();
        }
        else if (_senderName == "actionColorLibary") {
            GUI::ColorLibaryDialog* dialog = new GUI::ColorLibaryDialog(this);
            dialog->show();
        }
        else if (_senderName == "actionColorLibEdit") {
            GUI::TreeWidget* treeWidget = _mainWindow->getTreeWidget()->getTreeWidget();
            if (treeWidget == nullptr)return false;
            GUI::ColorLibaryEditDialog* dialog = new GUI::ColorLibaryEditDialog(treeWidget->getCurrentDataID(), this);
            dialog->show();
        }
        return false;
    }

    bool OperatorsColor::execProfession()
    {
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper) {
            graphOper->reRender();
        }
        return false;
    }
}