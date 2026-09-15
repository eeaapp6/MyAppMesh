/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorRigidWallProp.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"

#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"

#include "GUIDialog/GUIToolDialog/GUIRenameDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIRigidWallTypeDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIRigidWallPlaneDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIRigidWallCylinderDialog.h"


namespace OperModel
{
    bool OperatorRigidWallProp::execGUI()
    {
        if (_emitter == nullptr)
            return false;
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        if (!mw) return false;
        QString name = _emitter->objectName();
        if (name.isEmpty())return false;
        if (name == "actionRigidWallCreate")
        {
            GUI::GUIRigidWallTypeDialog dialog;
            if (dialog.exec() != QDialog::Accepted)return false;
            int type = dialog.selectedRigidWallType();
            this->setArgs("RigidWallName", dialog.getRigidWallName());
            switch (type)
            {
            case 0: {
                GUI::GUIRigidWallPlaneDialog* dialog = new GUI::GUIRigidWallPlaneDialog(nullptr, this ,mw);
                dialog->show();
                break;
            }
            case 1: {
                GUI::GUIRigidWallCylinderDialog* dialog = new GUI::GUIRigidWallCylinderDialog(nullptr, this, mw);
                dialog->show();
                break;
            }
            default:
                return false;
            }
        }
        else if (name == "actionRigidWallEdit")
        {
            int rigidWallID = -1;
            this->argValue("SelectedItemDataID", rigidWallID);

            Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
                getPhysicsData<Radioss::FITKRadiossCase>();
            if (caseObj == nullptr) return false;
            Radioss::FITKRadiossRWallManager* rWallMgr = caseObj->getRWallManager();
            if (!rWallMgr)return false;
            Radioss::FITKAbstractRWall* rigidWall = rWallMgr->getDataByID(rigidWallID);
            if (!rigidWall)return false;

            if (dynamic_cast<Radioss::FITKRWallPlane*>(rigidWall) != nullptr)
            {
                GUI::GUIRigidWallPlaneDialog* dialog = new GUI::GUIRigidWallPlaneDialog(rigidWall, this, mw);
                dialog->show();
            }
            else if (dynamic_cast<Radioss::FITKRWallCylinder*>(rigidWall) != nullptr)
            {
                GUI::GUIRigidWallCylinderDialog* dialog = new GUI::GUIRigidWallCylinderDialog(rigidWall, this, mw);
                dialog->show();
            }

        }
        else if (name == "")
        {
            return this->rigidWallPropRenameOper();
        }
        
        else if (name == "")
        {
            return deleteRigidWall();
        }
        return false;
    }
    bool OperatorRigidWallProp::execProfession()
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

    bool OperatorRigidWallProp::rigidWallPropRenameOper()
    {
        return true;
    }
    bool OperatorRigidWallProp::deleteRigidWall()
    {
        return true;
    }
}

