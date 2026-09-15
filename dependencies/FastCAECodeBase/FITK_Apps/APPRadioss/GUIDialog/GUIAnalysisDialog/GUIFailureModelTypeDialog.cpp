/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIFailureModelTypeDialog.h"
#include "ui_GUIFailureModelTypeDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

namespace GUI
{
    GUIFailureModelTypeDialog::GUIFailureModelTypeDialog(QWidget* parent) : QDialog(parent)
        , _ui(new Ui::GUIFailureModelTypeDialog)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸

        bool ok = false;
        for (int i = Radioss::FITKAbstractFailureModel::FailureModelType::ALTER;
            i< Radioss::FITKAbstractFailureModel::FailureModelType::WILKINS; ++i )
        {
            Radioss::FITKAbstractFailureModel::FailureModelType type 
                = (Radioss::FITKAbstractFailureModel::FailureModelType)i;
            Core::FITKEnumTransfer<Radioss::FITKAbstractFailureModel::FailureModelType> ft;
            QString str = ft.toString(type, ok);
            if (!ok)continue;
            _ui->comboBox_type->addItem(str, type);
        }
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKRadiossFailureModelManager* matMgr = caseObj->getFailureModelManager();
        if (!matMgr)return;
        _ui->lineEdit_name->setText(matMgr->checkName("FailureModel-1"));
    }
    GUI::GUIFailureModelTypeDialog::~GUIFailureModelTypeDialog()
    {
        if (_ui)
            delete _ui;
        _ui = nullptr;
    }
    int GUIFailureModelTypeDialog::selectedFailureModelType() const
    {
        return _ui->comboBox_type->currentData().toInt();
    }
    QString GUIFailureModelTypeDialog::getFailureModelName()
    {
        return _ui->lineEdit_name->text();
    }
    void GUIFailureModelTypeDialog::on_pushButtonOK_clicked()
    {
        this->accept();
    }
    void GUIFailureModelTypeDialog::on_pushButtonCancel_clicked()
    {
        this->reject();
    }
}