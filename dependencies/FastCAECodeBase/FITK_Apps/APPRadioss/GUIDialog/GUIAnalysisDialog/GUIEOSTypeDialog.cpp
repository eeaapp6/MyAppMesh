/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIEOSTypeDialog.h"
#include "ui_GUIEOSTypeDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"

namespace GUI
{
    GUIEOSTypeDialog::GUIEOSTypeDialog(QWidget* parent) : QDialog(parent)
        , _ui(new Ui::GUIEOSTypeDialog)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸

        _ui->comboBox_type->addItem(tr("Polynomial"), Radioss::FITKAbstractEquationOfState::Polynomial);

        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        auto matMgr = caseObj->getEquationOfState();
        if (!matMgr)return;
        _ui->lineEdit_name->setText(matMgr->checkName("EOS-1"));
    }
    GUI::GUIEOSTypeDialog::~GUIEOSTypeDialog()
    {
        if (_ui)
            delete _ui;
        _ui = nullptr;
    }
    int GUIEOSTypeDialog::selectedEOSType() const
    {
        return _ui->comboBox_type->currentData().toInt();
    }
    QString GUIEOSTypeDialog::getEOSName()
    {
        return _ui->lineEdit_name->text();
    }
    void GUIEOSTypeDialog::on_pushButtonOK_clicked()
    {
        this->accept();
    }
    void GUIEOSTypeDialog::on_pushButtonCancel_clicked()
    {
        this->reject();
    }
}