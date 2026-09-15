/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsPHengLEIBoundaryConditions.h"

#include "GUIFrame/PropertyWidget.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIDialog/GUICalculateDialog/PHengLEIBoundaryCreateDialog.h"
#include "GUIDialog/GUICalculateDialog/PHengLEIBoundaryWidget.h"
#include "GUIDialog/GUICalculateDialog/PHengLEIBoundaryDeleteDialog.h"


#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIBoundary.h"

namespace ModelOper
{
    OperatorsPHengLEIBoundaryConditions::OperatorsPHengLEIBoundaryConditions()
    {

    }

    OperatorsPHengLEIBoundaryConditions::~OperatorsPHengLEIBoundaryConditions()
    {

    }

    bool OperatorsPHengLEIBoundaryConditions::execGUI()
    {
        GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWindow == nullptr)return false;
        GUI::PropertyWidget* propertyWidget = mainWindow->getPropertyWidget();
        if (propertyWidget == nullptr)return false;
        auto physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();
        if (physicsData == nullptr)return false;
        auto boundaryManager = physicsData->getBoundaryManager();
        if (boundaryManager == nullptr)return false;

        switch (_operType){
        case ModelOper::OperPHengLEIManagerBase::Create: {
            GUI::PHengLEIBoundaryCreateDialog* dialog = new GUI::PHengLEIBoundaryCreateDialog(this);
            dialog->show();
            break;
        }
        case ModelOper::OperPHengLEIManagerBase::Edit: {
            int objID = -1;
            this->argValue<int>("objID", objID);
            Interface::FITKPHengLEIBoundary* boundaryObj = boundaryManager->getDataByID(objID);
            GUI::PHengLEIBoundaryWidget* widget = new GUI::PHengLEIBoundaryWidget(boundaryObj, this, propertyWidget);
            propertyWidget->setWidget(widget);
            break;
        }
        case ModelOper::OperPHengLEIManagerBase::Delete: {
            int objID = -1;
            this->argValue<int>("objID", objID);
            Interface::FITKPHengLEIBoundary* boundaryObj = boundaryManager->getDataByID(objID);
			GUI::PHengLEIBoundaryDeleteDialog* dialog = new GUI::PHengLEIBoundaryDeleteDialog(boundaryObj, this);
			dialog->show();
        }
        }

        return false;
    }

    bool OperatorsPHengLEIBoundaryConditions::execProfession()
    {
        // 获取模型树控制器
        auto treeOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::TreeEventOperator>("ModelTreeEvent");
        if (treeOper == nullptr) return false;

        switch (_operType){
        case ModelOper::OperPHengLEIManagerBase::Create:{
            treeOper->updateTree();
            break;
        }
        case ModelOper::OperPHengLEIManagerBase::Edit: {
            break;
        }
        case ModelOper::OperPHengLEIManagerBase::Delete: {
            treeOper->updateTree();
            break;
        }
        }

        return true;
    }
}

