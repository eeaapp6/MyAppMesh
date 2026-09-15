/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPropertySandwichDialog.h"
#include "GUIPropertySandwitchLayerDialog.h"
#include "ui_GUIPropertyDialog.h"
#include "FITK_Component/FITKRadiossData/FITKPropSandwichShell.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <QComboBox>
#include <QMessageBox>

namespace GUI
{
    namespace
    {
        QString getSandwichMaterialName(int materialId)
        {
            Radioss::FITKRadiossCase* caseData =
                FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
            if (caseData == nullptr)
            {
                return QString();
            }

            Interface::FITKMaterialManager* materialManager = caseData->getMaterialManager();
            if (materialManager == nullptr)
            {
                return QString();
            }

            Interface::FITKAbstractMaterial* material = materialManager->getDataByID(materialId);
            if (material == nullptr)
            {
                return QString();
            }

            return material->getDataObjectName();
        }
    }

    GUI::GUIPropertySandwichDialog::GUIPropertySandwichDialog(Core::FITKActionOperator * oper, Radioss::FITKPropAbstract * obj, QWidget * parent)
        : GUIPropertyDialog(oper, obj, parent)
    {
        _obj = dynamic_cast<Radioss::FITKPropSandwichShell*>(obj);
        if (_obj)
            _isEditMode = true;
        else
            _obj = new Radioss::FITKPropSandwichShell();
        // 初始化
        this->initUI();
        this->init();
    }
    void GUIPropertySandwichDialog::initUI()
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
        _ui->lineEditType->setText("SH_SANDW");
        _ui->lineEditType->setReadOnly(true);

        //厚度
        this->addLineEdit("Thickness", "Thickness_LineEdit");

        //壳元素公式标志位(I shell)
        QComboBox* comBobox = addComboBox("Quad_Element_Formulation", "Quad_Element_Formulation_Box");
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
        comBobox->addItem(tr("Old small strain"), 3);
        comBobox->addItem(tr("Full geometric nonlinearities"), 4);

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

        //层数(N) 范围(1,100)
        int row = _ui->tableWidgetValue->rowCount();
        _ui->tableWidgetValue->insertRow(row);
        QTableWidgetItem* item = new QTableWidgetItem("Number_Of_Layers");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        _ui->tableWidgetValue->setItem(row, 0, item);

        QWidget* widgetNum = new QWidget(this);
        QHBoxLayout* layoutNum = new QHBoxLayout(widgetNum);
        QPushButton* buttonNum = new QPushButton("Layer Settings");
        QLineEdit* lineEditNum = new QLineEdit(this);
        lineEditNum->setObjectName("Number_Of_Layers_LineEdit");
        lineEditNum->setText("0");
        lineEditNum->setReadOnly(true);
        layoutNum->addWidget(lineEditNum);
        layoutNum->addWidget(buttonNum);
        layoutNum->setContentsMargins(0, 0, 0, 0);
        layoutNum->setSpacing(0);
        _ui->tableWidgetValue->setCellWidget(row, 1, widgetNum);
        connect(buttonNum, &QPushButton::clicked, this, &GUIPropertySandwichDialog::on_pushButtonLayerSettings_clicked);

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

        //X component for reference vector(m_vx)
        this->addLineEdit("Reference_Vector_X", "Reference_Vector_X_LineEdit");

        //Y component for reference vector(m_vy)
        this->addLineEdit("Reference_Vector_Y", "Reference_Vector_Y_LineEdit");

        //Z component for reference vector(m_vz)
        this->addLineEdit("Reference_Vector_Z", "Reference_Vector_Z_LineEdit");

        //Skew identifier for reference vector(m_skew_id)
        comBobox = addComboBox("Skew_ID", "Skew_ID_Box");
        comBobox->addItem(tr("Global"), 0);

        //Orthotropic system formulation flag for reference vector(m_Iorth)
        comBobox = addComboBox("Orthotropic_System_Flag", "Orthotropic_System_Flag_Box");
        comBobox->addItem(tr("Default"), 0);
        comBobox->addItem(tr("The first orthotropy direction is constant with respect to a non-orthonormal system of deformed element."), 1);

        //Layer positioning flag for reference vector(m_Ipos)
        comBobox = addComboBox("Layer_Positioning_Flag", "Layer_Positioning_Flag_Box");
        comBobox->addItem(tr("Default"), 0);
        comBobox->addItem(tr("User defined"), 1);

        //Reference direction in shell plane(m_Ip)
        //comBobox = addComboBox("Reference_Direction_In_Shell_Plane", "Reference_Direction_In_Shell_Plane_Box");
        //comBobox->addItem(tr("Default"), 0);
        //comBobox->addItem(tr("N1,N2"), 20);
        //comBobox->addItem(tr("Vector V is ignored"), 22);
        //comBobox->addItem(tr("Skew_ID is ignored"), 23);

        //调整宽度
        _ui->tableWidgetValue->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    void GUIPropertySandwichDialog::init()
    {
        Radioss::FITKPropSandwichShell* obj = dynamic_cast<Radioss::FITKPropSandwichShell*>(_obj);
        if (!obj)return;
        Radioss::FITKPropSandwichShellValue* value = obj->getValue();
        if (!value)return;
        if (_isEditMode) {
            _ui->lineEditName->setText(obj->getDataObjectName());
        }
        _ui->lineEditPropertyID->setText(obj->GetFITKPropSandwichShellRadiossKeyWord());
        //厚度
        QLineEdit* edit = this->findChild<QLineEdit*>("Thickness_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getThick()));

        //壳元素公式标志位(I shell)
        QComboBox* comBobox = this->findChild<QComboBox*>("Quad_Element_Formulation_Box");
        if (!comBobox)return;
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

        //通过厚度积分点失效比率(P_thickfail)，输入范围0到1
        edit = this->findChild<QLineEdit*>("P_Thick_Fail_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getPThickfail()));

        //壳沙漏能(hm)，Default = 0.01 if Ishell =1, 2, 4.  Default = 0.1 if Ishell = 3
        edit = this->findChild<QLineEdit*>("Membrane_Hourglass_Coefficient_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getHm()));

        //平面外沙漏能(hf) ，Default = 0.01 if Ishell =1, 2, 3，4
        edit = this->findChild<QLineEdit*>("Out_Of_Plane_Hourglass_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getHf()));

        //旋转沙漏系数(hr),Default = 0.01 if Ishell =1, 2, 4.  Default = 0.1 if Ishell = 3
        edit = this->findChild<QLineEdit*>("Rotation_Hourglass_Coefficient_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getHr()));

        //薄膜阻尼(dm)
        edit = this->findChild<QLineEdit*>("Membrane_Damping_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getDm()));

        //数值阻尼(dn),Default =0.015 if Ishell =24 (QEPH),Default =0.001 if Ishell =12 (QBAT)Defa, Default =0.0001 for Ish3n =30 (DKT18)
        edit = this->findChild<QLineEdit*>("Numerical_Damping_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getDn()));

        //层数(N) 范围(1,100)
        edit = this->findChild<QLineEdit*>("Number_Of_Layers_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getN()));

        //剪切因子(A shear)
        edit = this->findChild<QLineEdit*>("Shear_Factor_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getAshear()));

        //resultant压力计算（I think）
        comBobox = this->findChild<QComboBox*>("Stress_Resultant_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIpos());
        comBobox->setCurrentIndex(index);

        //平面应力塑性(I plas)
        comBobox = this->findChild<QComboBox*>("Plane_Stress_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIplas());
        comBobox->setCurrentIndex(index);

        //X component for reference vector(m_vx)
        edit = this->findChild<QLineEdit*>("Reference_Vector_X_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getVx()));
        //Y component for reference vector(m_vy)
        edit = this->findChild<QLineEdit*>("Reference_Vector_Y_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getVy()));
        //Z component for reference vector(m_vz)
        edit = this->findChild<QLineEdit*>("Reference_Vector_Z_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getVz()));
        //Skew identifier for reference vector(m_skew_id)
        comBobox = this->findChild<QComboBox*>("Skew_ID_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getSkewID());
        comBobox->setCurrentIndex(index);
        //Orthotropic system formulation flag for reference vector(m_Iorth)
        comBobox = this->findChild<QComboBox*>("Orthotropic_System_Flag_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIorth());
        comBobox->setCurrentIndex(index);
        //Layer positioning flag for reference vector(m_Ipos)
        comBobox = this->findChild<QComboBox*>("Layer_Positioning_Flag_Box");
        if (!comBobox)return;
        index = comBobox->findData(value->getIpos());
        comBobox->setCurrentIndex(index);


    }
    bool GUIPropertySandwichDialog::ckeckData()
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
        QLineEdit* edit = this->findChild<QLineEdit*>("Number_Of_Layers_LineEdit");
        if (!edit)return false;
        int nLayers = edit->text().toInt();
        if (nLayers < 1 || nLayers > 100) {
            QMessageBox::warning(this, "", tr("Number of layers must be in the range (1,100)."), QMessageBox::Ok);
            return false;
        }
        return true;
    }
    void GUIPropertySandwichDialog::getDataFormWidget()
    {
        Radioss::FITKPropSandwichShell* obj = dynamic_cast<Radioss::FITKPropSandwichShell*>(_obj);
        if (!obj)return;
        Radioss::FITKPropSandwichShellValue* value = obj->getValue();
        if (!value)return;
        //厚度
        QLineEdit* edit = this->findChild<QLineEdit*>("Thickness_LineEdit");
        if (!edit)return;
        value->setThick(edit->text().toDouble());

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

        //通过厚度积分点失效比率(P_thickfail)，输入范围0到1
        edit = this->findChild<QLineEdit*>("P_Thick_Fail_LineEdit");
        if (!edit)return;
        value->setPThickfail(edit->text().toDouble());

        //壳沙漏能(hm)，Default = 0.01 if Ishell =1, 2, 4.  Default = 0.1 if Ishell = 3
        edit = this->findChild<QLineEdit*>("Membrane_Hourglass_Coefficient_LineEdit");
        if (!edit)return;
        value->setHm(edit->text().toDouble());

        //平面外沙漏能(hf) ，Default = 0.01 if Ishell =1, 2, 3，4
        edit = this->findChild<QLineEdit*>("Out_Of_Plane_Hourglass_LineEdit");
        if (!edit)return;
        value->setHf(edit->text().toDouble());

        //旋转沙漏系数(hr),Default = 0.01 if Ishell =1, 2, 4.  Default = 0.1 if Ishell = 3
        edit = this->findChild<QLineEdit*>("Rotation_Hourglass_Coefficient_LineEdit");
        if (!edit)return;
        value->setHr(edit->text().toDouble());

        //薄膜阻尼(dm)
        edit = this->findChild<QLineEdit*>("Membrane_Damping_LineEdit");
        if (!edit)return;
        value->setDm(edit->text().toDouble());

        //数值阻尼(dn),Default =0.015 if Ishell =24 (QEPH),Default =0.001 if Ishell =12 (QBAT)Defa, Default =0.0001 for Ish3n =30 (DKT18)
        edit = this->findChild<QLineEdit*>("Numerical_Damping_LineEdit");
        if (!edit)return;
        value->setDn(edit->text().toDouble());



        //层数(N) 范围(1,100)  to do 设置层信息界面



        //剪切因子(A shear)
        edit = this->findChild<QLineEdit*>("Shear_Factor_LineEdit");
        if (!edit)return;
        value->setAshear(edit->text().toDouble());

        //resultant压力计算（I think）
        comBobox = this->findChild<QComboBox*>("Stress_Resultant_Box");
        if (!comBobox)return;
        value->setIthick(comBobox->currentData().toInt());

        //平面应力塑性(I plas)
        comBobox = this->findChild<QComboBox*>("Plane_Stress_Box");
        if (!comBobox)return;
        value->setIplas(comBobox->currentData().toInt());

        //X component for reference vector(m_vx)
        edit = this->findChild<QLineEdit*>("Reference_Vector_X_LineEdit");
        if (!edit)return;
        value->setVx(edit->text().toDouble());
        //Y component for reference vector(m_vy)
        edit = this->findChild<QLineEdit*>("Reference_Vector_Y_LineEdit");
        if (!edit)return;
        value->setVy(edit->text().toDouble());
        //Z component for reference vector(m_vz)
        edit = this->findChild<QLineEdit*>("Reference_Vector_Z_LineEdit");
        if (!edit)return;
        value->setVz(edit->text().toDouble());
        //Skew identifier for reference vector(m_skew_id)
        comBobox = this->findChild<QComboBox*>("Skew_ID_Box");
        if (!comBobox)return;
        value->setSkewID(comBobox->currentData().toInt());
        //Orthotropic system formulation flag for reference vector(m_Iorth)
        comBobox = this->findChild<QComboBox*>("Orthotropic_System_Flag_Box");
        if (!comBobox)return;
        value->setIorth(comBobox->currentData().toInt());
        //Layer positioning flag for reference vector(m_Ipos)
        comBobox = this->findChild<QComboBox*>("Layer_Positioning_Flag_Box");
        if (!comBobox)return;
        value->setIpos(comBobox->currentData().toInt());

    }

    void GUIPropertySandwichDialog::on_pushButtonLayerSettings_clicked()
    {
        Radioss::FITKPropSandwichShell* obj = dynamic_cast<Radioss::FITKPropSandwichShell*>(_obj);
        if (!obj)return;
        GUI::GUIPropertySandwitchLayerDialog dlg(obj, this);
        dlg.exec();
        //刷新层数显示
        Radioss::FITKPropSandwichShellValue* value = obj->getValue();
        if (!value)return;
        QLineEdit* edit = this->findChild<QLineEdit*>("Number_Of_Layers_LineEdit");
        if (!edit)return;
        edit->setText(QString::number(value->getN()));
    }

    void GUIPropertySandwichDialog::writePythonScript()
    {
        Radioss::FITKPropSandwichShell* obj = dynamic_cast<Radioss::FITKPropSandwichShell*>(_obj);
        if (obj == nullptr)
        {
            return;
        }

        Radioss::FITKPropSandwichShellValue* value = obj->getValue();
        if (value == nullptr)
        {
            return;
        }

        QStringList script;
        if (_isEditMode)
        {
            QString oldName = _oldNameForScript.isEmpty() ? obj->getDataObjectName() : _oldNameForScript;
            script.append(QString("p=Property.SandwichShell.GetSandwichShell('%1')").arg(oldName));
            if (oldName != obj->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(obj->getDataObjectName()));
            }
        }
        else
        {
            script.append("p=Property.SandwichShell()");
            script.append(QString("p.createProperty('%1')").arg(obj->getDataObjectName()));
        }

        script.append(QString("p.setThick(%1)").arg(value->getThick()));
        script.append(QString("p.setIshell(%1)").arg(value->getIshell()));
        script.append(QString("p.setIsmstr(%1)").arg(value->getIsmstr()));
        script.append(QString("p.setIsh3n(%1)").arg(value->getIsh3n()));
        script.append(QString("p.setIdril(%1)").arg(value->getIdril()));
        script.append(QString("p.setPthickFail(%1)").arg(value->getPThickfail()));
        script.append(QString("p.setHm(%1)").arg(value->getHm()));
        script.append(QString("p.setHf(%1)").arg(value->getHf()));
        script.append(QString("p.setHr(%1)").arg(value->getHr()));
        script.append(QString("p.setDm(%1)").arg(value->getDm()));
        script.append(QString("p.setDn(%1)").arg(value->getDn()));
        script.append(QString("p.setNumOfLayers(%1)").arg(value->getN()));
        script.append(QString("p.setIstrain(%1)").arg(value->getIstrain()));
        script.append(QString("p.setAshear(%1)").arg(value->getAshear()));
        script.append(QString("p.setIthick(%1)").arg(value->getIthick()));
        script.append(QString("p.setIplas(%1)").arg(value->getIplas()));
        script.append(QString("p.setVx(%1)").arg(value->getVx()));
        script.append(QString("p.setVy(%1)").arg(value->getVy()));
        script.append(QString("p.setVz(%1)").arg(value->getVz()));
        script.append(QString("p.setSkewID(%1)").arg(value->getSkewID()));
        script.append(QString("p.setIorth(%1)").arg(value->getIorth()));
        script.append(QString("p.setIpos(%1)").arg(value->getIpos() ? 1 : 0));

        int layerCount = value->getN();
        for (int i = 0; i < layerCount; ++i)
        {
            Radioss::SandwichShellLayerValue layerValue = value->getLayerValue(i);
            int layerIndex = i + 1;
            script.append(QString("p.setLayerAngle(%1, %2)").arg(layerIndex).arg(layerValue._angle));
            script.append(
                QString("p.setLayerThickness(%1, %2)").arg(layerIndex).arg(layerValue._thickness));
            script.append(
                QString("p.setLayerPositionZ(%1, %2)").arg(layerIndex).arg(layerValue._positionZ));

            QString materialName = getSandwichMaterialName(layerValue._matID);
            if (!materialName.isEmpty())
            {
                script.append(
                    QString("p.setLayerMaterial(%1, '%2')").arg(layerIndex).arg(materialName));
            }

            script.append(
                QString("p.setLayerFailureWeight(%1, %2)").arg(layerIndex).arg(layerValue._relFWeight));
        }

        this->saveScript(script);
    }
}