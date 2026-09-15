/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUICreateSolutionDialog.h"
#include "ui_GUICreateSolutionDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"

namespace GUI
{
    GUICreateSolutionDialog::GUICreateSolutionDialog(QWidget* parent) : QDialog(parent)
        , _ui(new Ui::GUICreateSolutionDialog)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸

        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        auto matMgr = caseObj->getSolutionManager();
        if (!matMgr)return;
        _ui->lineEdit_name->setText(matMgr->checkName("Solution-1"));
        _ui->lineEdit_type->setText("Radioss");
        _ui->lineEdit_type->setReadOnly(true);
    }


    GUICreateSolutionDialog::~GUICreateSolutionDialog()
    {
        if (_ui)
            delete _ui;
        _ui = nullptr;
    }

    QString GUICreateSolutionDialog::getSolutionName()
    {
        return _ui->lineEdit_name->text();
    }

    void GUICreateSolutionDialog::on_pushButtonOK_clicked()
    {
        this->accept();
    }

    void GUICreateSolutionDialog::on_pushButtonCancel_clicked()
    {
        this->reject();
    }
}