/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorPart.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

#include "GUIDialog/GUIToolDialog/GUIRenameDialog.h"

namespace OperModel
{
    bool OperatorPart::execGUI()
    {
        if (_emitter == nullptr) return false;
        QString name = _emitter->objectName();
        bool ok = false;
        if (name == "actionPartCreate")
        {
            this->partCreateOper();
        }
        else if (name == "actionPartRename")
        {
            ok = this->partRenameOper();
        }
        else if (name == "actionPartDelete")
        {
            ok = this->partDeleteOper();
        }
        return ok;
    }

    bool OperatorPart::execProfession()
    {
        // 获取部件ID
        int partID = 0;
        if (!this->argValue<int>("PartID", partID)) return false;
        //获取渲染对象操作器
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper == nullptr) return false;
        graphOper->updateGraph(partID, true);
        //获取part树操作器
        EventOper::TreeEventOperator* treeOper = FITKOPERREPO->getOperatorT<EventOper::TreeEventOperator>("PartTreeEvent");
        if (treeOper == nullptr) return false;
        treeOper->updateTree();
        return false;
    }

    bool OperatorPart::partCreateOper()
    {
        return false;
    }

    bool OperatorPart::partRenameOper()
    {
        // 获取部件ID
        int partID = 0;
        if (!this->argValue<int>("PartID", partID)) return false;
        // 获取 Radioss Case
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return false;
        // 获取网格模型
        Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
        if (meshModel == nullptr) return false;
        // 获取部件管理器
        Radioss::FITKRadiossPartManager* partsManager = meshModel->getPartsManager();
        if (partsManager == nullptr) return false;
        Radioss::FITKRadiossPart* partData = partsManager->getDataByID(partID);
        if (!partData) return false;
        GUI::GUIRenameDialog renameDialog(this, partData, partsManager, FITKAPP->getGlobalData()->getMainWindow());
        renameDialog.exec();
        return true;
    }

    bool OperatorPart::partDeleteOper()
    {
        // 获取部件ID
        int partID = 0;
        if (!this->argValue<int>("PartID", partID)) return false;
        // 获取 Radioss Case
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return false;
        // 获取网格模型
        Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
        if (meshModel == nullptr) return false;
        // 获取部件管理器
        Radioss::FITKRadiossPartManager* partsManager = meshModel->getPartsManager();
        if (partsManager == nullptr) return false;
        // 移除部件
        partsManager->removeDataByID(partID);
        return true;
    }

}
