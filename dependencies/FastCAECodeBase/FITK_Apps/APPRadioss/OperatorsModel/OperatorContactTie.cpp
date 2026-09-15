/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorContactTie.h"
#include "GUIDialog/GUIAnalysisDialog/GUIContactTieDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"
#include "OperatorsInterface/TreeEventOperator.h"

namespace OperModel
{

    bool OperatorContactTie::execGUI()
    {
        if (!_emitter) return false;
        QString name = _emitter->objectName();
        if ("actionContactTieCreate" == name)
        {
            this->operCreateContactTie();
        }
        else if ("actionContactTieEdit" == name)
        {
            this->operEditContactTie();
        }


        return false;
    }

    bool OperatorContactTie::execProfession()
    {
        // 通过树形菜单事件处理器刷新树
        QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }
        return true;
    }

    bool OperatorContactTie::operCreateContactTie()
    {
        GUI::GUIContactTieDialog* dialog = new GUI::GUIContactTieDialog(this, FITKAPP->getGlobalData()->getMainWindow());
        dialog->show();
        return true;
    }

    bool OperatorContactTie::operEditContactTie()
    {
        Radioss::FITKInteractionKinematicTied* data = dynamic_cast<Radioss::FITKInteractionKinematicTied*>(this->getInteractionData());
        if (!data) return false;
        GUI::GUIContactTieDialog* dialog = new GUI::GUIContactTieDialog(data, this, FITKAPP->getGlobalData()->getMainWindow());
        dialog->show();
        return true;
    }
    Radioss::FITKAbstractInteraction * OperatorContactTie::getInteractionData()
    {
        // 获取属性ID
        int contactID = -1;
        this->argValue("SelectedItemDataID", contactID);
        if (contactID < 0) return nullptr;
        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return nullptr;
        Radioss::FITKInteractionManager * interactionManager = dataCase->getInteractionManager();
        if (!interactionManager) return nullptr;
        //获取数据
        return interactionManager->getDataByID(contactID);
    }
}

