/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIContactCreateDialog.h"
#include "ui_GUIContactCreateDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionMultiUsageImpact.h"

namespace GUI
{
    GUIContactCreateDialog::GUIContactCreateDialog(QWidget* parent) : QDialog(parent)
        , _ui(new Ui::GUIContactCreateDialog)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸

        _ui->comboBox_type->addItem(Radioss::FITKInteractionKinematicTied::GetFITKInteractionKinematicTiedRadiossKeyWord(),
            Radioss::FITKAbstractInteraction::InteractionType::IT_KinematicTied);
        _ui->comboBox_type->addItem(Radioss::FITKInteractionMultiUsageImpact::GetFITKInteractionMultiUsageImpactRadiossKeyWord(), 
            Radioss::FITKAbstractInteraction::InteractionType::IT_MultiUsageImpact);

        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKRadiossSolution* solutionManager = caseObj->getCurrentSolution();
        if (solutionManager == nullptr) return;
        auto mgr = solutionManager->getInteractionManager();
        _ui->lineEdit_name->setText(mgr->checkName("Contact-1"));
    }

    GUI::GUIContactCreateDialog::~GUIContactCreateDialog()
    {
        if (_ui)
            delete _ui;
        _ui = nullptr;
    }

    int GUIContactCreateDialog::selectedContactType() const
    {
        return _ui->comboBox_type->currentData().toInt();
    }

    QString GUIContactCreateDialog::getContactName()
    {
        return _ui->lineEdit_name->text();
    }

    void GUIContactCreateDialog::on_pushButton_OK_clicked()
    {
        this->accept();
    }

    void GUIContactCreateDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

}