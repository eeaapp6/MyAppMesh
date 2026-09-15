/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsDelete.h"

#include "GUIWidget/ProbeCurveWidget.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"
#include "PostGraphAdaptor/PostGraphObjectBase.h"
#include "PostGraphAdaptor/PostGraphObjectAgent.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

namespace OperModel
{
    OperatorsDelete::OperatorsDelete()
    {

    }

    bool OperatorsDelete::execGUI()
    {
        int objID = -1;
        argValue(objID_String, objID);
        if (objID <= 0)return false;
        Interface::FITKCFDPost3DManager* dataManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManager == nullptr)return false;
        Interface::PostGraphObjectManager* actorManager = Interface::PostGraphObjectManager::getInstance();
        if (actorManager == nullptr)return false;

        //删除渲染数据
        int actorID = -1;
        for (int i = 0; i < actorManager->getDataCount(); i++) {
            Interface::PostGraphObjectAgent* agent = actorManager->getDataByIndex(i);
            if (agent == nullptr)continue;
            Interface::PostGraphObjectBase* graphObj = agent->getGraphObject();
            if (graphObj == nullptr)continue;
            if (graphObj->getPostID() == objID) {
                actorID = agent->getDataObjectID();
                break;
            }
        }
        actorManager->removeDataByID(actorID);
        //删除vtk数据
        dataManager->removeDataByID(objID);

        //删除曲线界面
        Comp::FITKTabWidget* tabWidget = _mainWindow->getEventWidget();
        QList<GUI::ProbeCurveWidget*> curveWidgets = tabWidget->findChildren<GUI::ProbeCurveWidget*>();
        for (GUI::ProbeCurveWidget* curveWid : curveWidgets) {
            if (curveWid->getCurrentID() == objID) {
                delete curveWid;
                curveWid = nullptr;
            }
        }
        if (tabWidget->count() > 0)tabWidget->show();
        else tabWidget->hide();

        return true;
    }

    bool OperatorsDelete::execProfession()
    {
        //更新渲染窗口
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper) {
            graphOper->reRender();
        }
        
        //更新树界面
        EventOper::ParaWidgetInterfaceOperator* treeOper = FITKOPERREPO->getOperatorT<EventOper::ParaWidgetInterfaceOperator>("ModelTreeEvent");
        if (treeOper) {
            treeOper->updateTree();
        }

        //更新界面
        EventOper::ParaWidgetInterfaceOperator* infoOper = FITKOPERREPO->getOperatorT<EventOper::ParaWidgetInterfaceOperator>("OperatorsWidget");
        if (infoOper) {
            infoOper->CurrentObjectChange(-1);
        }
        return true;
    }
}