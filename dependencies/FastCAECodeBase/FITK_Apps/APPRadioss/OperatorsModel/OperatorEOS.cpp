/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorEOS.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"

#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"

#include "GUIDialog/GUIAnalysisDialog/GUIEOSTypeDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIEOSPolynomialDialog.h"

#include "GUIDialog/GUIToolDialog/GUIRenameDialog.h"
#include "OperatorsInterface/TreeEventOperator.h"

namespace OperModel
{
    bool OperModel::OperatorEOS::execGUI()
    {
        if (_emitter == nullptr)
            return false;
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        if (!mw) return false;
        QString name = _emitter->objectName();
        if (name.isEmpty())return false;
        if (name == "actionEOSCreate")
        {
            GUI::GUIEOSTypeDialog dialog;
            if (dialog.exec() != QDialog::Accepted)return false;
            int type = dialog.selectedEOSType();
            this->setArgs("EOSName", dialog.getEOSName());
            switch (type)
            {
            case Radioss::FITKAbstractEquationOfState::EOS_Type::Polynomial:{
                GUI::GUIEOSPolynomialDialog* dialog = new GUI::GUIEOSPolynomialDialog(nullptr, this, mw);
                dialog->show();
                break;
            }
            default:
                break;
            }
        }
        else if (name == "actionEOSEdit")
        {
            Radioss::FITKAbstractEquationOfState* selectedEOS = getSelectedEOS();
            if (!selectedEOS) return false;
            if (selectedEOS->getEOSType()== Radioss::FITKAbstractEquationOfState::EOS_Type::Polynomial)
            {
                GUI::GUIEOSPolynomialDialog* dialog = new GUI::GUIEOSPolynomialDialog(selectedEOS, this, mw);
                dialog->show();
            }
        }
        //else if (name == "actionEOSRename")
        //{
        //  //  return EOSRenameOper();
        //}
        //else if (name == "actionEOSDelete")
        //{
        //  //  return deleteEOS();
        //}
        return false;
    }
    bool OperatorEOS::execProfession()
    {
        // 通过树形菜单事件处理器刷新树
        EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator("TreePropertyEvent"));
        if (oper != nullptr) oper->updateTree();

        return false;
    }
    Radioss::FITKAbstractEquationOfState * OperatorEOS::getSelectedEOS()
    {
        //管理器中查找对应
        int eosID = -1;
        this->argValue("SelectedItemDataID", eosID);
        Radioss::FITKEquationOfStateManager* manager = getEOSManager();
        if (!manager) return nullptr;
        Radioss::FITKAbstractEquationOfState* eos = manager->getDataByID(eosID);
        if (!eos) return nullptr;
        return eos;
    }
    Radioss::FITKEquationOfStateManager * OperatorEOS::getEOSManager()
    {
        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return nullptr;
        // 获取状态方程管理
        return dataCase->getEquationOfState();
    }
}