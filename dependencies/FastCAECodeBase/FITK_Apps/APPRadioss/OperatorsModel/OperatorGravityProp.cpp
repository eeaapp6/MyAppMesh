/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorGravityProp.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"

#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"

#include "GUIDialog/GUIToolDialog/GUIRenameDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIGravityDialog.h"

namespace OperModel
{
    bool OperatorGravityProp::execGUI()
    {
        if (_emitter == nullptr)
            return false;
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        if (!mw) return false;
        QString name = _emitter->objectName();
        if (name.isEmpty())return false;
        if (name == "actionGravityCreate")
        {
            GUI::GUIGravityDialog* dialog = new GUI::GUIGravityDialog(nullptr, this, mw);
            dialog->show();
        }
        else if (name == "actionGravityEdit")
        {
            int gravityID = -1;
            this->argValue("SelectedItemDataID", gravityID);

            Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
                getPhysicsData<Radioss::FITKRadiossCase>();
            if (caseObj == nullptr) return false;
            Radioss::FITKGravityManager* gravityMgr = caseObj->getGravityManager();
            if (!gravityMgr)return false;
            Radioss::FITKGravity* gravity = gravityMgr->getDataByID(gravityID);
            if (!gravity)return false;
            GUI::GUIGravityDialog* dialog = new GUI::GUIGravityDialog(gravity, this, mw);
            dialog->show();
        }
        else if (name == "")
        {
            return this->gravityPropRenameOper();
        }
        
        else if (name == "")
        {
            return deleteGravity();
        }
        return true;
    }
    bool OperatorGravityProp::execProfession()
    {
        // 通过树形菜单事件处理器刷新树
        QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }

        int objID = -1;
        this->argValue("objID", objID);
        //更新渲染
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateGraph(objID,true);
        }
        return false;
    }

    bool OperatorGravityProp::gravityPropRenameOper()
    {
        return true;
    }
    bool OperatorGravityProp::deleteGravity()
    {
        return true;
    }
}

