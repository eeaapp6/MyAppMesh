/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPropertyBeamDialog.h"
#include "ui_GUIPropertyDialog.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeam.h"

#include <QComboBox>
#include <QMessageBox>
#include <QCheckBox>

namespace GUI
{
    GUIPropertyBeamDialog::GUIPropertyBeamDialog(Core::FITKActionOperator * oper, Radioss::FITKPropAbstract * obj, QWidget * parent)
        : GUIPropertyDialog(oper, obj, parent)
    {
        _obj = dynamic_cast<Radioss::FITKPropBeam*>(obj);
        if (_obj)
            _isEditMode = true;
        else
            _obj = new Radioss::FITKPropBeam();

        // 初始化
        this->initUI();
        this->init();
    }

    void GUIPropertyBeamDialog::initUI()
    {
        // 清空
        _ui->tableWidgetValue->clear();
        _ui->tableWidgetValue->setRowCount(0);
        // 设置列数
        _ui->tableWidgetValue->setColumnCount(2);
        _ui->tableWidgetValue->setHorizontalHeaderLabels(QStringList{ "Name","Value" });

        if (!_isEditMode) {
            QString name;
            _oper->argValue("AnalysisPropertyName", name);
            _ui->lineEditName->setText(name);
        }
        _ui->lineEditPropertyID->setReadOnly(true);
        _ui->lineEditType->setText("BEAM");
        _ui->lineEditType->setReadOnly(true);
        //面积
        this->addLineEdit("Area", "Area_LineEdit");
        //Ismstr
        QComboBox* comBobox = addComboBox("Small_Strain_Option", "Small_Strain_Option_Box");
        comBobox->addItem("Default", 0);
        comBobox->addItem("Small strain formulation from t = 0", 1);
        //comBobox->addItem("Set to 4", 2);
        //comBobox->addItem("Set to 4", 3);
        comBobox->addItem("Full geometric nonlinearities", 4);
        //dm
        this->addLineEdit("Membrane_Damping", "Membrane_Damping_LineEdit");
        //df
        this->addLineEdit("Flexural_Damping", "Flexural_Damping_LineEdit");
        //Iyy
        this->addLineEdit("Moment_of_Inertia_Bending_Y", "Moment_of_Inertia_Bending_Y_LineEdit");
        //Izz
        this->addLineEdit("Moment_of_Inertia_Bending_Z", "Moment_of_Inertia_Bending_Z_LineEdit");
        //Ixx
        this->addLineEdit("Moment_of_Inertia_Torsion", "Moment_of_Inertia_Torsion_LineEdit");
        //节点1旋转自由度
        int row = _ui->tableWidgetValue->rowCount();
        _ui->tableWidgetValue->insertRow(row);
        QTableWidgetItem* item = new QTableWidgetItem("Rotation_DOF_Node1");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        _ui->tableWidgetValue->setItem(row, 0, item);
        QWidget* widgetNode1 = new QWidget(this);
        QHBoxLayout* layout1 = new QHBoxLayout(widgetNode1);
        QCheckBox* checkNode1_x = new QCheckBox("X", this);
        checkNode1_x->setObjectName("Rotation_DOF_Node1_X_CheckBox");
        QCheckBox* checkNode1_y = new QCheckBox("Y", this);
        checkNode1_y->setObjectName("Rotation_DOF_Node1_Y_CheckBox");
        QCheckBox* checkNode1_z = new QCheckBox("Z", this);
        checkNode1_z->setObjectName("Rotation_DOF_Node1_Z_CheckBox");
        layout1->addWidget(checkNode1_x);
        layout1->addWidget(checkNode1_y);
        layout1->addWidget(checkNode1_z);
        layout1->setContentsMargins(9, 0, 0, 0);
        layout1->setSpacing(0);
        _ui->tableWidgetValue->setCellWidget(row, 1, widgetNode1);
        //节点2旋转自由度
        row = _ui->tableWidgetValue->rowCount();
        _ui->tableWidgetValue->insertRow(row);
        item = new QTableWidgetItem("Rotation_DOF_Node2");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        _ui->tableWidgetValue->setItem(row, 0, item);
        QWidget* widgetNode2 = new QWidget(this);
        QHBoxLayout* layout2 = new QHBoxLayout(widgetNode2);
        QCheckBox* checkNode2_x = new QCheckBox("X", this);
        checkNode2_x->setObjectName("Rotation_DOF_Node2_X_CheckBox");
        QCheckBox* checkNode2_y = new QCheckBox("Y", this);
        checkNode2_y->setObjectName("Rotation_DOF_Node2_Y_CheckBox");
        QCheckBox* checkNode2_z = new QCheckBox("Z", this);
        checkNode2_z->setObjectName("Rotation_DOF_Node2_Z_CheckBox");
        layout2->addWidget(checkNode2_x);
        layout2->addWidget(checkNode2_y);
        layout2->addWidget(checkNode2_z);
        layout2->setContentsMargins(9, 0, 0, 0);
        layout2->setSpacing(0);
        _ui->tableWidgetValue->setCellWidget(row, 1, widgetNode2);
        //Ishear
        comBobox = addComboBox("Beam_Formulation", "Beam_Formulation_Box");
        comBobox->addItem(tr("Include Shear"), 0);
        comBobox->addItem(tr("Exclude Shear"), 1);
        // 调整宽度
        _ui->tableWidgetValue->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    }

    void GUIPropertyBeamDialog::init()
    {
        Radioss::FITKPropBeam* obj = dynamic_cast<Radioss::FITKPropBeam*>(_obj);
        if (!obj)return;
        Radioss::FITKPropValueBeam* value = obj->getValue();
        if (!value)return;
        if (_isEditMode) {
            _ui->lineEditName->setText(obj->getDataObjectName());
        }
        _ui->lineEditPropertyID->setText(obj->GetFITKPropBeamRadiossKeyWord());

        //面积
        QLineEdit* edit = this->findChild<QLineEdit*>("Area_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getCrossSection()));
        //Ismstr
        QComboBox* comBobox = this->findChild<QComboBox*>("Small_Strain_Option_Box");
        if (!comBobox)return;
        int index = comBobox->findData(value->getSmallStrainOption());
        comBobox->setCurrentIndex(index);
        //dm
        edit = this->findChild<QLineEdit*>("Membrane_Damping_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getBeamMembraneDamping()));
        //df
        edit = this->findChild<QLineEdit*>("Flexural_Damping_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getBeamFlexuralDamping()));
        //Iyy
        edit = this->findChild<QLineEdit*>("Moment_of_Inertia_Bending_Y_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getAreaMomentInertiaIyy()));
        //Izz
        edit = this->findChild<QLineEdit*>("Moment_of_Inertia_Bending_Z_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getAreaMomentInertiaIzz()));
        //Ixx
        edit = this->findChild<QLineEdit*>("Moment_of_Inertia_Torsion_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getAreaMomentInertiaIxx()));
        //节点1旋转自由度
        QCheckBox* checkNode1_x = this->findChild<QCheckBox*>("Rotation_DOF_Node1_X_CheckBox");
        checkNode1_x->setChecked(value->isRotationDOF1(0));
        QCheckBox* checkNode1_y = this->findChild<QCheckBox*>("Rotation_DOF_Node1_Y_CheckBox");
        checkNode1_y->setChecked(value->isRotationDOF1(1));
        QCheckBox* checkNode1_z = this->findChild<QCheckBox*>("Rotation_DOF_Node1_Z_CheckBox");
        checkNode1_z->setChecked(value->isRotationDOF1(2));
        //节点2旋转自由度
        QCheckBox* checkNode2_x = this->findChild<QCheckBox*>("Rotation_DOF_Node2_X_CheckBox");
        checkNode2_x->setChecked(value->isRotationDOF2(0));
        QCheckBox* checkNode2_y = this->findChild<QCheckBox*>("Rotation_DOF_Node2_Y_CheckBox");
        checkNode2_y->setChecked(value->isRotationDOF2(1));
        QCheckBox* checkNode2_z = this->findChild<QCheckBox*>("Rotation_DOF_Node2_Z_CheckBox");
        checkNode2_z->setChecked(value->isRotationDOF2(2));
        //Ishear
        comBobox = this->findChild<QComboBox*>("Beam_Formulation_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->isNeglectsShear() ? 1 : 0);
        comBobox->setCurrentIndex(index);

    }

    bool GUIPropertyBeamDialog::ckeckData()
    {
        //获取名称，查重处理
        auto mgr = this->getSectionManager();
        if (!mgr)return false;
        QString name = _ui->lineEditName->text();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "", tr("Name cannot be empty."), QMessageBox::Ok);
            return false;
        }
        else if (mgr->getDataByName(name) && name != _obj->getDataObjectName()) {
            QMessageBox::warning(this, "", tr("\"%1\" The name already exists").arg(name), QMessageBox::Ok);
            return false;
        }
        return true;
    }

    void GUIPropertyBeamDialog::getDataFormWidget()
    {
        Radioss::FITKPropBeam* obj = dynamic_cast<Radioss::FITKPropBeam*>(_obj);
        if (!obj)return;
        Radioss::FITKPropValueBeam* value = obj->getValue();
        if (!value)return;
        //面积
        QLineEdit* edit = this->findChild<QLineEdit*>("Area_LineEdit");
        if (!edit)return;
        value->setCrossSection(edit->text().toDouble());
        //Ismstr  
        QComboBox* comBobox = this->findChild<QComboBox*>("Small_Strain_Option_Box");
        if (!comBobox)return;
        value->setSmallStrainOption(comBobox->currentData().toInt());
        //dm
        edit = this->findChild<QLineEdit*>("Membrane_Damping_LineEdit");
        if (!edit)return;
        value->setBeamMembraneDamping(edit->text().toDouble());
        //df
        edit = this->findChild<QLineEdit*>("Flexural_Damping_LineEdit");
        if (!edit)return;
        value->setBeamFlexuralDamping(edit->text().toDouble());
        //Iyy
        edit = this->findChild<QLineEdit*>("Moment_of_Inertia_Bending_Y_LineEdit");
        if (!edit)return;
        value->setAreaMomentInertiaIyy(edit->text().toDouble());
        //Izz
        edit = this->findChild<QLineEdit*>("Moment_of_Inertia_Bending_Z_LineEdit");
        if (!edit)return;
        value->setAreaMomentInertiaIzz(edit->text().toDouble());
        //Ixx
        edit = this->findChild<QLineEdit*>("Moment_of_Inertia_Torsion_LineEdit");
        if (!edit)return;
        value->setAreaMomentInertiaIxx(edit->text().toDouble());
        //节点1旋转自由度
        QCheckBox* checkNode1_x = this->findChild<QCheckBox*>("Rotation_DOF_Node1_X_CheckBox");
        value->setRotationDOF1(0, checkNode1_x->isChecked());
        QCheckBox* checkNode1_y = this->findChild<QCheckBox*>("Rotation_DOF_Node1_Y_CheckBox");
        value->setRotationDOF1(1, checkNode1_y->isChecked());
        QCheckBox* checkNode1_z = this->findChild<QCheckBox*>("Rotation_DOF_Node1_Z_CheckBox");
        value->setRotationDOF1(2, checkNode1_z->isChecked());
        //节点2旋转自由度
        QCheckBox* checkNode2_x = this->findChild<QCheckBox*>("Rotation_DOF_Node2_X_CheckBox");
        value->setRotationDOF2(0, checkNode2_x->isChecked());
        QCheckBox* checkNode2_y = this->findChild<QCheckBox*>("Rotation_DOF_Node2_Y_CheckBox");
        value->setRotationDOF2(1, checkNode2_y->isChecked());
        QCheckBox* checkNode2_z = this->findChild<QCheckBox*>("Rotation_DOF_Node2_Z_CheckBox");
        value->setRotationDOF2(2, checkNode2_z->isChecked());
        //Ishear
        comBobox = this->findChild<QComboBox*>("Beam_Formulation_Box");
        if (!comBobox)return;
        value->setNeglectsShear(comBobox->currentData().toInt());
    }

    void GUIPropertyBeamDialog::writePythonScript()
    {
        Radioss::FITKPropBeam* obj = dynamic_cast<Radioss::FITKPropBeam*>(_obj);
        if (obj == nullptr)
        {
            return;
        }

        Radioss::FITKPropValueBeam* value = obj->getValue();
        if (value == nullptr)
        {
            return;
        }

        QStringList script;
        if (_isEditMode)
        {
            QString oldName = _oldNameForScript.isEmpty() ? obj->getDataObjectName() : _oldNameForScript;
            script.append(QString("p=Property.Beam.GetBeam('%1')").arg(oldName));
            if (oldName != obj->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(obj->getDataObjectName()));
            }
        }
        else
        {
            script.append("p=Property.Beam()");
            script.append(QString("p.createProperty('%1')").arg(obj->getDataObjectName()));
        }

        script.append(QString("p.setCrossSection(%1)").arg(value->getCrossSection()));
        script.append(QString("p.setSmallStrainOption(%1)").arg(value->getSmallStrainOption()));
        script.append(QString("p.setBeamMembraneDamping(%1)").arg(value->getBeamMembraneDamping()));
        script.append(QString("p.setBeamFlexuralDamping(%1)").arg(value->getBeamFlexuralDamping()));
        script.append(QString("p.setAreaMomentInertiaIyy(%1)").arg(value->getAreaMomentInertiaIyy()));
        script.append(QString("p.setAreaMomentInertiaIzz(%1)").arg(value->getAreaMomentInertiaIzz()));
        script.append(QString("p.setAreaMomentInertiaIxx(%1)").arg(value->getAreaMomentInertiaIxx()));
        script.append(QString("p.setRotationDOF1('x', %1)").arg(value->isRotationDOF1(0) ? 1 : 0));
        script.append(QString("p.setRotationDOF1('y', %1)").arg(value->isRotationDOF1(1) ? 1 : 0));
        script.append(QString("p.setRotationDOF1('z', %1)").arg(value->isRotationDOF1(2) ? 1 : 0));
        script.append(QString("p.setRotationDOF2('x', %1)").arg(value->isRotationDOF2(0) ? 1 : 0));
        script.append(QString("p.setRotationDOF2('y', %1)").arg(value->isRotationDOF2(1) ? 1 : 0));
        script.append(QString("p.setRotationDOF2('z', %1)").arg(value->isRotationDOF2(2) ? 1 : 0));
        script.append(QString("p.setNeglectsShear(%1)").arg(value->isNeglectsShear() ? 1 : 0));

        this->saveScript(script);
    }
}