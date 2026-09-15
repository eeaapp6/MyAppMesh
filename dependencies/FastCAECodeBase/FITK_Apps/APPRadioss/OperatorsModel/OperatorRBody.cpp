/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorRBody.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKConnectionRBODY.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"

#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"

#include "GUIDialog/GUIAnalysisDialog/GUIRBodyDialog.h"

#include "GUIDialog/GUIToolDialog/GUIRenameDialog.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
namespace OperModel
{
    bool OperatorRBody::execGUI()
    {
        if (_emitter == nullptr)
            return false;
        QString name = _emitter->objectName();
        if (name.isEmpty())return false;
        if (name == "actionRBodyCreate")
        {
            this->createRigidBodyOper();
        }
        else if (name == "actionRBodyEdit")
        {
            this->editRigidBodyOper();
        }
        return false;
    }

    bool OperatorRBody::execProfession()
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
        int partID = -1;
        this->argValue("partID", partID);
        //更新渲染
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateGraph(objID, true);
            operGraph->updateGraph(partID, true);
        }
        return false;
    }

    bool OperatorRBody::createRigidBodyOper()
    {
        GUI::GUIRBodyDialog* dialog = new GUI::GUIRBodyDialog(this, FITKAPP->getGlobalData()->getMainWindow());
        dialog->show();
        return true;
    }

    bool OperatorRBody::editRigidBodyOper()
    {
        Radioss::FITKConnectionRBODY* data = dynamic_cast<Radioss::FITKConnectionRBODY*>(this->getConnectionData());
        if (!data) return false;
        GUI::GUIRBodyDialog* dialog = new GUI::GUIRBodyDialog(data, this, FITKAPP->getGlobalData()->getMainWindow());
        dialog->show();
        return true;
    }

    Radioss::FITKAbstractConnection * OperatorRBody::getConnectionData()
    {
        // 获取ID
        int id = -1;
        this->argValue("SelectedItemDataID", id);
        if (id < 0) return nullptr;
        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return nullptr;
        Radioss::FITKConnectionManager * manager = dataCase->getConnectionManager();
        if (!manager) return nullptr;
        //获取数据
        return manager->getDataByID(id);
    }

}