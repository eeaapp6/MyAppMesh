/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIMaterialTypeDialog.h"
#include "ui_GUIMaterialTypeDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasJohns.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasTab.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialElastic.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasBrit.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialHydro.h"

namespace GUI
{
    GUIMaterialTypeDialog::GUIMaterialTypeDialog(QWidget* parent): QDialog(parent)
        , _ui(new Ui::GUIMaterialTypeDialog)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸

        _ui->comboBox_type->addItem(tr("PLAS_JOHNS"),0/* MaterialType::Johnson */);
        _ui->comboBox_type->addItem(tr("ELAST"),1/* Elastic */);
        _ui->comboBox_type->addItem(tr("PLAS_TAB"),2/* PlasTab */);
        _ui->comboBox_type->addItem(tr("PLAS_BRIT"), 3/* PlasBrit */);
        _ui->comboBox_type->addItem(tr("HYDRO"), 4/* Hydro */);

        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        auto matMgr = caseObj->getMaterialManager();
        if (!matMgr)return;
        _ui->lineEdit_name->setText(matMgr->checkName("Material-1"));
        _ui->lineEdit->setReadOnly(true);
    }

    GUIMaterialTypeDialog::~GUIMaterialTypeDialog()
    {
        if (_ui)
            delete _ui;
        _ui = nullptr;
    }

    int GUIMaterialTypeDialog::selectedMaterialType() const
    {
        return _ui->comboBox_type->currentData().toInt();
    }

    void GUIMaterialTypeDialog::on_pushButtonOK_clicked()
    {
        this->accept();
    }

    void GUIMaterialTypeDialog::on_pushButtonCancel_clicked()
    {
        this->reject();
    }

    void GUIMaterialTypeDialog::on_comboBox_type_currentIndexChanged(int index)
    {
        if (index == 0)
            _ui->lineEdit->setText(Radioss::FITKMaterialPlasJohns::GetFITKMaterialPlasJohnsRadiossKeyWord());
        else if(index == 1)
            _ui->lineEdit->setText(Radioss::FITKMaterialElastic::GetFITKMaterialElasticRadiossKeyWord());
        else if(index == 2)
            _ui->lineEdit->setText(Radioss::FITKMaterialPlasTab::GetFITKMaterialPlasTabRadiossKeyWord());
        else if(index == 3)
            _ui->lineEdit->setText(Radioss::FITKMaterialPlasBrit::GetFITKMaterialPlasBritRadiossKeyWord());
        else if (index == 4)
            _ui->lineEdit->setText(Radioss::FITKMaterialHydro::GetFITKMaterialHydroRadiossKeyWord());
    }
    QString GUIMaterialTypeDialog::getMaterialName()
    {
        return _ui->lineEdit_name->text();
    }
} // namespace GUI
