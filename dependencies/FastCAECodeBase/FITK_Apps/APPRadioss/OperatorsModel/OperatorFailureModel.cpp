/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorFailureModel.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"

#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"

#include "GUIDialog/GUIAnalysisDialog/GUIFailureModelTypeDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIFailureBIQUAD_Dialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIFailureJOHNSON_Dialog.h"

#include "GUIDialog/GUIToolDialog/GUIRenameDialog.h"
#include "OperatorsInterface/TreeEventOperator.h"

namespace OperModel
{
    bool OperatorFailureModel::execGUI()
    {
        if (_emitter == nullptr)
            return false;
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        if (!mw) return false;
        QString name = _emitter->objectName();
        if (name.isEmpty())return false;
        if (name == "actionFailureModelCreate")
        {
            GUI::GUIFailureModelTypeDialog dialog;
            if (dialog.exec() != QDialog::Accepted)return false;
            int type = dialog.selectedFailureModelType();
            this->setArgs("FailureModelName", dialog.getFailureModelName());
            switch (type)
            {
            case Radioss::FITKAbstractFailureModel::FailureModelType::BIQUAD:{
                GUI::GUIFailureBIQUAD_Dialog* dialog = new GUI::GUIFailureBIQUAD_Dialog(this,nullptr, mw);
                dialog->show();
                break;
            }
            case Radioss::FITKAbstractFailureModel::FailureModelType::JOHNSON: {
                GUI::GUIFailureJOHNSON_Dialog* dialog = new GUI::GUIFailureJOHNSON_Dialog(this, nullptr, mw);
                dialog->show();
                break;
            }
            default:
                break;
            }
        }
        else if (name == "actionFailureModelEdit")
        {
            Radioss::FITKAbstractFailureModel* selectedFail = getSelectedFailureModel();
            if (!selectedFail) return false;
            switch (selectedFail->getFailureModelType())
            {
            case Radioss::FITKAbstractFailureModel::FailureModelType::BIQUAD: {
                GUI::GUIFailureBIQUAD_Dialog* dialog = new GUI::GUIFailureBIQUAD_Dialog(this, selectedFail, mw);
                dialog->show();
                break;
            }
            case Radioss::FITKAbstractFailureModel::FailureModelType::JOHNSON: {
                GUI::GUIFailureJOHNSON_Dialog* dialog = new GUI::GUIFailureJOHNSON_Dialog(this, selectedFail, mw);
                dialog->show();
                break;
            }
            default:
                break;
            }
        }
        return false;
    }
    bool OperatorFailureModel::execProfession()
    {
        // 通过树形菜单事件处理器刷新树
        EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator("TreePropertyEvent"));
        if (oper != nullptr) oper->updateTree();

        return false;
    }
    Radioss::FITKAbstractFailureModel * OperatorFailureModel::getSelectedFailureModel()
    {
        // 从管理器中查找对应
        int eosID = -1;
        this->argValue("SelectedItemDataID", eosID);
        Radioss::FITKRadiossFailureModelManager* manager = getFailureModelManager();
        if (!manager) return nullptr;
        Radioss::FITKAbstractFailureModel* data = manager->getDataByID(eosID);
        if (!data) return nullptr;
        return data;
    }
    Radioss::FITKRadiossFailureModelManager * OperatorFailureModel::getFailureModelManager()
    {
        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return nullptr;
        // 获取状态方程管理
        return dataCase->getFailureModelManager();
    }

}