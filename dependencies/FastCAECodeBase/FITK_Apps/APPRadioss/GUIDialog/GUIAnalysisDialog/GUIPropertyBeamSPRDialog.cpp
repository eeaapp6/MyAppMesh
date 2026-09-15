/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPropertyBeamSPRDialog.h"
#include "ui_GUIPropertyDialog.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeamSPR.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "GUIPropertyBeamSPR_DOF_Dialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <QComboBox>
#include <QMessageBox>
#include <QCheckBox>

namespace GUI
{
    namespace
    {
        QString getBeamSPRCurveName(int curveId)
        {
            if (curveId <= 0)
            {
                return QString();
            }

            Radioss::FITKRadiossCase* caseData =
                FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
            if (caseData == nullptr)
            {
                return QString();
            }

            Radioss::FITKNonLinerCurveManager* curveManager = caseData->getNonLinerCurveManager();
            if (curveManager == nullptr)
            {
                return QString();
            }

            Radioss::FITKNonLinerCurve* curve = curveManager->getDataByID(curveId);
            if (curve == nullptr)
            {
                return QString();
            }

            return curve->getDataObjectName();
        }
    }

    GUIPropertyBeamSPRDialog::GUIPropertyBeamSPRDialog(Core::FITKActionOperator* oper, Radioss::FITKPropAbstract* obj, QWidget* parent)
        : GUIPropertyDialog(oper, obj, parent)
    {
        _obj = dynamic_cast<Radioss::FITKPropBeamSPR*>(obj);
        if (_obj)
            _isEditMode = true;
        else
            _obj = new Radioss::FITKPropBeamSPR();

        // 初始化
        this->initUI();
        this->init();
    }
    void GUIPropertyBeamSPRDialog::initUI()
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
        _ui->lineEditType->setText("SPR_BEAM");
        _ui->lineEditType->setReadOnly(true);
        //mass
        this->addLineEdit("Mass", "Mass_LineEdit");
        //inertia
        this->addLineEdit("Inertia", "Inertia_LineEdit");
        //skewID
        QComboBox* comBobox = addComboBox("Skew_ID", "SkewID_Box");
        comBobox->addItem(tr("Global"), 0);
        //sensID
        comBobox = addComboBox("Sensor_Identifier", "Sensor_Box");
        comBobox->addItem(tr("Active"), 0);
        comBobox->addItem(tr("Inactive"), 1);
        //Isflag
        comBobox = addComboBox("Sensor_Flag", "IsFlag_Box");
        comBobox->addItem(tr("Activat when Sensor activates"), 0);
        comBobox->addItem(tr("Inactive when Sensor activates"), 1);
        comBobox->addItem(tr("State matches the Sensor"), 2);
        //Ifail
        comBobox = addComboBox("Failure_Criteria", "Ifail_Box");
        comBobox->addItem(tr("Uni-directional"), 0);
        comBobox->addItem(tr("Multi-directional"), 1);
        //Ileng
        comBobox = addComboBox("Input_Per_Unit_Length", "Ileng_Box");
        comBobox->addItem(tr("Use definition table"), 0);
        comBobox->addItem(tr("Not used definition table"), 1);
        //Ifai2
        comBobox = addComboBox("Failure_Model", "Ifail2_Box");
        comBobox->addItem(tr("Displacement and rotation"), 0);
        comBobox->addItem(tr("Displacement and rotation with velocity"), 1);
        comBobox->addItem(tr("Force and moment"), 2);
        comBobox->addItem(tr("Internal energy"), 3);

        //Tension_Compression
        QPushButton* button = this->addPushButton("Tension_Compression", "Tension Compression Settings");
        connect(button, &QPushButton::clicked, this, [this]() { this->onDOFButtonClicked(Radioss::DOF_SPR::Tension_Compression); });
        //Shear_XY
        button = this->addPushButton("Shear_XY", "Shear XY Settings");
        connect(button, &QPushButton::clicked, this, [this]() { this->onDOFButtonClicked(Radioss::DOF_SPR::Shear_XY); });
        //Shear_XZ
        button = this->addPushButton("Shear_XZ", "Shear XZ Settings");
        connect(button, &QPushButton::clicked, this, [this]() { this->onDOFButtonClicked(Radioss::DOF_SPR::Shear_XZ); });
        //Torsion
        button = this->addPushButton("Torsion", "Torsion Settings");
        connect(button, &QPushButton::clicked, this, [this]() { this->onDOFButtonClicked(Radioss::DOF_SPR::Torsion); });
        //Bending_Y
        button = this->addPushButton("Bending_Y", "Bending Y Settings");
        connect(button, &QPushButton::clicked, this, [this]() { this->onDOFButtonClicked(Radioss::DOF_SPR::Bending_Y); });
        //Bending_Z
        button = this->addPushButton("Bending_Z", "Bending Z Settings");
        connect(button, &QPushButton::clicked, this, [this]() { this->onDOFButtonClicked(Radioss::DOF_SPR::Bending_Z); });

        //v0
        this->addLineEdit("Reference_Velocity", "Reference_Velocity_LineEdit");
        //w0
        this->addLineEdit("Reference_Angular_Velocity", "Reference_Angular_Velocity_LineEdit");
        //Fcut
        this->addLineEdit("Strain_Rate_Cutoff_Frequency", "Strain_Rate_Cutoff_Frequency_LineEdit");
        //Fsmooth
        comBobox = addComboBox("Strain_Rate_Smoothing_Flag", "Strain_Rate_Smoothing_Flag_Box");
        comBobox->addItem(tr("Inactive"), 0);
        comBobox->addItem(tr("Active"), 1);

        //调整宽度
        _ui->tableWidgetValue->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
    void GUIPropertyBeamSPRDialog::init()
    {
        Radioss::FITKPropBeamSPR* obj = dynamic_cast<Radioss::FITKPropBeamSPR*>(_obj);
        if (!obj)return;
        Radioss::FITKPropValueBeamSPR* value = obj->getValue();
        if (!value)return;
        if (_isEditMode) {
            _ui->lineEditName->setText(obj->getDataObjectName());
        }
        _ui->lineEditPropertyID->setText(obj->GetFITKPropBeamSPRRadiossKeyWord());
        //mass
        QLineEdit* edit = this->findChild<QLineEdit*>("Mass_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getMass()));
        //inertia
        edit = this->findChild<QLineEdit*>("Inertia_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getInertia()));
        //skewID
        QComboBox* comBobox = this->findChild<QComboBox*>("SkewID_Box");
        if (!comBobox)return;          
        int index = comBobox->findData(value->getSkewID());
        comBobox->setCurrentIndex(index);
        //sensID
        comBobox = this->findChild<QComboBox*>("Sensor_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getSensID());
        comBobox->setCurrentIndex(index);
        //Isflag
        comBobox = this->findChild<QComboBox*>("IsFlag_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIsflag());
        comBobox->setCurrentIndex(index);
        //Ifail
        comBobox = this->findChild<QComboBox*>("Ifail_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIfail());
        comBobox->setCurrentIndex(index);
        //Ileng
        comBobox = this->findChild<QComboBox*>("Ileng_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIleng());
        comBobox->setCurrentIndex(index);
        //Ifail2
        comBobox = this->findChild<QComboBox*>("Ifail2_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIfail2());
        comBobox->setCurrentIndex(index);
        //v0
        edit = this->findChild<QLineEdit*>("Reference_Velocity_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getV0()));
        //w0
        edit = this->findChild<QLineEdit*>("Reference_Angular_Velocity_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getW0()));
        //Fcut
        edit = this->findChild<QLineEdit*>("Strain_Rate_Cutoff_Frequency_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getFcut()));
        //Fsmooth
        comBobox = this->findChild<QComboBox*>("Strain_Rate_Smoothing_Flag_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getFsmooth());
        comBobox->setCurrentIndex(index);

    }
    bool GUIPropertyBeamSPRDialog::ckeckData()
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
    void GUIPropertyBeamSPRDialog::getDataFormWidget()
    {
        Radioss::FITKPropBeamSPR* obj = dynamic_cast<Radioss::FITKPropBeamSPR*>(_obj);
        if (!obj)return;
        Radioss::FITKPropValueBeamSPR* value = obj->getValue();
        if (!value)return;
        //mass
        QLineEdit* edit = this->findChild<QLineEdit*>("Mass_LineEdit");
        if (!edit)return;
        value->setMass(edit->text().toDouble());
        //inertia
        edit = this->findChild<QLineEdit*>("Inertia_LineEdit");
        if (!edit)return;
        value->setInertia(edit->text().toDouble());
        //skewID
        QComboBox* comBobox = this->findChild<QComboBox*>("SkewID_Box");
        if (!comBobox)return;
        value->setSkewID(comBobox->currentData().toInt());
        //sensID
        comBobox = this->findChild<QComboBox*>("Sensor_Box");
        if (!comBobox)return;
        value->setSensID(comBobox->currentData().toInt());
        //Isflag
        comBobox = this->findChild<QComboBox*>("IsFlag_Box");
        if (!comBobox)return;
        value->setIsflag(comBobox->currentData().toInt());
        //Ifail
        comBobox = this->findChild<QComboBox*>("Ifail_Box");
        if (!comBobox)return;
        value->setIfail(comBobox->currentData().toInt());
        //Ileng
        comBobox = this->findChild<QComboBox*>("Ileng_Box");
        if (!comBobox)return;
        value->setIleng(comBobox->currentData().toInt());
        //Ifail2
        comBobox = this->findChild<QComboBox*>("Ifail2_Box");
        if (!comBobox)return;
        value->setIfail2(comBobox->currentData().toInt());
        //v0
        edit = this->findChild<QLineEdit*>("Reference_Velocity_LineEdit");
        if (!edit)return;
        value->setV0(edit->text().toDouble());
        //w0
        edit = this->findChild<QLineEdit*>("Reference_Angular_Velocity_LineEdit");
        if (!edit)return;
        value->setW0(edit->text().toDouble());
        //Fcut
        edit = this->findChild<QLineEdit*>("Strain_Rate_Cutoff_Frequency_LineEdit");
        if (!edit)return;
        value->setFcut(edit->text().toDouble());
        //Fsmooth
        comBobox = this->findChild<QComboBox*>("Strain_Rate_Smoothing_Flag_Box");
        if (!comBobox)return;
        value->setFsmooth(comBobox->currentData().toInt());

    }

    void GUIPropertyBeamSPRDialog::writePythonScript()
    {
        Radioss::FITKPropBeamSPR* obj = dynamic_cast<Radioss::FITKPropBeamSPR*>(_obj);
        if (obj == nullptr)
        {
            return;
        }

        Radioss::FITKPropValueBeamSPR* value = obj->getValue();
        if (value == nullptr)
        {
            return;
        }

        QStringList script;
        if (_isEditMode)
        {
            QString oldName = _oldNameForScript.isEmpty() ? obj->getDataObjectName() : _oldNameForScript;
            script.append(QString("p=Property.BeamSPR.GetBeamSPR('%1')").arg(oldName));
            if (oldName != obj->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(obj->getDataObjectName()));
            }
        }
        else
        {
            script.append("p=Property.BeamSPR()");
            script.append(QString("p.createProperty('%1')").arg(obj->getDataObjectName()));
        }

        script.append(QString("p.setMass(%1)").arg(value->getMass()));
        script.append(QString("p.setInertia(%1)").arg(value->getInertia()));
        //script.append(QString("p.setSensID(%1)").arg(value->getSensID()));
        script.append(QString("p.setIsflag(%1)").arg(value->getIsflag()));
        script.append(QString("p.setIfail(%1)").arg(value->getIfail()));
        script.append(QString("p.setIleng(%1)").arg(value->getIleng()));
        script.append(QString("p.setIfail2(%1)").arg(value->getIfail2()));
        script.append(QString("p.setV0(%1)").arg(value->getV0()));
        script.append(QString("p.setW0(%1)").arg(value->getW0()));
        script.append(QString("p.setFcut(%1)").arg(value->getFcut()));
        script.append(QString("p.setFsmooth(%1)").arg(value->getFsmooth()));

        for (int dof = (int)Radioss::DOF_SPR::Tension_Compression;
             dof <= (int)Radioss::DOF_SPR::Bending_Z; ++dof)
        {
            Radioss::ValueDOF& dofValue = value->getValueDOF(static_cast<Radioss::DOF_SPR>(dof));
            script.append(QString("p.setK(%1, %2)").arg(dof).arg(dofValue.K));
            script.append(QString("p.setC(%1, %2)").arg(dof).arg(dofValue.C));
            script.append(QString("p.setA(%1, %2)").arg(dof).arg(dofValue.A));
            script.append(QString("p.setB(%1, %2)").arg(dof).arg(dofValue.B));
            script.append(QString("p.setD(%1, %2)").arg(dof).arg(dofValue.D));

            QString curveName = getBeamSPRCurveName(dofValue.fct_ID1);
            if (!curveName.isEmpty())
            {
                script.append(QString("p.setFunction1(%1, '%2')").arg(dof).arg(curveName));
            }

            script.append(QString("p.setH(%1, %2)").arg(dof).arg(dofValue.H));

            curveName = getBeamSPRCurveName(dofValue.fct_ID2);
            if (!curveName.isEmpty())
            {
                script.append(QString("p.setFunction2(%1, '%2')").arg(dof).arg(curveName));
            }

            curveName = getBeamSPRCurveName(dofValue.fct_ID3);
            if (!curveName.isEmpty())
            {
                script.append(QString("p.setFunction3(%1, '%2')").arg(dof).arg(curveName));
            }

            curveName = getBeamSPRCurveName(dofValue.fct_ID4);
            if (!curveName.isEmpty())
            {
                script.append(QString("p.setFunction4(%1, '%2')").arg(dof).arg(curveName));
            }

            script.append(QString("p.setMin(%1, %2)").arg(dof).arg(dofValue.min));
            script.append(QString("p.setMax(%1, %2)").arg(dof).arg(dofValue.max));
            script.append(QString("p.setF(%1, %2)").arg(dof).arg(dofValue.F));
            script.append(QString("p.setE(%1, %2)").arg(dof).arg(dofValue.E));
            script.append(QString("p.setAscale(%1, %2)").arg(dof).arg(dofValue.Ascale));
            script.append(QString("p.setHscale(%1, %2)").arg(dof).arg(dofValue.Hscale));
            script.append(QString("p.setVc(%1, %2)").arg(dof).arg(dofValue.vc));
            script.append(QString("p.setN(%1, %2)").arg(dof).arg(dofValue.n));
            script.append(QString("p.setAlpha(%1, %2)").arg(dof).arg(dofValue.a));
            script.append(QString("p.setExponent(%1, %2)").arg(dof).arg(dofValue.e));
        }

        this->saveScript(script);
    }

    QPushButton* GUIPropertyBeamSPRDialog::addPushButton(const QString& label, const QString& buttonText)
    {
        int row = _ui->tableWidgetValue->rowCount();
        _ui->tableWidgetValue->insertRow(row);
        QTableWidgetItem* item = new QTableWidgetItem(label);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        _ui->tableWidgetValue->setItem(row, 0, item);

        QPushButton* button = new QPushButton(buttonText);
        _ui->tableWidgetValue->setCellWidget(row, 1, button);
        return button;
    }

    void GUIPropertyBeamSPRDialog::onDOFButtonClicked(int Type)
    {
        Radioss::FITKPropBeamSPR* obj = dynamic_cast<Radioss::FITKPropBeamSPR*>(_obj);
        if (!obj)return;
        Radioss::FITKPropValueBeamSPR* value = obj->getValue();
        if (!value)return;
        Radioss::ValueDOF& dofValue = value->getValueDOF(static_cast<Radioss::DOF_SPR>(Type));
        GUIPropertyBeamSPR_DOF_Dialog* dlg = new GUIPropertyBeamSPR_DOF_Dialog(dofValue, this);
        dlg->exec();
    }
}