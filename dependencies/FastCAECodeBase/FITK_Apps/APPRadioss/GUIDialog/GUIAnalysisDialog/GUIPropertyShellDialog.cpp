/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPropertyShellDialog.h"
#include "ui_GUIPropertyDialog.h"
#include "FITK_Component/FITKRadiossData/FITKPropShell.h"

#include <QComboBox>
#include <QMessageBox>

namespace GUI
{
    GUI::GUIPropertyShellDialog::GUIPropertyShellDialog(Core::FITKActionOperator * oper, Radioss::FITKPropAbstract* obj, QWidget * parent)
        : GUIPropertyDialog(oper, obj, parent)
    {
        _obj = dynamic_cast<Radioss::FITKPropShell*>(obj);
        if (_obj)  
            _isEditMode = true;
        else 
            _obj = new Radioss::FITKPropShell();

        // 初始化
        this->initUI();
        this->init();
    }

    void GUIPropertyShellDialog::initUI()
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
        _ui->lineEditType->setText("SHELL");
        _ui->lineEditType->setReadOnly(true);

        //厚度
        this->addLineEdit("Thickness", "Thickness_LineEdit");

        //壳元素公式标志位(I shell)
        QComboBox* comBobox = addComboBox("Quad_Element_Formulation","Quad_Element_Formulation_Box");
        comBobox->addItem(tr("Default"), 0);//Use value in DEF_SHELL
        comBobox->addItem(tr("Belytschko"), 1);//Default, if DEF_SHELL is not defined,Belytschko
        comBobox->addItem(tr("Hallquist"), 2);
        comBobox->addItem(tr("Elasto-Plastic-Orthogonality"), 3);
        comBobox->addItem(tr("warped elements"), 4);
        comBobox->addItem(tr("QBAT"), 12);
        comBobox->addItem(tr("QEPH"), 24);

        //壳应变公式标志位(I smstr)
        comBobox = addComboBox("Strain_Formulation", "Strain_Formulation_Box");
        comBobox->addItem(tr("None"), -1);//Automatically set the best value according to element typeand material law.
        comBobox->addItem(tr("Default"), 0);//Use value in DEF_SHELL
        comBobox->addItem(tr("Small strain-time = 0"), 1);
        comBobox->addItem(tr("Full geometric with strain"), 2);
        comBobox->addItem(tr("Old small strain"),3);
        comBobox->addItem(tr("Full geometric nonlinearities"), 4);
        comBobox->addItem(tr("Lagrange"), 10);

        //三节点壳元素公式标志位(I sh3n)
        comBobox = addComboBox("Shell_Element_Formulation", "Shell_Element_Formulation_Box");
        comBobox->addItem(tr("Default"), 0);//Use value in DEF_SHELL
        comBobox->addItem(tr("Standard triangle-C0"), 1);
        comBobox->addItem(tr("Standard triangle-rotation"), 2);
        comBobox->addItem(tr("DKT18"), 30);
        comBobox->addItem(tr("DKT_S3"), 31);

        //钻削自由度刚性标志(I dril)
        comBobox = addComboBox("Drill_DOF_Stiffness", "Drill_DOF_Stiffness_Box");
        comBobox->addItem(tr("None"), 0);//Use value in DEF_SHELL
        comBobox->addItem(tr("Yes"), 1);
        comBobox->addItem(tr("No"), 2);//Default

        //收缩自由度标志(I pinch)，仅支持 QBAT
        comBobox = addComboBox("Pinching_DOF", "Pinching_DOF_Box");
        comBobox->addItem(tr("Yes"), 0);//启用
        comBobox->addItem(tr("No"), 1);//禁用

        //通过厚度积分点失效比率(P_thickfail)，输入范围0到1
        this->addLineEdit("P_Thick_Fail", "P_Thick_Fail_LineEdit");

        //壳沙漏能(hm)，Default = 0.01 if Ishell =1, 2, 4.  Default = 0.1 if Ishell = 3
        this->addLineEdit("Membrane_Hourglass_Coefficient", "Membrane_Hourglass_Coefficient_LineEdit");

        //平面外沙漏能(hf) ，Default = 0.01 if Ishell =1, 2, 3，4
        this->addLineEdit("Out_Of_Plane_Hourglass", "Out_Of_Plane_Hourglass_LineEdit");

        //旋转沙漏系数(hr),Default = 0.01 if Ishell =1, 2, 4.  Default = 0.1 if Ishell = 3
        this->addLineEdit("Rotation_Hourglass_Coefficient", "Rotation_Hourglass_Coefficient_LineEdit");

        //薄膜阻尼(dm)
        this->addLineEdit("Membrane_Damping", "Membrane_Damping_LineEdit");

        //数值阻尼(dn),Default =0.015 if Ishell =24 (QEPH),Default =0.001 if Ishell =12 (QBAT)Defa, Default =0.0001 for Ish3n =30 (DKT18)
        this->addLineEdit("Numerical_Damping", "Numerical_Damping_LineEdit");

        //积分点数(N) 范围(0,10)
        this->addLineEdit("Integration_Points", "Integration_Points_LineEdit");

        //剪切因子(A shear)
        this->addLineEdit("Shear_Factor", "Shear_Factor_LineEdit");

        //resultant压力计算（I think）
        comBobox = addComboBox("Stress_Resultant", "Stress_Resultant_Box");
        comBobox->addItem(tr("None"), -1);//Automatically set the best value according to element typeand material law.
        comBobox->addItem(tr("Default"), 0);//Use value in DEF_SHELL
        comBobox->addItem(tr("Dynamic"), 1);
        comBobox->addItem(tr("Constant"), 2);

        //平面应力塑性(I plas)
        comBobox = addComboBox("Plane_Stress", "Plane_Stress_Box");
        comBobox->addItem(tr("None"), -1);//Automatically set the best value according to element typeand material law.
        comBobox->addItem(tr("Default"), 0);//Use value in DEF_SHELL
        comBobox->addItem(tr("Newton iteration"), 1);
        comBobox->addItem(tr("Radial return"), 2);

        //调整宽度
        _ui->tableWidgetValue->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    void GUIPropertyShellDialog::init()
    {
        Radioss::FITKPropShell* obj = dynamic_cast<Radioss::FITKPropShell*>(_obj);
        if (!obj)return;
        Radioss::FITKPropShellValue* value = obj->getValue();
        if (!value)return;
        if (_isEditMode) {
            _ui->lineEditName->setText(obj->getDataObjectName());
        }
        _ui->lineEditPropertyID->setText(obj->GetFITKPropShellRadiossKeyWord());

        //厚度
        QLineEdit* edit=this->findChild<QLineEdit*>("Thickness_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(obj->getThick()));

        //壳元素公式标志位(I shell)
        QComboBox* comBobox = this->findChild<QComboBox*>("Quad_Element_Formulation_Box");
        if(!comBobox)return;
        int index = comBobox->findData(value->getIshell());
        comBobox->setCurrentIndex(index);

        //壳应变公式标志位(I smstr)
        comBobox = this->findChild<QComboBox*>("Strain_Formulation_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIsmstr());
        comBobox->setCurrentIndex(index);

        //三节点壳元素公式标志位(I sh3n)
        comBobox = this->findChild<QComboBox*>("Shell_Element_Formulation_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIsh3n());
        comBobox->setCurrentIndex(index);

        //钻削自由度刚性标志(I dril)
        comBobox = this->findChild<QComboBox*>("Drill_DOF_Stiffness_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIdril());
        comBobox->setCurrentIndex(index);

        //收缩自由度标志(I pinch)，仅支持 QBAT
        comBobox = this->findChild<QComboBox*>("Pinching_DOF_Box");
        if (!comBobox)return;
        index = comBobox->findData(obj->getIpinch());
        comBobox->setCurrentIndex(index);

        //通过厚度积分点失效比率(P_thickfail)，输入范围0到1
        edit = this->findChild<QLineEdit*>("P_Thick_Fail_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(obj->getPthickFail()));

        //壳沙漏能(hm)，Default = 0.01 if Ishell =1, 2, 4.  Default = 0.1 if Ishell = 3
        edit = this->findChild<QLineEdit*>("Membrane_Hourglass_Coefficient_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(obj->getHm()));

        //平面外沙漏能(hf) ，Default = 0.01 if Ishell =1, 2, 3，4
        edit = this->findChild<QLineEdit*>("Out_Of_Plane_Hourglass_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(obj->getHf()));

        //旋转沙漏系数(hr),Default = 0.01 if Ishell =1, 2, 4.  Default = 0.1 if Ishell = 3
        edit = this->findChild<QLineEdit*>("Rotation_Hourglass_Coefficient_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(obj->getHr()));

        //薄膜阻尼(dm)
        edit = this->findChild<QLineEdit*>("Membrane_Damping_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(obj->getDm()));

        //数值阻尼(dn),Default =0.015 if Ishell =24 (QEPH),Default =0.001 if Ishell =12 (QBAT)Defa, Default =0.0001 for Ish3n =30 (DKT18)
        edit = this->findChild<QLineEdit*>("Numerical_Damping_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(obj->getDn()));

        //积分点数(N) 范围(0,10)
        edit = this->findChild<QLineEdit*>("Integration_Points_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(obj->getN()));

        //剪切因子(A shear)
        edit = this->findChild<QLineEdit*>("Shear_Factor_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(obj->getAshear()));

        //resultant压力计算（I think）
        comBobox = this->findChild<QComboBox*>("Stress_Resultant_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIthick());
        comBobox->setCurrentIndex(index);

        //平面应力塑性(I plas)
        comBobox = this->findChild<QComboBox*>("Plane_Stress_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIplas());
        comBobox->setCurrentIndex(index);
    }

    bool GUIPropertyShellDialog::ckeckData()
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

    void GUIPropertyShellDialog::getDataFormWidget()
    {
        Radioss::FITKPropShell* obj = dynamic_cast<Radioss::FITKPropShell*>(_obj);
        if (!obj)return;
        Radioss::FITKPropShellValue* value = obj->getValue();
        if (!value)return;

        //厚度
        QLineEdit* edit = this->findChild<QLineEdit*>("Thickness_LineEdit");
        if (!edit)return;
        obj->setThick(edit->text().toDouble());

        //壳元素公式标志位(I shell)
        QComboBox* comBobox = this->findChild<QComboBox*>("Quad_Element_Formulation_Box");
        if (!comBobox)return;
        value->setIshell(comBobox->currentData().toInt());

        //壳应变公式标志位(I smstr)
        comBobox = this->findChild<QComboBox*>("Strain_Formulation_Box");
        if (!comBobox)return;
        value->setIsmstr(comBobox->currentData().toInt());

        //三节点壳元素公式标志位(I sh3n)
        comBobox = this->findChild<QComboBox*>("Shell_Element_Formulation_Box");
        if (!comBobox)return;
        value->setIsh3n(comBobox->currentData().toInt());

        //钻削自由度刚性标志(I dril)
        comBobox = this->findChild<QComboBox*>("Drill_DOF_Stiffness_Box");
        if (!comBobox)return;
        value->setIdril(comBobox->currentData().toInt());

        //收缩自由度标志(I pinch)，仅支持 QBAT
        comBobox = this->findChild<QComboBox*>("Pinching_DOF_Box");
        if (!comBobox)return;
        obj->setIpinch(comBobox->currentData().toInt());

        //通过厚度积分点失效比率(P_thickfail)，输入范围0到1
        edit = this->findChild<QLineEdit*>("P_Thick_Fail_LineEdit");
        if (!edit)return;
        obj->setPthickFail(edit->text().toDouble());

        //壳沙漏能(hm)，Default = 0.01 if Ishell =1, 2, 4.  Default = 0.1 if Ishell = 3
        edit = this->findChild<QLineEdit*>("Membrane_Hourglass_Coefficient_LineEdit");
        if (!edit)return;
        obj->setHm(edit->text().toDouble());

        //平面外沙漏能(hf) ，Default = 0.01 if Ishell =1, 2, 3，4
        edit = this->findChild<QLineEdit*>("Out_Of_Plane_Hourglass_LineEdit");
        if (!edit)return;
        obj->setHf(edit->text().toDouble());

        //旋转沙漏系数(hr),Default = 0.01 if Ishell =1, 2, 4.  Default = 0.1 if Ishell = 3
        edit = this->findChild<QLineEdit*>("Rotation_Hourglass_Coefficient_LineEdit");
        if (!edit)return;
        obj->setHr(edit->text().toDouble());

        //薄膜阻尼(dm)
        edit = this->findChild<QLineEdit*>("Membrane_Damping_LineEdit");
        if (!edit)return;
        obj->setDm(edit->text().toDouble());

        //数值阻尼(dn),Default =0.015 if Ishell =24 (QEPH),Default =0.001 if Ishell =12 (QBAT)Defa, Default =0.0001 for Ish3n =30 (DKT18)
        edit = this->findChild<QLineEdit*>("Numerical_Damping_LineEdit");
        if (!edit)return;
        obj->setDn(edit->text().toDouble());

        //积分点数(N) 范围(0,10)
        edit = this->findChild<QLineEdit*>("Integration_Points_LineEdit");
        if (!edit)return;
        obj->setN(edit->text().toDouble());

        //剪切因子(A shear)
        edit = this->findChild<QLineEdit*>("Shear_Factor_LineEdit");
        if (!edit)return;
        obj->setAshear(edit->text().toDouble());

        //resultant压力计算（I think）
        comBobox = this->findChild<QComboBox*>("Stress_Resultant_Box");
        if (!comBobox)return;
        value->setIthick(comBobox->currentData().toInt());

        //平面应力塑性(I plas)
        comBobox = this->findChild<QComboBox*>("Plane_Stress_Box");
        if (!comBobox)return;
        value->setIplas(comBobox->currentData().toInt());
    }
    
    void GUIPropertyShellDialog::writePythonScript()
    {
        Radioss::FITKPropShell* obj = dynamic_cast<Radioss::FITKPropShell*>(_obj);
        if (obj == nullptr)
        {
            return;
        }

        Radioss::FITKPropShellValue* value = obj->getValue();
        if (value == nullptr)
        {
            return;
        }

        QStringList script;
        if (_isEditMode)
        {
            QString oldName = _oldNameForScript.isEmpty() ? obj->getDataObjectName() : _oldNameForScript;
            script.append(QString("p=Property.Shell.GetShell('%1')").arg(oldName));
            if (oldName != obj->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(obj->getDataObjectName()));
            }
        }
        else
        {
            script.append("p=Property.Shell()");
            script.append(QString("p.createProperty('%1')").arg(obj->getDataObjectName()));
        }

        script.append(QString("p.setThick(%1)").arg(obj->getThick()));
        script.append(QString("p.setIshell(%1)").arg(value->getIshell()));
        script.append(QString("p.setIsmstr(%1)").arg(value->getIsmstr()));
        script.append(QString("p.setIsh3n(%1)").arg(value->getIsh3n()));
        script.append(QString("p.setIdril(%1)").arg(value->getIdril()));
        script.append(QString("p.setIpinch(%1)").arg(obj->getIpinch()));
        script.append(QString("p.setPthickFail(%1)").arg(obj->getPthickFail()));
        script.append(QString("p.setHm(%1)").arg(obj->getHm()));
        script.append(QString("p.setHf(%1)").arg(obj->getHf()));
        script.append(QString("p.setHr(%1)").arg(obj->getHr()));
        script.append(QString("p.setDm(%1)").arg(obj->getDm()));
        script.append(QString("p.setDn(%1)").arg(obj->getDn()));
        script.append(QString("p.setN(%1)").arg(obj->getN()));
        script.append(QString("p.setAshear(%1)").arg(obj->getAshear()));
        script.append(QString("p.setIthick(%1)").arg(value->getIthick()));
        script.append(QString("p.setIplas(%1)").arg(value->getIplas()));

        this->saveScript(script);
    }
}