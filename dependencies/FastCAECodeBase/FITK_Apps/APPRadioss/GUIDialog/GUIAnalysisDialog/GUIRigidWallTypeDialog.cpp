/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIRigidWallTypeDialog.h"
#include "ui_GUIRigidWallTypeDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"
#include <QMessageBox>

namespace GUI
{
    GUIRigidWallTypeDialog::GUIRigidWallTypeDialog(QWidget* parent): QDialog(parent)
        , _ui(new Ui::GUIRigidWallTypeDialog)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸

        _ui->comboBox_type->addItem(tr("PLANE"),0/*  平面壁 */);
        _ui->comboBox_type->addItem(tr("CYLINDER"),1/*  圆柱壁 */);
        _ui->comboBox_type->addItem(tr("SPHER"),2/*  球面壁 */);
        _ui->comboBox_type->addItem(tr("PARAL"),3/*  平行平面 */);
        // 设置第三，第四项为不可选中
        _ui->comboBox_type->setItemData(2, false, Qt::UserRole - 1);
        _ui->comboBox_type->setItemData(3, false, Qt::UserRole - 1);

        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        auto rWallMgr = caseObj->getRWallManager();
        if (!rWallMgr)return;
        _ui->lineEdit_name->setText(rWallMgr->checkName("RigidWall-1"));
        _ui->lineEdit->setReadOnly(true);
    }

    GUIRigidWallTypeDialog::~GUIRigidWallTypeDialog()
    {
        if (_ui)
            delete _ui;
        _ui = nullptr;
    }

    int GUIRigidWallTypeDialog::selectedRigidWallType() const
    {
        return _ui->comboBox_type->currentData().toInt();
    }

    void GUIRigidWallTypeDialog::on_pushButtonOK_clicked()
    {
        //获取名称，查重处理
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        auto rWallMgr = caseObj->getRWallManager();
        if (!rWallMgr)return;
        QString name = _ui->lineEdit_name->text();
        if (name.isEmpty())
        {
            QMessageBox::warning(this, "", tr("Name cannot be empty."), QMessageBox::Ok);
            return;
        }
        else if (rWallMgr->getDataByName(name)!=nullptr) 
        {
            QMessageBox::warning(this, "", tr("\"%1\" The name already exists").arg(name), QMessageBox::Ok);
            return;
        }
        this->accept();
    }

    void GUIRigidWallTypeDialog::on_pushButtonCancel_clicked()
    {
        this->reject();
    }

    void GUIRigidWallTypeDialog::on_comboBox_type_currentIndexChanged(int index)
    {
        if (index == 0)
            _ui->lineEdit->setText(Radioss::FITKRWallPlane::GetFITKRWallPlaneRadiossKeyWord());
        else if(index == 1)
            _ui->lineEdit->setText(Radioss::FITKRWallCylinder::GetFITKRWallCylinderRadiossKeyWord());
        else if (index == 2)
            _ui->lineEdit->setText("");
        else
            _ui->lineEdit->setText("");
    }
    QString GUIRigidWallTypeDialog::getRigidWallName()
    {
        return _ui->lineEdit_name->text();
    }
} // namespace GUI
