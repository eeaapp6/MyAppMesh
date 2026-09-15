/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIMaterialJohnsonCookDialog.h"
#include "ui_GUIMaterialPropDialog.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasJohns.h"

#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QComboBox>

namespace GUI
{
    GUIMaterialJohnsonCookDialog::GUIMaterialJohnsonCookDialog(Interface::FITKAbstractMaterial* obj, Core::FITKActionOperator* oper, QWidget* parent)
        : GUIMaterialPropDialog(obj, oper, parent)
    {
        QString MaterialName;
        _oper->argValue("MaterialName", MaterialName);
        if (!_obj) {
            _ui->lineEdit_name->setReadOnly(true);
            _obj = new Radioss::FITKMaterialPlasJohns();
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

    void GUIMaterialJohnsonCookDialog::initUI()
    {
        //添加根节点Common Properties
        QTreeWidgetItem* commonProperties_item = new QTreeWidgetItem(_ui->treeWidget_parameter);
        commonProperties_item->setText(0, tr("Common Properties"));
        commonProperties_item->setExpanded(true);
        //添加Common Properties子项材料密度
        this->addChiledItem(commonProperties_item,"density_lineEdit",tr("Initial Density"),"");
   
        //添加根节点Mechanical Properties
        QTreeWidgetItem* mechanicalProperties_item = new QTreeWidgetItem(_ui->treeWidget_parameter);
        mechanicalProperties_item->setText(0, tr("Mechanical Properties"));
        mechanicalProperties_item->setExpanded(true);
        //添加Mechanical Properties子项Elastic
        QTreeWidgetItem* elastic_item = new QTreeWidgetItem(mechanicalProperties_item);
        elastic_item->setText(0, tr("Elastic"));
        elastic_item->setExpanded(true);
        //添加Elastic子项杨氏模量
        this->addChiledItem(elastic_item,"youngsModulus_lineEdit", tr("Young's Modulus"),"");
        //添加Elastic子项泊松比
        this->addChiledItem(elastic_item, "poissonsRatio_lineEdit", tr("Poisson's Ratio"));

        //添加Mechanical Properties子项Plastic
        QTreeWidgetItem* plastic_item = new QTreeWidgetItem(mechanicalProperties_item);
        plastic_item-> setText(0, tr("Plastic"));
        plastic_item-> setExpanded(true);
        //添加Plastic子项Johnson Cook Parameter Input Method
        QWidget* plastic_widget1 = new QWidget(this);
        QHBoxLayout* plastic_layout1 = new QHBoxLayout();
        QComboBox* inputMethod_box = new QComboBox(this);
        inputMethod_box->addItem(tr("Classic"), Radioss::FITKMaterialPlasJohns::MATERIAL_PLAS_JOHNS_TYPE_CLASSIC);
        inputMethod_box->addItem(tr("Simplified"), Radioss::FITKMaterialPlasJohns::MATERIAL_PLAS_JOHNS_TYPE_SIMPLE);
        inputMethod_box->setObjectName("inputMethod_box");
        QLabel* plastic_label = new QLabel(this);
        plastic_label->setText("");
        plastic_layout1->addWidget(inputMethod_box);
        plastic_layout1->addWidget(plastic_label);
        plastic_layout1->setStretch(0, 1);
        plastic_layout1->setStretch(1, 1);
        plastic_widget1->setLayout(plastic_layout1);
        QTreeWidgetItem* inputMethod_item = new QTreeWidgetItem(plastic_item);
        inputMethod_item->setText(0, tr("Johnson Cook Parameter Input Method"));
        _ui->treeWidget_parameter->setItemWidget(inputMethod_item, 1, plastic_widget1);
        //添加Plastic子项Yield stress
        this->addChiledItem(plastic_item,"yieldStress_lineEdit", tr("Yield stress"),"");
        //添加Plastic子项plastic hardening parameter 塑性硬化参数
        QTreeWidgetItem* plasticHardening_item = this->addChiledItem(plastic_item, "plasticHardening_lineEdit", tr("Plastic hardening parameter"), "");
        //添加Plastic子项plastic hardening exponent 塑性硬化指数
        QTreeWidgetItem* plasticHardeningExp_item = this->addChiledItem(plastic_item, "plasticHardeningExp_lineEdit", tr("Plastic hardening exponent"), "");
        //添加Plastic子项Ultimate tensile stress极限拉伸应力
        QTreeWidgetItem* ultimateTensile_item = this->addChiledItem(plastic_item, "ultimateTensile_lineEdit", tr("Ultimate tensile stress"), "");
        //添加Plastic子项Engineering strain at UTS UTS工程应变
        QTreeWidgetItem* engineeringStrain_item = this->addChiledItem(plastic_item, "engineeringStrain_lineEdit", tr("Engineering strain at UTS"));
        ultimateTensile_item->setHidden(true);
        engineeringStrain_item->setHidden(true);

        connect(inputMethod_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {  
           bool is = (index == 0);  //索引0是Johnson-Cook
           plasticHardening_item->setHidden(!is);  
           plasticHardeningExp_item->setHidden(!is);  
           ultimateTensile_item->setHidden(is);  
           engineeringStrain_item->setHidden(is);  
        });
        //添加Plastic子项Failure plastic strain断裂应变
        this->addChiledItem(plastic_item, "failurePlastic_lineEdit", tr("Failure plastic strain"), "");
        //添加Plastic子项Maximum stress最大应力
        this->addChiledItem(plastic_item, "maximumStress_lineEdit", tr("Maximum stress"), "");
        //添加Plastic子项Strain rate coefficient应变率系数
        this->addChiledItem(plastic_item, "strainCoefficient_lineEdit", tr("Strain rate coefficient"), "");
        //添加Plastic子项Reference strain rate参考应变率
        this->addChiledItem(plastic_item, "ReferenceStrain_lineEdit", tr("Reference strain rate"), "");

        //添加Plastic子项Strain rate computation应变率计算标志
        QWidget* plastic_widget2 = new QWidget(this);
        QHBoxLayout* plastic_layout2 = new QHBoxLayout();
        QComboBox* StrainRateCom_box = new QComboBox(this);
        StrainRateCom_box->addItem(tr("Default"), 1);
        StrainRateCom_box->addItem(tr("set to 1"), 0);
        StrainRateCom_box->addItem(tr("No strain rate effect on Maximum stress"), 2);
        StrainRateCom_box->setObjectName("StrainRateCom_box");
        QLabel* plastic_labe2 = new QLabel(this);
        plastic_labe2->setText("");
        plastic_layout2->addWidget(StrainRateCom_box);
        plastic_layout2->addWidget(plastic_labe2);
        plastic_layout2->setStretch(0, 1);
        plastic_layout2->setStretch(1, 1);
        plastic_widget2->setLayout(plastic_layout2);
        QTreeWidgetItem* StrainRateCom_item = new QTreeWidgetItem(plastic_item);
        StrainRateCom_item->setText(0, tr("Strain rate computation"));
        _ui->treeWidget_parameter->setItemWidget(StrainRateCom_item, 1, plastic_widget2);

        //添加Plastic子项Strain rate smoothing应变率平滑标志
        QWidget* plastic_widget3 = new QWidget(this);
        QHBoxLayout* plastic_layout3 = new QHBoxLayout();
        QComboBox* StrainRateSmo_box = new QComboBox(this);
        StrainRateSmo_box->addItem(tr("Default"), 1);
        StrainRateSmo_box->addItem(tr("set to 1"), 0);
        StrainRateSmo_box->setObjectName("StrainRateSmo_box");
        QLabel* plastic_labe3 = new QLabel(this);
        plastic_labe3->setText("");
        plastic_layout3->addWidget(StrainRateSmo_box);
        plastic_layout3->addWidget(plastic_labe3);
        plastic_layout3->setStretch(0, 1);
        plastic_layout3->setStretch(1, 1);
        plastic_widget3->setLayout(plastic_layout3);
        QTreeWidgetItem* StrainRateSmo_item = new QTreeWidgetItem(plastic_item);
        StrainRateSmo_item->setText(0, tr("Strain rate smoothing"));
        _ui->treeWidget_parameter->setItemWidget(StrainRateSmo_item, 1, plastic_widget3);

        //添加Plastic子项Cut off Frequency应变率平滑截止频率
        this->addChiledItem(plastic_item, "Fcut_lineEdit", tr("Cut off Frequency"), "");

        //添加Plastic子项Hardening coefficient硬化系数
        this->addChiledItem(plastic_item, "HardeningCoe_lineEdit", tr("Hardening coefficient"), "");

        //添加根节点Thermal Properties
        QTreeWidgetItem* thermalProperties_item = new QTreeWidgetItem(_ui->treeWidget_parameter);
        thermalProperties_item->setText(0, tr("Thermal Properties"));
        thermalProperties_item->setExpanded(true);
        //添加Thermal Properties子项Temperature exponent温度指数
        this->addChiledItem(thermalProperties_item, "TemperatureExp_lineEdit", tr("Temperature exponent"), "");
        //添加Thermal Properties子项Melting temperature熔融温度
        this->addChiledItem(thermalProperties_item, "MeltingTem_lineEdit", tr("Melting temperature"), "");
        //添加Thermal Properties子项Specific heat per unit volume比热容
        this->addChiledItem(thermalProperties_item, "heatPerUnit_lineEdit", tr("Specific heat per unit volume"), "");
        //添加Thermal Properties子项Reference temperature参考温度
        this->addChiledItem(thermalProperties_item, "RefTem_lineEdit", tr("Reference temperature"), "");

        _ui->treeWidget_parameter->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    void GUIMaterialJohnsonCookDialog::init()
    {
        Radioss::FITKMaterialPlasJohns* joh = dynamic_cast<Radioss::FITKMaterialPlasJohns*>(_obj);
        if (!joh) return;
        //通用初始化
        this->GUIMaterialPropDialog::init();

        Radioss::FITKMaterialPlasJohnsData johData = joh->getMaterialDataReadOnly();

        _ui->lineEdit_matID->setReadOnly(true);
        _ui->lineEdit_model->setReadOnly(true);
        //设置名称
        _ui->lineEdit_name->setText(joh->getDataObjectName());
        //设置材料ID
        _ui->lineEdit_matID->setText(joh->GetFITKMaterialPlasJohnsRadiossKeyWord());
        //设置材料类型
        _ui->lineEdit_model->setText("PLAS_JOHNS");
        //材料密度
        QLineEdit* density_lineEdit = this->findChild<QLineEdit*>("density_lineEdit");
        if (!density_lineEdit)return;
        density_lineEdit->setText(QString::number(johData.m_density));
        //杨氏模量
        QLineEdit* youngsModulus_lineEdit=this->findChild<QLineEdit*>("youngsModulus_lineEdit");
        if (!youngsModulus_lineEdit)return;
        youngsModulus_lineEdit->setText(QString::number(johData.m_youngsModulus));
        //泊松比
        QLineEdit* poissonsRatio_lineEdit = this->findChild<QLineEdit*>("poissonsRatio_lineEdit");
        if (!poissonsRatio_lineEdit)return;
        poissonsRatio_lineEdit->setText(QString::number(johData.m_poissonsRatio));
        //类型
        QComboBox* inputMethod_box= this->findChild<QComboBox*>("inputMethod_box");
        inputMethod_box->setCurrentIndex(0);
        if (joh->getType() == Radioss::FITKMaterialPlasJohns::MATERIAL_PLAS_JOHNS_TYPE_SIMPLE)
            inputMethod_box->setCurrentIndex(1);
        //屈服应力
        QLineEdit* yieldStress_lineEdit = this->findChild<QLineEdit*>("yieldStress_lineEdit");
        if (!yieldStress_lineEdit)return;
        yieldStress_lineEdit->setText(QString::number(johData.m_yieldStress));
        //塑性硬化参数
        QLineEdit* plasticHardening_lineEdit = this->findChild<QLineEdit*>("plasticHardening_lineEdit");
        if (!plasticHardening_lineEdit)return;
        plasticHardening_lineEdit->setText(QString::number(johData.m_plasticHardening));
        //塑性硬化指数
        QLineEdit* plasticHardeningExp_lineEdit = this->findChild<QLineEdit*>("plasticHardeningExp_lineEdit");
        if (!plasticHardeningExp_lineEdit)return;
        plasticHardeningExp_lineEdit->setText(QString::number(johData.m_plasticHardeningExponent));
        //极限拉伸应力
        QLineEdit* ultimateTensile_lineEdit = this->findChild<QLineEdit*>("ultimateTensile_lineEdit");
        if (!ultimateTensile_lineEdit)return;
        ultimateTensile_lineEdit->setText(QString::number(johData.m_UTS));
        //UTS工程应变
        QLineEdit* engineeringStrain_lineEdit = this->findChild<QLineEdit*>("engineeringStrain_lineEdit");
        if (!engineeringStrain_lineEdit)return;
        engineeringStrain_lineEdit->setText(QString::number(johData.m_EpsilonUTS));
        //断裂应变
        QLineEdit* failurePlastic_lineEdit = this->findChild<QLineEdit*>("failurePlastic_lineEdit");
        if (!failurePlastic_lineEdit)return;
        failurePlastic_lineEdit->setText(QString::number(johData.m_failureStrain));
        //最大应力
        QLineEdit* maximumStress_lineEdit = this->findChild<QLineEdit*>("maximumStress_lineEdit");
        if (!maximumStress_lineEdit)return;
        maximumStress_lineEdit->setText(QString::number(johData.m_maxStress));
        //应变率系数
        QLineEdit* strainCoefficient_lineEdit = this->findChild<QLineEdit*>("strainCoefficient_lineEdit");
        if (!strainCoefficient_lineEdit)return;
        strainCoefficient_lineEdit->setText(QString::number(johData.m_c));
        //参考应变率
        QLineEdit* ReferenceStrain_lineEdit = this->findChild<QLineEdit*>("ReferenceStrain_lineEdit");
        if (!ReferenceStrain_lineEdit)return;
        ReferenceStrain_lineEdit->setText(QString::number(johData.m_Epsilon0));
        //应变率计算标志
        QComboBox* StrainRateCom_box = this->findChild<QComboBox*>("StrainRateCom_box");
        int index1 = StrainRateCom_box->findData(johData.m_ICC);
        StrainRateCom_box->setCurrentIndex(index1);
        //应变率平滑标志
        QComboBox* StrainRateSmo_box = this->findChild<QComboBox*>("StrainRateSmo_box");
        int index2 = StrainRateSmo_box->findData(johData.m_Fsmooth);
        StrainRateSmo_box->setCurrentIndex(index2);
        //应变率平滑截止频率
        QLineEdit* Fcut_lineEdit = this->findChild<QLineEdit*>("Fcut_lineEdit");
        if (!Fcut_lineEdit)return;
        Fcut_lineEdit->setText(QString::number(johData.m_Fcut));
        //硬化系数
        QLineEdit* HardeningCoe_lineEdit = this->findChild<QLineEdit*>("HardeningCoe_lineEdit");
        if (!HardeningCoe_lineEdit)return;
        HardeningCoe_lineEdit->setText(QString::number(johData.m_Chard));
        //温度指数
        QLineEdit* TemperatureExp_lineEdit = this->findChild<QLineEdit*>("TemperatureExp_lineEdit");
        if (!TemperatureExp_lineEdit)return;
        TemperatureExp_lineEdit->setText(QString::number(johData.m_tempExponent));
        //熔融温度
        QLineEdit* MeltingTem_lineEdit = this->findChild<QLineEdit*>("MeltingTem_lineEdit");
        if (!MeltingTem_lineEdit)return;
        MeltingTem_lineEdit->setText(QString::number(johData.m_meltingTemperature));
        //比热容
        QLineEdit* heatPerUnit_lineEdit = this->findChild<QLineEdit*>("heatPerUnit_lineEdit");
        if (!heatPerUnit_lineEdit)return;
        heatPerUnit_lineEdit->setText(QString::number(johData.m_rhoCp));
        //参考温度
        QLineEdit* RefTem_lineEdit = this->findChild<QLineEdit*>("RefTem_lineEdit");
        if (!RefTem_lineEdit)return;
        RefTem_lineEdit->setText(QString::number(johData.m_Tref));
    }

    bool GUIMaterialJohnsonCookDialog::ckeckData()
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
        //硬化系数
        QLineEdit* HardeningCoe_lineEdit = this->findChild<QLineEdit*>("HardeningCoe_lineEdit");
        if (!HardeningCoe_lineEdit)return false;
        if (HardeningCoe_lineEdit->text().toDouble() > 1) {
            QMessageBox::warning(this, "", tr("The Hardening coefficient ranges from 0 to 1"), QMessageBox::Ok);
            return false;
        };
        return true;
    }

    void GUIMaterialJohnsonCookDialog::getDataFormWidget()
    {
        Radioss::FITKMaterialPlasJohns* joh = dynamic_cast<Radioss::FITKMaterialPlasJohns*>(_obj);
        if (!joh) return;
        Radioss::FITKMaterialPlasJohnsData& johData = joh->getMaterialData();
        //材料密度
        QLineEdit* density_lineEdit = this->findChild<QLineEdit*>("density_lineEdit");
        if (!density_lineEdit)return;
        johData.m_density= density_lineEdit->text().toDouble();
        //杨氏模量
        QLineEdit* youngsModulus_lineEdit = this->findChild<QLineEdit*>("youngsModulus_lineEdit");
        if (!youngsModulus_lineEdit)return;
        johData.m_youngsModulus = youngsModulus_lineEdit->text().toDouble();
        //泊松比
        QLineEdit* poissonsRatio_lineEdit = this->findChild<QLineEdit*>("poissonsRatio_lineEdit");
        if (!poissonsRatio_lineEdit)return;
        johData.m_poissonsRatio = poissonsRatio_lineEdit->text().toDouble();
        //类型
        QComboBox* inputMethod_box = this->findChild<QComboBox*>("inputMethod_box");
        joh->setType((Radioss::FITKMaterialPlasJohns::MaterialPlasJohnsType)(inputMethod_box->currentData().toInt()));
        //屈服应力
        QLineEdit* yieldStress_lineEdit = this->findChild<QLineEdit*>("yieldStress_lineEdit");
        if (!yieldStress_lineEdit)return;
        johData.m_yieldStress = yieldStress_lineEdit->text().toDouble();
        //塑性硬化参数
        QLineEdit* plasticHardening_lineEdit = this->findChild<QLineEdit*>("plasticHardening_lineEdit");
        if (!plasticHardening_lineEdit)return;
        johData.m_plasticHardening = plasticHardening_lineEdit->text().toDouble();
        //塑性硬化指数
        QLineEdit* plasticHardeningExp_lineEdit = this->findChild<QLineEdit*>("plasticHardeningExp_lineEdit");
        if (!plasticHardeningExp_lineEdit)return;
        johData.m_plasticHardeningExponent = plasticHardeningExp_lineEdit->text().toDouble();
        //极限拉伸应力
        QLineEdit* ultimateTensile_lineEdit = this->findChild<QLineEdit*>("ultimateTensile_lineEdit");
        if (!ultimateTensile_lineEdit)return;
        johData.m_UTS = ultimateTensile_lineEdit->text().toDouble();
        //UTS工程应变
        QLineEdit* engineeringStrain_lineEdit = this->findChild<QLineEdit*>("engineeringStrain_lineEdit");
        if (!engineeringStrain_lineEdit)return;
        johData.m_EpsilonUTS = engineeringStrain_lineEdit->text().toDouble();
        //断裂应变
        QLineEdit* failurePlastic_lineEdit = this->findChild<QLineEdit*>("failurePlastic_lineEdit");
        if (!failurePlastic_lineEdit)return;
        johData.m_failureStrain = failurePlastic_lineEdit->text().toDouble();
        //最大应力
        QLineEdit* maximumStress_lineEdit = this->findChild<QLineEdit*>("maximumStress_lineEdit");
        if (!maximumStress_lineEdit)return;
        johData.m_maxStress = maximumStress_lineEdit->text().toDouble();
        //应变率系数
        QLineEdit* strainCoefficient_lineEdit = this->findChild<QLineEdit*>("strainCoefficient_lineEdit");
        if (!strainCoefficient_lineEdit)return;
        johData.m_c = strainCoefficient_lineEdit->text().toDouble();
        //参考应变率
        QLineEdit* ReferenceStrain_lineEdit = this->findChild<QLineEdit*>("ReferenceStrain_lineEdit");
        if (!ReferenceStrain_lineEdit)return;
        johData.m_Epsilon0 = ReferenceStrain_lineEdit->text().toDouble();
        //应变率计算标志
        QComboBox* StrainRateCom_box = this->findChild<QComboBox*>("StrainRateCom_box");
        johData.m_ICC = StrainRateCom_box->currentData().toInt();
        //应变率平滑标志
        QComboBox* StrainRateSmo_box = this->findChild<QComboBox*>("StrainRateSmo_box");
        johData.m_Fsmooth = StrainRateSmo_box->currentData().toInt();
        //应变率平滑截止频率
        QLineEdit* Fcut_lineEdit = this->findChild<QLineEdit*>("Fcut_lineEdit");
        if (!Fcut_lineEdit)return;
        johData.m_Fcut = Fcut_lineEdit->text().toDouble();
        //硬化系数
        QLineEdit* HardeningCoe_lineEdit = this->findChild<QLineEdit*>("HardeningCoe_lineEdit");
        if (!HardeningCoe_lineEdit)return;
        johData.m_Chard = HardeningCoe_lineEdit->text().toDouble();
        //温度指数
        QLineEdit* TemperatureExp_lineEdit = this->findChild<QLineEdit*>("TemperatureExp_lineEdit");
        if (!TemperatureExp_lineEdit)return;
        johData.m_tempExponent = TemperatureExp_lineEdit->text().toDouble();
        //熔融温度
        QLineEdit* MeltingTem_lineEdit = this->findChild<QLineEdit*>("MeltingTem_lineEdit");
        if (!MeltingTem_lineEdit)return;
        johData.m_meltingTemperature = MeltingTem_lineEdit->text().toDouble();
        //比热容
        QLineEdit* heatPerUnit_lineEdit = this->findChild<QLineEdit*>("heatPerUnit_lineEdit");
        if (!heatPerUnit_lineEdit)return;
        johData.m_rhoCp = heatPerUnit_lineEdit->text().toDouble();
        //参考温度
        QLineEdit* RefTem_lineEdit = this->findChild<QLineEdit*>("RefTem_lineEdit");
        if (!RefTem_lineEdit)return;
        johData.m_Tref = RefTem_lineEdit->text().toDouble();
    }

    void GUIMaterialJohnsonCookDialog::writePythonScript()
    {
        Radioss::FITKMaterialPlasJohns* joh = dynamic_cast<Radioss::FITKMaterialPlasJohns*>(_obj);
        if (joh == nullptr)
        {
            return;
        }

        Radioss::FITKMaterialPlasJohnsData johData = joh->getMaterialDataReadOnly();

        QStringList script;
        if (_isCreate)
        {
            script.append("m = Material.PlasJohns()");
            script.append(QString("m.createMaterial('%1')").arg(joh->getDataObjectName()));
        }
        else
        {
            QString oldName = _oldNameForScript.isEmpty() ? joh->getDataObjectName() : _oldNameForScript;
            script.append(QString("m = Material.PlasJohns.GetPlasJohns('%1')").arg(oldName));
            if (oldName != joh->getDataObjectName())
            {
                script.append(QString("m.setName('%1')").arg(joh->getDataObjectName()));
            }
        }

        script.append(QString("m.setType(%1)").arg(static_cast<int>(joh->getType())));
        script.append(QString("m.setDensity(%1)").arg(johData.m_density));
        script.append(QString("m.setYoungsModulus(%1)").arg(johData.m_youngsModulus));
        script.append(QString("m.setPoissonsRatio(%1)").arg(johData.m_poissonsRatio));
        script.append(QString("m.setYieldStress(%1)").arg(johData.m_yieldStress));
        script.append(QString("m.setPlasticHardening(%1)").arg(johData.m_plasticHardening));
        script.append(QString("m.setPlasticHardeningExponent(%1)").arg(johData.m_plasticHardeningExponent));
        script.append(QString("m.setFailureStrain(%1)").arg(johData.m_failureStrain));
        script.append(QString("m.setMaxStress(%1)").arg(johData.m_maxStress));
        script.append(QString("m.setUTS(%1)").arg(johData.m_UTS));
        script.append(QString("m.setEpsilonUTS(%1)").arg(johData.m_EpsilonUTS));
        script.append(QString("m.setStrainRateCoefficient(%1)").arg(johData.m_c));
        script.append(QString("m.setReferenceStrainRate(%1)").arg(johData.m_Epsilon0));
        script.append(QString("m.setStrainRateComputation(%1)").arg(johData.m_ICC));
        script.append(QString("m.setStrainRateSmoothing(%1)").arg(johData.m_Fsmooth));
        script.append(QString("m.setCutoffFreq(%1)").arg(johData.m_Fcut));
        script.append(QString("m.setChard(%1)").arg(johData.m_Chard));
        script.append(QString("m.setTempExponent(%1)").arg(johData.m_tempExponent));
        script.append(QString("m.setMeltingTemperature(%1)").arg(johData.m_meltingTemperature));
        script.append(QString("m.setRhoCp(%1)").arg(johData.m_rhoCp));
        script.append(QString("m.setTref(%1)").arg(johData.m_Tref));

        this->saveScript(script);
    }
}