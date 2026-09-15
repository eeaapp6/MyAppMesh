/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorContact.h"
#include "GUIDialog/GUIAnalysisDialog/GUIContactCreateDialog.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"

namespace OperModel
{
    bool OperatorContact::execGUI()
    {
        GUI::GUIContactCreateDialog dialog;
        if (dialog.exec() != QDialog::Accepted)
            return false;
        int type = dialog.selectedContactType();
        QString name = dialog.getContactName();
        switch (type)
        {
        case Radioss::FITKAbstractInteraction::InteractionType::IT_KinematicTied: 
        {
            Core::FITKActionOperator* actionOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionContactTieCreate");
            if (actionOper == nullptr) {
                return false;
            }
            QObject* obj = new QObject();
            obj->setObjectName("actionContactTieCreate");
            actionOper->setEmitter(obj);
            actionOper->setArgs("ContactName", name);
            if (actionOper) {
                actionOper->actionTriggered();
            }
            break;
        }
        case Radioss::FITKAbstractInteraction::InteractionType::IT_MultiUsageImpact: 
        {
            Core::FITKActionOperator* actionOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionContactMultiUsageImpactCreate");
            if (actionOper == nullptr) {
                return false;
            }
            QObject* obj = new QObject();
            obj->setObjectName("actionContactMultiUsageImpactCreate");
            actionOper->setEmitter(obj);
            actionOper->setArgs("ContactName", name);
            if (actionOper) {
                actionOper->actionTriggered();
            }
            break;
        }
        default:
            break;
        }
        return false;
    }

    bool OperatorContact::execProfession()
    {
        return false;
    }
}


