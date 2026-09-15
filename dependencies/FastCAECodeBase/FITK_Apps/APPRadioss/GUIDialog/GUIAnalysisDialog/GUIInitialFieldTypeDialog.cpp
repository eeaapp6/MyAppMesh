/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIInitialFieldTypeDialog.h"
#include "ui_GUIInitialFieldTypeDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"
#include <QStandardItemModel>
#include <QMessageBox>

namespace GUI
{
    GUIInitialFieldTypeDialog::GUIInitialFieldTypeDialog(QWidget* parent): QDialog(parent)
    {
        _ui = new Ui::GUIInitialFieldTypeDialog;
        _ui->setupUi(this);

        // 初始化对话框
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸

        _ui->comboBoxType->addItem(tr("TRA"), Radioss::FITKAbstractInitialField::TRA);
        //_ui->comboBoxType->addItem(tr("ROT"), Radioss::FITKAbstractInitialField::ROT);
        //_ui->comboBoxType->addItem(tr("TG"), Radioss::FITKAbstractInitialField::TG);
        //_ui->comboBoxType->addItem(tr("GRID"), Radioss::FITKAbstractInitialField::GRID);
        _ui->comboBoxType->addItem(tr("AXIS"), Radioss::FITKAbstractInitialField::AXIS);

        // 获取管理器
        Radioss::FITKInitialFieldManager* manager = getManagerInitialField();
        if (manager == nullptr) return;
        
        // 设置名称
        _ui->lineEditName->setText(manager->checkName("InitialField-1"));
        // 设置关键字
        _ui->lineEditKeyWord->setText(Radioss::FITKInitialFieldTra::GetFITKInitialFieldTraRadiossKeyWord());
        _ui->lineEditKeyWord->setReadOnly(true);
    }

    GUIInitialFieldTypeDialog::~GUIInitialFieldTypeDialog()
    {
        if (_ui != nullptr) delete _ui;
        _ui = nullptr;
    }

    QString GUIInitialFieldTypeDialog::getDataName()
    {
        return _ui->lineEditName->text();
    }

    int GUIInitialFieldTypeDialog::getDataType()
    {
        return _ui->comboBoxType->currentData().toInt();
    }

    QString GUIInitialFieldTypeDialog::getDataKeyWord()
    {
        return _ui->lineEditKeyWord->text();
    }

    void GUIInitialFieldTypeDialog::on_buttonOK_clicked()
    {
        // 获取管理器
        Radioss::FITKInitialFieldManager* manager = getManagerInitialField();
        if (manager == nullptr) return;

        // 获取名称
        QString name = _ui->lineEditName->text();
        if (name.isEmpty())
        {
            QMessageBox::warning(this, "Warning", tr("Name cannot be empty."), QMessageBox::Ok);
            return;
        }
        else if (manager->getDataByName(name) != nullptr)
        {
            QMessageBox::warning(this, "Warning", tr("\"%1\" The name already exists").arg(name), QMessageBox::Ok);
            return;
        }
        else this->accept();
    }

    void GUIInitialFieldTypeDialog::on_buttonCancel_clicked()
    {
        this->reject();
    }

    void GUIInitialFieldTypeDialog::on_comboBoxType_currentIndexChanged(int index)
    {
        Q_UNUSED(index);
        Radioss::FITKAbstractInitialField::InitialFieldType type = Radioss::FITKAbstractInitialField::InitialFieldType(_ui->comboBoxType->currentData().toInt());
        switch (type)
        {
        case Radioss::FITKAbstractInitialField::TRA: _ui->lineEditKeyWord->setText(Radioss::FITKInitialFieldTra::GetFITKInitialFieldTraRadiossKeyWord());
            break;
        case Radioss::FITKAbstractInitialField::ROT:
            break;
        case Radioss::FITKAbstractInitialField::TG:
            break;
        case Radioss::FITKAbstractInitialField::GRID:
            break;
        case Radioss::FITKAbstractInitialField::AXIS: _ui->lineEditKeyWord->setText(Radioss::FITKInitialFieldAxis::GetFITKInitialFieldAxisRadiossKeyWord());
            break;
        default:
            break;
        }
    }

    Radioss::FITKInitialFieldManager* GUIInitialFieldTypeDialog::getManagerInitialField()
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        return dataCase == nullptr ? nullptr : dataCase->getInitialFieldManager();
    }
}