/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIMaterialHydroDialog.h"
#include "ui_GUIMaterialPropDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialHydro.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"

#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QComboBox>

namespace GUI
{
    GUIMaterialHydroDialog::GUIMaterialHydroDialog(Interface::FITKAbstractMaterial * obj, Core::FITKActionOperator * oper, QWidget * parent)
        : GUIMaterialPropDialog(obj, oper, parent)
    {
        QString MaterialName;
        _oper->argValue("MaterialName", MaterialName);
        if (!_obj) {
            _ui->lineEdit_name->setReadOnly(true);
            _obj = new Radioss::FITKMaterialHydro();
            _obj->setDataObjectName(MaterialName);
            setWindowTitle(tr("Create Material"));
        }
        else
        {
            setWindowTitle(tr("Edit Material"));
        }
        this->initUI();
        this->init();
    }
    void GUI::GUIMaterialHydroDialog::initUI()
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return;
        Radioss::FITKEquationOfStateManager* Mgr = dataCase->getEquationOfState();
        if (Mgr == nullptr) return;
        //添加根节点 Properties
        QTreeWidgetItem* commonProperties_item = new QTreeWidgetItem(_ui->treeWidget_parameter);
        commonProperties_item->setText(0, tr("Properties"));
        commonProperties_item->setExpanded(true);
        //添加 Properties子项材料密度
        this->addChiledItem(commonProperties_item, "density_lineEdit", tr("Initial Density"), "");
        //添加 Properties子项运动粘度
        this->addChiledItem(commonProperties_item, "kinematicViscosity_lineEdit", tr("Kinematic Viscosity"), "");
        //添加 Properties子项剪切压力
        this->addChiledItem(commonProperties_item, "pressureCut_lineEdit", tr("Pressure cut-off"), "");

        //添加根节点 Other
        QTreeWidgetItem* other_item = new QTreeWidgetItem(_ui->treeWidget_parameter);
        other_item->setText(0, tr("Other Properties"));
        other_item->setExpanded(true);
        //添加 Properties子项参考密度
        this->addChiledItem(other_item, "referenceDensity_lineEdit", tr("Reference Density"), "");

        //添加Other子项EOS ID
        QWidget* plastic_widget2 = new QWidget(this);
        QHBoxLayout* plastic_layout2 = new QHBoxLayout();
        QComboBox* eos_box = new QComboBox(this);
        for (int i = 0;i < Mgr->getDataCount();++i) 
        {
            Radioss::FITKAbstractEquationOfState* eos = dynamic_cast<Radioss::FITKAbstractEquationOfState*>(Mgr->getDataByIndex(i));
            if (!eos) continue;
            eos_box->addItem(eos->getDataObjectName(), eos->getDataObjectID());
        }
        eos_box->setObjectName("eos_box");
        QLabel* plastic_labe2 = new QLabel(this);
        plastic_labe2->setText("");
        plastic_layout2->addWidget(eos_box);
        plastic_layout2->addWidget(plastic_labe2);
        plastic_layout2->setStretch(0, 1);
        plastic_layout2->setStretch(1, 1);
        plastic_widget2->setLayout(plastic_layout2);
        QTreeWidgetItem* StrainRateCom_item = new QTreeWidgetItem(other_item);
        StrainRateCom_item->setText(0, tr("State Equation"));
        _ui->treeWidget_parameter->setItemWidget(StrainRateCom_item, 1, plastic_widget2);

        _ui->treeWidget_parameter->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    }
    void GUIMaterialHydroDialog::init()
    {
        Radioss::FITKMaterialHydro* hydro = dynamic_cast<Radioss::FITKMaterialHydro*>(_obj);
        if (!hydro) return;
        //通用初始化
        this->GUIMaterialPropDialog::init();

        Radioss::FITKMaterialHydroData hydroData = hydro->getMaterialDataReadOnly();
        _ui->lineEdit_matID->setReadOnly(true);
        _ui->lineEdit_model->setReadOnly(true);
        //设置名称
        _ui->lineEdit_name->setText(hydro->getDataObjectName());
        //设置材料ID
        _ui->lineEdit_matID->setText(hydro->GetFITKMaterialHydroRadiossKeyWord());
        //设置材料类型  流体粒子
        _ui->lineEdit_model->setText("HYDRO");
        //材料密度
        QLineEdit* density_lineEdit = this->findChild<QLineEdit*>("density_lineEdit");
        if (!density_lineEdit)return;
        density_lineEdit->setText(QString::number(hydroData.m_density));
        //参考密度
        QLineEdit* referenceDensity_lineEdit = this->findChild<QLineEdit*>("referenceDensity_lineEdit");
        if (!referenceDensity_lineEdit)return;
        referenceDensity_lineEdit->setText(QString::number(hydroData.m_R_density));
        //运动粘度
        QLineEdit* kinematicViscosity_lineEdit = this->findChild<QLineEdit*>("kinematicViscosity_lineEdit");
        if (!kinematicViscosity_lineEdit)return;
        kinematicViscosity_lineEdit->setText(QString::number(hydroData.m_kinematicViscosity));
        //剪切压力
        QLineEdit* pressureCut_lineEdit = this->findChild<QLineEdit*>("pressureCut_lineEdit");
        if (!pressureCut_lineEdit)return;
        pressureCut_lineEdit->setText(QString::number(hydroData.m_pressureCut));
        //EOS ID
        QComboBox* eos_box = this->findChild<QComboBox*>("eos_box");
        if (!eos_box)return;
        int index = eos_box->findData(hydro->getEOSID());
        eos_box->setCurrentIndex(index);
    }
    bool GUIMaterialHydroDialog::ckeckData()
    {
        //获取名称，查重处理
        QString name = _ui->lineEdit_name->text();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "", tr("Name cannot be empty."), QMessageBox::Ok);
            return false;
        }
        else if (_matMgr->getDataByName(name) && name != _obj->getDataObjectName()) {
            QMessageBox::warning(this, "", tr("\"%1\" The name already exists").arg(name), QMessageBox::Ok);
            return false;
        }
        //EOS ID
        QComboBox* eos_box = this->findChild<QComboBox*>("eos_box");
        if (!eos_box)return false;
        int eosID = eos_box->currentData().toInt();
        if (eosID < 1)
        {
            QMessageBox::warning(this, "", tr("Please select a EOS."), QMessageBox::Ok);
            return false;
        }
        return true;
    }
    void GUIMaterialHydroDialog::getDataFormWidget()
    {
        Radioss::FITKMaterialHydro* hydro = dynamic_cast<Radioss::FITKMaterialHydro*>(_obj);
        if (!hydro) return;
        Radioss::FITKMaterialHydroData& hydroData = hydro->getMaterialData();
        //材料密度
        QLineEdit* density_lineEdit = this->findChild<QLineEdit*>("density_lineEdit");
        if (!density_lineEdit)return;
        hydroData.m_density = density_lineEdit->text().toDouble();
        //参考密度
        QLineEdit* referenceDensity_lineEdit = this->findChild<QLineEdit*>("referenceDensity_lineEdit");
        if (!referenceDensity_lineEdit)return;
        hydroData.m_R_density = referenceDensity_lineEdit->text().toDouble();
        //运动粘度
        QLineEdit* kinematicViscosity_lineEdit = this->findChild<QLineEdit*>("kinematicViscosity_lineEdit");
        if (!kinematicViscosity_lineEdit)return;
        hydroData.m_kinematicViscosity = kinematicViscosity_lineEdit->text().toDouble();
        //剪切压力
        QLineEdit* pressureCut_lineEdit = this->findChild<QLineEdit*>("pressureCut_lineEdit");
        if (!pressureCut_lineEdit)return;
        hydroData.m_pressureCut = pressureCut_lineEdit->text().toDouble();
        //EOS ID
        QComboBox* eos_box = this->findChild<QComboBox*>("eos_box");
        if (!eos_box)return;
        int eosID = eos_box->currentData().toInt();
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return;
        Radioss::FITKEquationOfStateManager* Mgr = dataCase->getEquationOfState();
        if (Mgr == nullptr) return;
        Radioss::FITKAbstractEquationOfState* eos = Mgr->getDataByID(eosID);
        if (!eos)return;
        eos->addMaterialID(hydro->getDataObjectID(), Mgr);
        hydro->setEOSID(eosID);
    }

    void GUIMaterialHydroDialog::writePythonScript()
    {
        Radioss::FITKMaterialHydro* hydro = dynamic_cast<Radioss::FITKMaterialHydro*>(_obj);
        if (hydro == nullptr)
        {
            return;
        }

        Radioss::FITKMaterialHydroData hydroData = hydro->getMaterialDataReadOnly();

        QString eosName;
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase != nullptr)
        {
            Radioss::FITKEquationOfStateManager* manager = dataCase->getEquationOfState();
            if (manager != nullptr)
            {
                Radioss::FITKAbstractEquationOfState* eos = manager->getDataByID(hydro->getEOSID());
                if (eos != nullptr)
                {
                    eosName = eos->getDataObjectName();
                }
            }
        }

        QStringList script;
        if (_isCreate)
        {
            script.append("m = Material.Hydro()");
            script.append(QString("m.createMaterial('%1')").arg(hydro->getDataObjectName()));
        }
        else
        {
            QString oldName = _oldNameForScript.isEmpty() ? hydro->getDataObjectName() : _oldNameForScript;
            script.append(QString("m = Material.Hydro.GetHydro('%1')").arg(oldName));
            if (oldName != hydro->getDataObjectName())
            {
                script.append(QString("m.setName('%1')").arg(hydro->getDataObjectName()));
            }
        }

        script.append(QString("m.setDensity(%1)").arg(hydroData.m_density));
        script.append(QString("m.setReferenceDensity(%1)").arg(hydroData.m_R_density));
        script.append(QString("m.setKinematicViscosity(%1)").arg(hydroData.m_kinematicViscosity));
        script.append(QString("m.setPressureCut(%1)").arg(hydroData.m_pressureCut));
        if (!eosName.isEmpty())
        {
            script.append(QString("m.setEOS('%1')").arg(eosName));
        }

        this->saveScript(script);
    }
}