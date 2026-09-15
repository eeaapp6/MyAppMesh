/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPropertyTypeDialog.h"
#include "ui_GUIPropertyTypeDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKPropAbstract.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include <QMessageBox>

namespace GUI
{
    GUIPropertyTypeDialog::GUIPropertyTypeDialog(QWidget* parent) : QDialog(parent)
        , _ui(new Ui::GUIPropertyTypeDialog)
    {
        _ui->setupUi(this);
        _ui->comboBox_type->addItem(tr("SOLID"), Radioss::FITKPropAbstract::RadPropType::Solid);
        _ui->comboBox_type->addItem(tr("SHELL"), Radioss::FITKPropAbstract::RadPropType::Shell);
        _ui->comboBox_type->addItem(tr("BEAM"), Radioss::FITKPropAbstract::RadPropType::Beam);
        _ui->comboBox_type->addItem(tr("SH_SANDW"), Radioss::FITKPropAbstract::RadPropType::Sandwich);
        _ui->comboBox_type->addItem(tr("SPR_BEAM"), Radioss::FITKPropAbstract::RadPropType::BeamSPR);
        _ui->comboBox_type->addItem(tr("SPH"), Radioss::FITKPropAbstract::RadPropType::SPH);
        _ui->lineEdit->setReadOnly(true);
        _ui->lineEdit->setText("PROP/TYPE14");
        //获取截面管理器
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        auto matMgr = caseObj->getSectionManager();
        if (!matMgr)return;
        _ui->lineEditName->setText(matMgr->checkName("Property-1"));
    }

    GUIPropertyTypeDialog::~GUIPropertyTypeDialog()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
    }

    int GUIPropertyTypeDialog::getPropType()
    {
        return _ui->comboBox_type->currentData().toInt();
    }

    QString GUIPropertyTypeDialog::getName()
    {
        return _ui->lineEditName->text();
    }

    void GUIPropertyTypeDialog::on_pushButtonOK_clicked()
    {
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        //获取截面管理器
        auto matMgr = caseObj->getSectionManager();
        if (!matMgr)return;
        //获取名称，查重处理
        QString name = _ui->lineEditName->text();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "", tr("Name cannot be empty."), QMessageBox::Ok);
            return ;
        }
        else if (matMgr->getDataByName(name)) {
            QMessageBox::warning(this, "", tr("\"%1\" The name already exists").arg(name), QMessageBox::Ok);
            return ;
        }
        this->accept();
    }

    void GUIPropertyTypeDialog::on_pushButtonCancel_clicked()
    {
        this->reject();
    }

    void GUIPropertyTypeDialog::on_comboBox_type_currentIndexChanged(int index)
    {
        int type = _ui->comboBox_type->currentData().toInt();

        switch (type)
        {
        case Radioss::FITKPropAbstract::RadPropType::Solid: {
            _ui->lineEdit->setText("PROP/TYPE14");
            break;
        }
        case Radioss::FITKPropAbstract::RadPropType::Shell: {
            _ui->lineEdit->setText("PROP/TYPE1");
            break;
        }
        case Radioss::FITKPropAbstract::RadPropType::Beam: {
            _ui->lineEdit->setText("PROP/TYPE3");
            break;
        }
        case Radioss::FITKPropAbstract::RadPropType::Sandwich: {
            _ui->lineEdit->setText("PROP/TYPE11");
            break;
        }
        case Radioss::FITKPropAbstract::RadPropType::BeamSPR: {
            _ui->lineEdit->setText("PROP/TYPE13");
            break;
        }
        case Radioss::FITKPropAbstract::RadPropType::SPH: {
            _ui->lineEdit->setText("PROP/TYPE34");
            break;
        }
        }
    }
}
