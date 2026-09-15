/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorBCS.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"

#include "GUIDialog/GUIToolDialog/GUIRenameDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIBCSDialog.h"
#include "GUIFrame/MainWindow.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
namespace OperModel
{
    bool OperatorBCS::execGUI()
    {
        //触发器
        if (_emitter == nullptr)
            return false;
        QString name = _emitter->objectName();
        if (name.isEmpty()) return false;
        //创建
        if (name == "actionBCSCreate")
        {
            this->bcsCreateOper();
        }
        //重命名
        else if (name == "actionBCSRename")
        {
            return this->bcsRenameOper();
        }
        //编辑
        else if (name == "actionBCSEdit")
        {
            this->bcsEditOper();
        }
        //删除
        else if (name == "actionBCSDelete")
        {
            return this->bcsDeleteOper();
        }

        return false;
    }

    bool OperatorBCS::execProfession()
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
            operGraph->updateGraph(objID, true);
        }
        return true;
    }

    bool OperatorBCS::bcsCreateOper()
    {
        //修改名称
        GUI::GUIBCSDialog* dialog = new GUI::GUIBCSDialog(this, FITKAPP->getGlobalData()->getMainWindow());
        dialog->show();
        return true;
    }

    bool OperatorBCS::bcsRenameOper()
    {
        //获取边界ID
        int bcsID = 0;
        if (!this->argValue<int>("BCSID", bcsID)) return false;
        //获取算例数据
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return false;
        //获取边界数据与边界管理器
        Radioss::FITKRadiossBCSManager* bcsManager = caseData->getBCSManager();
        if (!bcsManager) return false;
        Radioss::FITKAbstractBCS* bcsData = bcsManager->getDataByID(bcsID);
        if (!bcsData) return false;
        //修改名称
        GUI::GUIRenameDialog renameDialog(this, bcsData, bcsManager, FITKAPP->getGlobalData()->getMainWindow());
        renameDialog.exec();
        return true;
    }

    bool OperatorBCS::bcsEditOper()
    {
        //获取边界ID
        int bcsID = 0;
        if (!this->argValue<int>("SelectedItemDataID", bcsID)) return false;
        //获取算例数据
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return false;
        //获取边界数据与边界管理器
        Radioss::FITKRadiossBCSManager* bcsManager = caseData->getBCSManager();
        if (!bcsManager) return false;
        Radioss::FITKAbstractBCS* bcsData = bcsManager->getDataByID(bcsID);
        if (!bcsData) return false;
        //修改名称
        GUI::GUIBCSDialog* dialog = new GUI::GUIBCSDialog(dynamic_cast<Radioss::FITKRadiossBCS*>(bcsData), this, FITKAPP->getGlobalData()->getMainWindow());
        dialog->show();
        return true;
    }

    bool OperatorBCS::bcsDeleteOper()
    {
        //获取边界ID
        int bcsID = 0;
        if (!this->argValue<int>("BCSID", bcsID)) return false;
        //获取算例数据
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return false;
        //获取边界管理器
        Radioss::FITKRadiossBCSManager* bcsManager = caseData->getBCSManager();
        if (!bcsManager) return false;
        //移除边界
        bcsManager->removeDataByID(bcsID);
        return true;
    }

}