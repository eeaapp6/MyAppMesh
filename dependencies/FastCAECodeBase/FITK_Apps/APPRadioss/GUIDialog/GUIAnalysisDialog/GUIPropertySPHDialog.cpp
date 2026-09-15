/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPropertySPHDialog.h"
#include "ui_GUIPropertyDialog.h"
#include "FITK_Component/FITKRadiossData/FITKPropSPH.h"

#include <QComboBox>
#include <QMessageBox>
#include <QCheckBox>

namespace GUI
{
    GUIPropertySPHDialog::GUIPropertySPHDialog(Core::FITKActionOperator * oper, Radioss::FITKPropAbstract * obj, QWidget * parent)
        : GUIPropertyDialog(oper, obj, parent)
    {
        _obj = dynamic_cast<Radioss::FITKPropSPH*>(obj);
        if (_obj)
            _isEditMode = true;
        else
            _obj = new Radioss::FITKPropSPH();

        // 初始化
        this->initUI();
        this->init();
    }
    void GUIPropertySPHDialog::initUI()
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
        _ui->lineEditType->setText("SPH");
        _ui->lineEditType->setReadOnly(true);

        //粒子质量(Mp)
        this->addLineEdit("Particle_Mass", "Particle_Mass_LineEdit");
        //Quadratic bulk viscosity (qa)   
        this->addLineEdit("Quadratic_Bulk_Viscosity", "Quadratic_Bulk_Viscosity_LineEdit");
        //Linear bulk viscosity (qb)      
        this->addLineEdit("Linear_Bulk_Viscosity", "Linear_Bulk_Viscosity_LineEdit");
        //Conservative smoothing coefficient (acs)
        this->addLineEdit("Conservative_Smoothing_Coefficient", "Conservative_Smoothing_Coefficient_LineEdit");
        //Skew identifier to define the initial orthotropic directions in the case of skew_ID
        QComboBox* comBobox = addComboBox("Skew_ID", "Skew_ID_Box");
        comBobox->addItem(tr("Global"), 0);
        //Smoothing length change based on volume(h_ID)
        comBobox = addComboBox("Smoothing_Length_Change", "Smoothing_Length_Change_Box");
        comBobox->addItem(tr("3D expansion of h"), 0);
        comBobox->addItem(tr("1D expansion of h"), 1);
        //SPH correction order (order)
        comBobox = addComboBox("SPH_Correction_Order", "SPH_Correction_Order_Box");
        comBobox->addItem(tr("No correction"), -1);
        comBobox->addItem(tr("0 correction"), 0);
        comBobox->addItem(tr("1 correction"), 1);
        //Smoothing length (h)
        this->addLineEdit("Smoothing_Length", "Smoothing_Length_LineEdit");
        //Coefficient for solving tensile instability (stab)
        this->addLineEdit("Tensile_Instability_Coefficient", "Tensile_Instability_Coefficient_LineEdit");

        //调整宽度
        _ui->tableWidgetValue->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
    void GUIPropertySPHDialog::init()
    {
        Radioss::FITKPropSPH* obj = dynamic_cast<Radioss::FITKPropSPH*>(_obj);
        if (!obj)return;
        Radioss::FITKPropSPHValue* value = obj->getValue();
        if (!value)return;
        if (_isEditMode) {
            _ui->lineEditName->setText(obj->getDataObjectName());
        }
        _ui->lineEditPropertyID->setText(obj->GetFITKPropSPHRadiossKeyWord());
        //粒子质量(Mp)
        QLineEdit* edit = this->findChild<QLineEdit*>("Particle_Mass_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getMP()));

        //Quadratic bulk viscosity (qa)   
        edit = this->findChild<QLineEdit*>("Quadratic_Bulk_Viscosity_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getQA()));

        //Linear bulk viscosity (qb)      
        edit = this->findChild<QLineEdit*>("Linear_Bulk_Viscosity_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getQB()));

        //Conservative smoothing coefficient (acs)
        edit = this->findChild<QLineEdit*>("Conservative_Smoothing_Coefficient_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getACS()));

        //Skew identifier to define the initial orthotropic directions in the case of skew_ID     
        QComboBox* comBobox = this->findChild<QComboBox*>("Skew_ID_Box");
        if (!comBobox)return;
        int index = comBobox->findData(value->getSkewID());
        comBobox->setCurrentIndex(index);

        //Smoothing length change based on volume(h_ID)
        comBobox = this->findChild<QComboBox*>("Smoothing_Length_Change_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getHID());
        comBobox->setCurrentIndex(index);

        //SPH correction order (order)   
        comBobox = this->findChild<QComboBox*>("SPH_Correction_Order_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getOrderID());
        comBobox->setCurrentIndex(index);

        //Smoothing length (h)
        edit = this->findChild<QLineEdit*>("Smoothing_Length_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getH()));

        //Coefficient for solving tensile instability (stab)
        edit = this->findChild<QLineEdit*>("Tensile_Instability_Coefficient_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getStab()));

    }
    bool GUIPropertySPHDialog::ckeckData()
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
    void GUIPropertySPHDialog::getDataFormWidget()
    {
        Radioss::FITKPropSPH* obj = dynamic_cast<Radioss::FITKPropSPH*>(_obj);
        if (!obj)return;
        Radioss::FITKPropSPHValue* value = obj->getValue();
        if (!value)return;
        //粒子质量(Mp)
        QLineEdit* edit = this->findChild<QLineEdit*>("Particle_Mass_LineEdit");
        if (!edit)return;
        value->setMP(edit->text().toDouble());
        //Quadratic bulk viscosity (qa)
        edit = this->findChild<QLineEdit*>("Quadratic_Bulk_Viscosity_LineEdit");
        if (!edit)return;
        value->setQA(edit->text().toDouble());
        //Linear bulk viscosity (qb)
        edit = this->findChild<QLineEdit*>("Linear_Bulk_Viscosity_LineEdit");
        if (!edit)return;
        value->setQB(edit->text().toDouble());
        //Conservative smoothing coefficient (acs)
        edit = this->findChild<QLineEdit*>("Conservative_Smoothing_Coefficient_LineEdit");
        if (!edit)return;
        value->setACS(edit->text().toDouble());
        //Skew identifier to define the initial orthotropic directions in the case of skew_ID
        QComboBox* comBobox = this->findChild<QComboBox*>("Skew_ID_Box");
        if (!comBobox)return;
        value->setSkewID(comBobox->currentData().toInt());
        //Smoothing length change based on volume(h_ID)
        comBobox = this->findChild<QComboBox*>("Smoothing_Length_Change_Box");
        if (!comBobox)return;
        value->setHID(comBobox->currentData().toInt());
        //SPH correction order (order)
        comBobox = this->findChild<QComboBox*>("SPH_Correction_Order_Box");
        if (!comBobox)return;
        value->setOrderID(comBobox->currentData().toInt());
        //Smoothing length (h)
        edit = this->findChild<QLineEdit*>("Smoothing_Length_LineEdit");
        if (!edit)return;
        value->setH(edit->text().toDouble());
        //Coefficient for solving tensile instability (stab)
        edit = this->findChild<QLineEdit*>("Tensile_Instability_Coefficient_LineEdit");
        if (!edit)return;
        value->setStab(edit->text().toDouble());
    }

    void GUIPropertySPHDialog::writePythonScript()
    {
        Radioss::FITKPropSPH* obj = dynamic_cast<Radioss::FITKPropSPH*>(_obj);
        if (obj == nullptr)
        {
            return;
        }

        Radioss::FITKPropSPHValue* value = obj->getValue();
        if (value == nullptr)
        {
            return;
        }

        QStringList script;
        if (_isEditMode)
        {
            QString oldName = _oldNameForScript.isEmpty() ? obj->getDataObjectName() : _oldNameForScript;
            script.append(QString("p=Property.SPH.GetSPH('%1')").arg(oldName));
            if (oldName != obj->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(obj->getDataObjectName()));
            }
        }
        else
        {
            script.append("p=Property.SPH()");
            script.append(QString("p.createProperty('%1')").arg(obj->getDataObjectName()));
        }

        script.append(QString("p.setMP(%1)").arg(value->getMP()));
        script.append(QString("p.setQA(%1)").arg(value->getQA()));
        script.append(QString("p.setQB(%1)").arg(value->getQB()));
        script.append(QString("p.setACS(%1)").arg(value->getACS()));
        script.append(QString("p.setHID(%1)").arg(value->getHID()));
        script.append(QString("p.setOrderID(%1)").arg(value->getOrderID()));
        script.append(QString("p.setH(%1)").arg(value->getH()));
        script.append(QString("p.setStab(%1)").arg(value->getStab()));

        this->saveScript(script);
    }
}