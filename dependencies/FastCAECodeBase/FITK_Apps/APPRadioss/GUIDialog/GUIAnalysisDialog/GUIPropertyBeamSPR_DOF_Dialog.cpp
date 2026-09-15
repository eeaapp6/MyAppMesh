/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPropertyBeamSPR_DOF_Dialog.h"
#include "ui_GUIPropertyDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include <QComboBox>

namespace GUI
{
    GUIPropertyBeamSPR_DOF_Dialog::GUIPropertyBeamSPR_DOF_Dialog(Radioss::ValueDOF& obj, QWidget* parent)
        : GUIPropertyDialog(nullptr, nullptr, parent), _objDof(obj)
    {
        setWindowTitle(tr("DOF Property"));
        // 初始化
        this->initUI();
        this->init();
    }
    void GUIPropertyBeamSPR_DOF_Dialog::initUI()
    {
        _ui->labelName->setVisible(false);
        _ui->lineEditName->setVisible(false);
        _ui->labelPropertyID->setVisible(false);
        _ui->lineEditPropertyID->setVisible(false);
        // 清空
        _ui->tableWidgetValue->clear();
        _ui->tableWidgetValue->setRowCount(0);
        // 设置列数
        _ui->tableWidgetValue->setColumnCount(2);
        _ui->tableWidgetValue->setHorizontalHeaderLabels(QStringList{ "Name","Value" });
        //K
        this->addLineEdit("Stiffness", "Stiffness_LineEdit");
        //C
        this->addLineEdit("Damping", "Damping_LineEdit");
        //A
        this->addLineEdit("Nonlinear_Stiffness_Scale_Factor", "Nonlinear_Stiffness_Scale_Factor_LineEdit");
        //B
        this->addLineEdit("Logarithmic_Rate_Effects_Scale_Factor_B", "Logarithmic_Rate_Effects_Scale_FactorB_LineEdit");
        //D
        this->addLineEdit("Logarithmic_Rate_Effects_Scale_Factor_D", "Logarithmic_Rate_Effects_Scale_FactorD_LineEdit");
        //fct_ID1
        QComboBox* comBobox = addComboBox("Function_Identifier_1", "Function_Identifier_1_Box",true);
        //H
        comBobox = addComboBox("Spring_Hardening_Flag", "Spring_Hardening_Flag_Box",false);
        comBobox->addItem(tr("Elastic spring"), 0); 
        comBobox->addItem(tr("Nonlinear elasto-plastic spring with isotropic hardening"), 1);
        comBobox->addItem(tr("Nonlinear elasto-plastic spring with uncoupled hardening"), 2);
        comBobox->addItem(tr("Nonlinear elasto-plastic spring with kinematic hardening"), 4);
        comBobox->addItem(tr("Nonlinear elasto-plastic spring with nonlinear hardening"), 5);
        comBobox->addItem(tr("Nonlinear elasto-plastic spring with isotropic hardening and nonlinear unloading"), 6);
        comBobox->addItem(tr("Nonlinear elastic plastic spring with elastic hysteresis"), 7);
        //fct_ID2
        comBobox = addComboBox("Function_Identifier_2", "Function_Identifier_2_Box",true);
        //fct_ID3
        comBobox = addComboBox("Function_Identifier_3", "Function_Identifier_3_Box", true);
        //fct_ID4
        comBobox = addComboBox("Function_Identifier_4", "Function_Identifier_4_Box", true);
        //min
        this->addLineEdit("Dmin", "Dmin_LineEdit");
        //max
        this->addLineEdit("Dmax", "Dmax_LineEdit");
        //F
        this->addLineEdit("Abscissa_Scale_Factor_Damping_Functions", "Abscissa_Scale_Factor_Damping_Functions_LineEdit");
        //E
        this->addLineEdit("Ordinate_Scale_Factor_Damping_Function_E", "Ordinate_Scale_Factor_Damping_FunctionE_LineEdit");
        //Ascale
        this->addLineEdit("Abscissa_Scale_Factor_Stiffness_Function", "Abscissa_Scale_Factor_Stiffness_Function_LineEdit");
        //Hscale
        this->addLineEdit("Ordinate_Scale_Factor_Damping_Function_H", "Ordinate_Scale_Factor_Damping_FunctionH_LineEdit");
        //vc
        this->addLineEdit("Relative_Velocity_Coefficient", "Relative_Velocity_Coefficient_LineEdit");
        //n
        this->addLineEdit("Relative_Velocity_Exponent", "Relative_Velocity_Exponent_LineEdit");
        //a
        this->addLineEdit("Alpha", "alpha_LineEdit");
        //e
        this->addLineEdit("Beta", "beta_LineEdit");

        //调整宽度
        _ui->tableWidgetValue->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    void GUIPropertyBeamSPR_DOF_Dialog::init()
    {
        QString typeName ;
        if (_objDof.dof == Radioss::DOF_SPR::Tension_Compression)
            typeName = "Tension Compression";
        else if (_objDof.dof == Radioss::DOF_SPR::Shear_XY)
            typeName = "Shear XY";
        else if (_objDof.dof == Radioss::DOF_SPR::Shear_XZ)
            typeName = "Shear XZ";
        else if (_objDof.dof == Radioss::DOF_SPR::Torsion)
            typeName = "Torsion";
        else if (_objDof.dof == Radioss::DOF_SPR::Bending_Y)
            typeName = "Bending Y";
        else if (_objDof.dof == Radioss::DOF_SPR::Bending_Z)
            typeName = "Bending Z";
        else
            typeName = "Unknown";
        _ui->lineEditType->setText(typeName);
        _ui->lineEditType->setReadOnly(true);
        //K
        QLineEdit* edit = this->findChild<QLineEdit*>("Stiffness_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.K));
        //C
        edit = this->findChild<QLineEdit*>("Damping_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.C));
        //A
        edit = this->findChild<QLineEdit*>("Nonlinear_Stiffness_Scale_Factor_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.A));
        //B
        edit = this->findChild<QLineEdit*>("Logarithmic_Rate_Effects_Scale_FactorB_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.B));
        //D
        edit = this->findChild<QLineEdit*>("Logarithmic_Rate_Effects_Scale_FactorD_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.D));
        //fct_ID1
        QComboBox* comBobox = this->findChild<QComboBox*>("Function_Identifier_1_Box");
        if (!comBobox)return;
        int index = comBobox->findData(_objDof.fct_ID1);
        comBobox->setCurrentIndex(index);
        //H
        comBobox = this->findChild<QComboBox*>("Spring_Hardening_Flag_Box");
        if (!comBobox)return;
        index = comBobox->findData(_objDof.H);
        comBobox->setCurrentIndex(index);
        //fct_ID2
        comBobox = this->findChild<QComboBox*>("Function_Identifier_2_Box");
        if (!comBobox)return;
        index = comBobox->findData(_objDof.fct_ID2);
        comBobox->setCurrentIndex(index);
        //fct_ID3
        comBobox = this->findChild<QComboBox*>("Function_Identifier_3_Box");
        if (!comBobox)return;
        index = comBobox->findData(_objDof.fct_ID3);
        comBobox->setCurrentIndex(index);
        //fct_ID4
        comBobox = this->findChild<QComboBox*>("Function_Identifier_4_Box");
        if (!comBobox)return;
        index = comBobox->findData(_objDof.fct_ID4);
        comBobox->setCurrentIndex(index);
        //min
        edit = this->findChild<QLineEdit*>("Dmin_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.min));
        //max
        edit = this->findChild<QLineEdit*>("Dmax_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.max));
        //F
        edit = this->findChild<QLineEdit*>("Abscissa_Scale_Factor_Damping_Functions_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.F));
        //E
        edit = this->findChild<QLineEdit*>("Ordinate_Scale_Factor_Damping_FunctionE_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.E));
        //Ascale
        edit = this->findChild<QLineEdit*>("Abscissa_Scale_Factor_Stiffness_Function_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.Ascale));
        //Hscale
        edit = this->findChild<QLineEdit*>("Ordinate_Scale_Factor_Damping_FunctionH_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.Hscale));
        //vc
        edit = this->findChild<QLineEdit*>("Relative_Velocity_Coefficient_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.vc));
        //n
        edit = this->findChild<QLineEdit*>("Relative_Velocity_Exponent_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.n));
        //a
        edit = this->findChild<QLineEdit*>("alpha_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.a));
        //e
        edit = this->findChild<QLineEdit*>("beta_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(_objDof.e));

    }

    bool GUIPropertyBeamSPR_DOF_Dialog::ckeckData()
    {
        return true;
    }

    void GUIPropertyBeamSPR_DOF_Dialog::getDataFormWidget()
    {
        //K
        QLineEdit* edit = this->findChild<QLineEdit*>("Stiffness_LineEdit");
        if (!edit)return;
        _objDof.K = edit->text().toDouble();
        //C
        edit = this->findChild<QLineEdit*>("Damping_LineEdit");
        if (!edit)return;
        _objDof.C = edit->text().toDouble();
        //A
        edit = this->findChild<QLineEdit*>("Nonlinear_Stiffness_Scale_Factor_LineEdit");
        if (!edit)return;
        _objDof.A = edit->text().toDouble();
        //B
        edit = this->findChild<QLineEdit*>("Logarithmic_Rate_Effects_Scale_FactorB_LineEdit");
        if (!edit)return;
        _objDof.B = edit->text().toDouble();
        //D
        edit = this->findChild<QLineEdit*>("Logarithmic_Rate_Effects_Scale_FactorD_LineEdit");
        if (!edit)return;
        _objDof.D = edit->text().toDouble();
        //fct_ID1
        QComboBox* comBobox = this->findChild<QComboBox*>("Function_Identifier_1_Box");
        if (!comBobox)return;
        _objDof.fct_ID1 = comBobox->currentData().toInt();
        //H
        comBobox = this->findChild<QComboBox*>("Spring_Hardening_Flag_Box");
        if (!comBobox)return;
        _objDof.H = comBobox->currentData().toInt();
        //fct_ID2
        comBobox = this->findChild<QComboBox*>("Function_Identifier_2_Box");
        if (!comBobox)return;
        _objDof.fct_ID2 = comBobox->currentData().toInt();
        //fct_ID3
        comBobox = this->findChild<QComboBox*>("Function_Identifier_3_Box");
        if (!comBobox)return;
        _objDof.fct_ID3 = comBobox->currentData().toInt();
        //fct_ID4
        comBobox = this->findChild<QComboBox*>("Function_Identifier_4_Box");
        if (!comBobox)return;
        _objDof.fct_ID4 = comBobox->currentData().toInt();
        //min
        edit = this->findChild<QLineEdit*>("Dmin_LineEdit");
        if (!edit)return;
        _objDof.min = edit->text().toDouble();
        //max
        edit = this->findChild<QLineEdit*>("Dmax_LineEdit");
        if (!edit)return;
        _objDof.max = edit->text().toDouble();
        //F
        edit = this->findChild<QLineEdit*>("Abscissa_Scale_Factor_Damping_Functions_LineEdit");
        if (!edit)return;
        _objDof.F = edit->text().toDouble();
        //E
        edit = this->findChild<QLineEdit*>("Ordinate_Scale_Factor_Damping_FunctionE_LineEdit");
        if (!edit)return;
        _objDof.E = edit->text().toDouble();
        //Ascale
        edit = this->findChild<QLineEdit*>("Abscissa_Scale_Factor_Stiffness_Function_LineEdit");
        if (!edit)return;
        _objDof.Ascale = edit->text().toDouble();
        //Hscale
        edit = this->findChild<QLineEdit*>("Ordinate_Scale_Factor_Damping_FunctionH_LineEdit");
        if (!edit)return;
        _objDof.Hscale = edit->text().toDouble();
        //vc
        edit = this->findChild<QLineEdit*>("Relative_Velocity_Coefficient_LineEdit");
        if (!edit)return;
        _objDof.vc = edit->text().toDouble();
        //n
        edit = this->findChild<QLineEdit*>("Relative_Velocity_Exponent_LineEdit");
        if (!edit)return;
        _objDof.n = edit->text().toDouble();
        //a
        edit = this->findChild<QLineEdit*>("alpha_LineEdit");
        if (!edit)return;
        _objDof.a = edit->text().toDouble();
        //e
        edit = this->findChild<QLineEdit*>("beta_LineEdit");
        if (!edit)return;
        _objDof.e = edit->text().toDouble();
            
    }

    QComboBox* GUIPropertyBeamSPR_DOF_Dialog::addComboBox(const QString& label, const QString& objname, bool isCurve)
    {
        QComboBox* comBobox = GUIPropertyDialog::addComboBox(label, objname);
        if (!isCurve)
            return comBobox;
        //获取非线性曲线管理器
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (!caseObj) return nullptr;
        Radioss::FITKNonLinerCurveManager* curveMgr = caseObj->getNonLinerCurveManager();
        int curNum = curveMgr->getDataCount();

        comBobox->addItem(tr("None"), 0);
        for (int i = 0;i < curNum;++i) {
            Radioss::FITKNonLinerCurve* curve = curveMgr->getDataByIndex(i);
            if (!curve) continue;
            comBobox->addItem(curve->getDataObjectName(), curve->getDataObjectID());
        }
        return comBobox;
    }
    void GUIPropertyBeamSPR_DOF_Dialog::on_pushButtonCancel_clicked()
    {
        this->reject();
    }
    void GUIPropertyBeamSPR_DOF_Dialog::on_pushButtonOK_clicked()
    {
        if (!ckeckData())return;
        getDataFormWidget();
        this->accept();
    }
}