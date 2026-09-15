/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIFailureBIQUAD_Dialog.h"
#include "ui_GUIFailureModelDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKFailureModelBIQUAD.h"

#include <QComboBox>
#include <QMessageBox>

namespace GUI
{
    GUIFailureBIQUAD_Dialog::GUIFailureBIQUAD_Dialog(Core::FITKActionOperator * oper, Radioss::FITKAbstractFailureModel * obj, QWidget * parent)
        : GUIFailureModelDialog(oper, obj, parent)
    {
        _obj = dynamic_cast<Radioss::FITKFailureModelBIQUAD*>(obj);
        if (_obj)
            _isEditMode = true;
        else
            _obj = new Radioss::FITKFailureModelBIQUAD();

        // 初始化
        this->initUI();
        this->init();
    }
    void GUIFailureBIQUAD_Dialog::initUI()
    {
        // 清空
        _ui->tableWidget->clear();
        _ui->tableWidget->setRowCount(0);
        // 设置列数
        _ui->tableWidget->setColumnCount(2);
        _ui->tableWidget->setHorizontalHeaderLabels(QStringList{ "Name","Value" });

        if (!_isEditMode) {
            QString name;
            _oper->argValue("FailureModelName", name);
            _ui->lineEditName->setText(name);
        }
        _ui->lineEditKeyWord->setText("/FAIL/BIQUAD");
        _ui->lineEditKeyWord->setReadOnly(true);
        _ui->lineEditType->setText("Biquad");
        _ui->lineEditType->setReadOnly(true);
        // 获取曲线管理器
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return;
        Radioss::FITKNonLinerCurveManager* curveManager = caseData->getNonLinerCurveManager();
        if (curveManager == nullptr) return;
        //c1
        this->addLineEdit("Failure plastic strain at uniaxial compression(c1)", "C1_LineEdit");
        //c2
        this->addLineEdit("Failure plastic strain at shear(c2)", "C2_LineEdit");
        //c3
        this->addLineEdit("Failure plastic strain in uniaxial tension(c3)", "C3_LineEdit");
        //c4
        this->addLineEdit("Failure plastic strain at plain strain tension(c4)", "C4_LineEdit");
        //c5
        this->addLineEdit("Failure strain at biaxial tension(c5)", "C5_LineEdit");
        //P_thickfail
        this->addLineEdit("Percentage of through thickness integration points(P_thickfail)", "P_Thickfail_LineEdit");
        //M_Flag
        QComboBox* comBoboxM_Flag = addComboBox("Material selector flag(M_Flag)", "M_Flag_Box");
        comBoboxM_Flag->addItem(tr("Default"), 0);
        comBoboxM_Flag->addItem(tr("Mild steel"), 1);
        comBoboxM_Flag->addItem(tr("HSS steel"), 2);
        comBoboxM_Flag->addItem(tr("UHSS steel"), 3);
        comBoboxM_Flag->addItem(tr("Aluminum AA5182"), 4);
        comBoboxM_Flag->addItem(tr("Aluminum AA6082-T6"), 5);
        comBoboxM_Flag->addItem(tr("Plastic PA6GF30"), 6);
        comBoboxM_Flag->addItem(tr("Plastic PP T40"), 7);
        comBoboxM_Flag->addItem(tr("User-defined r1,r2,r4,r5"), 99);
        //S_Flag
        QComboBox* comBoboxS_Flag = addComboBox("Specific behavior flag(S_Flag)", "S_Flag_Box");
        comBoboxS_Flag->addItem(tr("Default"), 1);
        comBoboxS_Flag->addItem(tr("global minimum"), 2);
        comBoboxS_Flag->addItem(tr("global minimum+localized necking"), 3);
        //Inst_start
        this->addLineEdit("Instability start value for localized necking(Inst_start)", "Inst_Start_LineEdit");
        //fct_IDel
        QComboBox* comBoboxfct_IDel = addComboBox("Element size factor function(fct_IDel)", "Fct_IDel_Box");
        comBoboxfct_IDel->addItem("None", 0);
        for (int i = 0; i < curveManager->getDataCount();++i) {
            Radioss::FITKNonLinerCurve* curve = curveManager->getDataByIndex(i);
            if (!curve)continue;
            comBoboxfct_IDel->addItem(curve->getDataObjectName(), curve->getDataObjectID());
        }
        //El_ref
        this->addLineEdit("Reference element size(El_ref)", "El_Ref_LineEdit");
        //r1
        QLineEdit* edit = this->addLineEdit("Failure plastic strain ratio:r1(c1=r1*c3)", "R1_LineEdit");
        edit->setEnabled(false);
        //r2
        edit = this->addLineEdit("Failure plastic strain ratio:r2(c2=r2*c3)", "R2_LineEdit");
        edit->setEnabled(false);
        //r4
        edit = this->addLineEdit("Failure plastic strain ratio:r4(c4=r4*c3)", "R4_LineEdit");
        edit->setEnabled(false);
        //r5
        edit = this->addLineEdit("Failure plastic strain ratio:r5(c5=r5*c3)", "R5_LineEdit");
        edit->setEnabled(false);
        //if M_Flag=99,user-defined r1,r2,r4,r5 are required, otherwise they are ignored.
        connect(comBoboxM_Flag, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
            bool isUserDefined = (comBoboxM_Flag->currentData().toInt() == 99);
            QLineEdit* r1_lineEdit = this->findChild<QLineEdit*>("R1_LineEdit");
            QLineEdit* r2_lineEdit = this->findChild<QLineEdit*>("R2_LineEdit");
            QLineEdit* r4_lineEdit = this->findChild<QLineEdit*>("R4_LineEdit");
            QLineEdit* r5_lineEdit = this->findChild<QLineEdit*>("R5_LineEdit");
            if (r1_lineEdit) r1_lineEdit->setEnabled(isUserDefined);
            if (r2_lineEdit) r2_lineEdit->setEnabled(isUserDefined);
            if (r4_lineEdit) r4_lineEdit->setEnabled(isUserDefined);
            if (r5_lineEdit) r5_lineEdit->setEnabled(isUserDefined);
            });

        // 行高
        _ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        // 宽度
        _ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    }
    void GUIFailureBIQUAD_Dialog::init()
    {
        Radioss::FITKFailureModelBIQUAD* obj = dynamic_cast<Radioss::FITKFailureModelBIQUAD*>(_obj);
        if (!obj)return;
        if (_isEditMode) {
            _ui->lineEditName->setText(obj->getDataObjectName());
        }
        //c1
        QLineEdit* edit = this->findChild<QLineEdit*>("C1_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getC1()));
        //c2
        edit = this->findChild<QLineEdit*>("C2_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getC2()));
        //c3
        edit = this->findChild<QLineEdit*>("C3_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getC3()));
        //c4
        edit = this->findChild<QLineEdit*>("C4_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getC4()));
        //c5
        edit = this->findChild<QLineEdit*>("C5_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getC5()));
        //P_thickfail
        edit = this->findChild<QLineEdit*>("P_Thickfail_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getPthickfail()));
        //M_Flag
        QComboBox* comBoboxM_Flag = this->findChild<QComboBox*>("M_Flag_Box");
        if (comBoboxM_Flag) {
            int index = comBoboxM_Flag->findData(obj->getMFlag());
            comBoboxM_Flag->setCurrentIndex(index);
        }
        //S_Flag
        QComboBox* comBoboxS_Flag = this->findChild<QComboBox*>("S_Flag_Box");
        if (comBoboxS_Flag) {
            int index = comBoboxS_Flag->findData(obj->getSFlag());
            comBoboxS_Flag->setCurrentIndex(index);
        }
        //Inst_start
        edit = this->findChild<QLineEdit*>("Inst_Start_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getInststart()));
        //fct_IDel
        QComboBox* comBoboxfct_IDel = this->findChild<QComboBox*>("Fct_IDel_Box");
        if (comBoboxfct_IDel) {
            int index = comBoboxfct_IDel->findData(obj->getFctIDel());
            comBoboxfct_IDel->setCurrentIndex(index);
        }
        //El_ref
        edit = this->findChild<QLineEdit*>("El_Ref_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getElref()));
        //if M_Flag=99,初始化 r1,r2,r4,r5 
        if (obj->getMFlag()!= 99)return;
        //r1
        edit = this->findChild<QLineEdit*>("R1_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getR1()));
        //r2
        edit = this->findChild<QLineEdit*>("R2_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getR2()));
        //r4
        edit = this->findChild<QLineEdit*>("R4_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getR4()));
        //r5
        edit = this->findChild<QLineEdit*>("R5_LineEdit");
        if (edit)
            edit->setText(QString::number(obj->getR5()));
    }
    bool GUIFailureBIQUAD_Dialog::ckeckData()
    {
        //获取名称，查重处理
        auto mgr = this->getFailureManager();
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
    void GUIFailureBIQUAD_Dialog::getDataFormWidget()
    {
        Radioss::FITKFailureModelBIQUAD* obj = dynamic_cast<Radioss::FITKFailureModelBIQUAD*>(_obj);
        if (!obj)return;
        //c1
        QLineEdit* edit = this->findChild<QLineEdit*>("C1_LineEdit");
        if (edit)
            obj->setC1(edit->text().toDouble());
        //c2
        edit = this->findChild<QLineEdit*>("C2_LineEdit");
        if (edit)
            obj->setC2(edit->text().toDouble());
        //c3
        edit = this->findChild<QLineEdit*>("C3_LineEdit");
        if (edit)
            obj->setC3(edit->text().toDouble());
        //c4
        edit = this->findChild<QLineEdit*>("C4_LineEdit");
        if (edit)
            obj->setC4(edit->text().toDouble());
        //c5
        edit = this->findChild<QLineEdit*>("C5_LineEdit");
        if (edit)
            obj->setC5(edit->text().toDouble());
        //P_thickfail
        edit = this->findChild<QLineEdit*>("P_Thickfail_LineEdit");
        if (edit)
            obj->setPthickfail(edit->text().toDouble());
        //M_Flag
        QComboBox* comBoboxM_Flag = this->findChild<QComboBox*>("M_Flag_Box");
        if (comBoboxM_Flag)
            obj->setMFlag(comBoboxM_Flag->currentData().toInt());
        //S_Flag
        QComboBox* comBoboxS_Flag = this->findChild<QComboBox*>("S_Flag_Box");
        if (comBoboxS_Flag)
            obj->setSFlag(comBoboxS_Flag->currentData().toInt());
        //Inst_start
        edit = this->findChild<QLineEdit*>("Inst_Start_LineEdit");
        if (edit)
            obj->setInststart(edit->text().toDouble());
        //fct_IDel
        QComboBox* comBoboxfct_IDel = this->findChild<QComboBox*>("Fct_IDel_Box");
        if (comBoboxfct_IDel)
            obj->setFctIDel(comBoboxfct_IDel->currentData().toInt());
        //El_ref
        edit = this->findChild<QLineEdit*>("El_Ref_LineEdit");
        if (edit)
            obj->setElref(edit->text().toDouble());
        //if M_Flag=99,获取 r1,r2,r4,r5
        if (obj->getMFlag() != 99)return;
        //r1
        edit = this->findChild<QLineEdit*>("R1_LineEdit");
        if (edit)
            obj->setR1(edit->text().toDouble());
        //r2
        edit = this->findChild<QLineEdit*>("R2_LineEdit");
        if (edit)
            obj->setR2(edit->text().toDouble());
        //r4
        edit = this->findChild<QLineEdit*>("R4_LineEdit");
        if (edit)
            obj->setR4(edit->text().toDouble());
        //r5
        edit = this->findChild<QLineEdit*>("R5_LineEdit");
        if (edit)
            obj->setR5(edit->text().toDouble());

    }

    void GUIFailureBIQUAD_Dialog::writePythonScript()
    {
        Radioss::FITKFailureModelBIQUAD* obj = dynamic_cast<Radioss::FITKFailureModelBIQUAD*>(_obj);
        if (obj == nullptr)
        {
            return;
        }

        QStringList script;
        if (!_isEditMode)
        {
            script.append("f=Failure.BIQUAD()");
            script.append(QString("f.createFailureModel('%1')").arg(obj->getDataObjectName()));
        }
        else
        {
            QString oldName = _oldNameForScript.isEmpty() ? obj->getDataObjectName() : _oldNameForScript;
            script.append(QString("f=Failure.BIQUAD.GetBIQUAD('%1')").arg(oldName));
            if (oldName != obj->getDataObjectName())
            {
                script.append(QString("f.setName('%1')").arg(obj->getDataObjectName()));
            }
        }

        script.append(QString("f.setC1(%1)").arg(obj->getC1()));
        script.append(QString("f.setC2(%1)").arg(obj->getC2()));
        script.append(QString("f.setC3(%1)").arg(obj->getC3()));
        script.append(QString("f.setC4(%1)").arg(obj->getC4()));
        script.append(QString("f.setC5(%1)").arg(obj->getC5()));
        script.append(QString("f.setPthickfail(%1)").arg(obj->getPthickfail()));
        script.append(QString("f.setMFlag(%1)").arg(obj->getMFlag()));
        script.append(QString("f.setSFlag(%1)").arg(obj->getSFlag()));
        script.append(QString("f.setInststart(%1)").arg(obj->getInststart()));

        QComboBox* comBoboxfct_IDel = this->findChild<QComboBox*>("Fct_IDel_Box");
        QString curveName;
        if (comBoboxfct_IDel != nullptr && comBoboxfct_IDel->currentData().toInt() > 0)
        {
            curveName = comBoboxfct_IDel->currentText();
        }
        script.append(QString("f.setFctIDel('%1')").arg(curveName));

        script.append(QString("f.setElref(%1)").arg(obj->getElref()));
        if (obj->getMFlag() == 99)
        {
            script.append(QString("f.setR1(%1)").arg(obj->getR1()));
            script.append(QString("f.setR2(%1)").arg(obj->getR2()));
            script.append(QString("f.setR4(%1)").arg(obj->getR4()));
            script.append(QString("f.setR5(%1)").arg(obj->getR5()));
        }

        this->saveScript(script);
    }
}