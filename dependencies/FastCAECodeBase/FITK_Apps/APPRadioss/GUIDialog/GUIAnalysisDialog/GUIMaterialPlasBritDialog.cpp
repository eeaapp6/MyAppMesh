/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIMaterialPlasBritDialog.h"
#include "ui_GUIMaterialPropDialog.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasBrit.h"

#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QComboBox>

namespace GUI
{
    GUIMaterialPlasBritDialog::GUIMaterialPlasBritDialog(Interface::FITKAbstractMaterial * obj, Core::FITKActionOperator * oper, QWidget * parent)
        : GUIMaterialPropDialog(obj, oper, parent)
    {
        QString MaterialName;
        _oper->argValue("MaterialName", MaterialName);
        if (!_obj) {
            _ui->lineEdit_name->setReadOnly(true);
            _obj = new Radioss::FITKMaterialPlasBrit();
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
    void GUIMaterialPlasBritDialog::initUI()
    {
        //添加根节点Common Properties
        QTreeWidgetItem* commonProperties_item = new QTreeWidgetItem(_ui->treeWidget_parameter);
        commonProperties_item->setText(0, tr("Common Properties"));
        commonProperties_item->setExpanded(true);
        //添加Common Properties子项材料密度
        this->addChiledItem(commonProperties_item, "density_lineEdit", tr("Initial Density"), "");

        //添加根节点Mechanical Properties
        QTreeWidgetItem* mechanicalProperties_item = new QTreeWidgetItem(_ui->treeWidget_parameter);
        mechanicalProperties_item->setText(0, tr("Mechanical Properties"));
        mechanicalProperties_item->setExpanded(true);
        //添加Mechanical Properties子项Elastic
        QTreeWidgetItem* elastic_item = new QTreeWidgetItem(mechanicalProperties_item);
        elastic_item->setText(0, tr("Elastic"));
        elastic_item->setExpanded(true);
        //添加Elastic子项杨氏模量
        this->addChiledItem(elastic_item, "youngsModulus_lineEdit", tr("Young's Modulus"), "");
        //添加Elastic子项泊松比
        this->addChiledItem(elastic_item, "poissonsRatio_lineEdit", tr("Poisson's Ratio"));

        //添加Mechanical Properties子项Plastic
        QTreeWidgetItem* plastic_item = new QTreeWidgetItem(mechanicalProperties_item);
        plastic_item->setText(0, tr("Plastic"));
        plastic_item->setExpanded(true);
        //添加Plastic子项Yield stress   a
        this->addChiledItem(plastic_item, "yieldStress_lineEdit", tr("Yield stress"), "");
        //添加Plastic子项plastic hardening parameter 塑性硬化参数 b
        this->addChiledItem(plastic_item, "plasticHardening_lineEdit", tr("Plastic hardening parameter"), "");
        //添加Plastic子项plastic hardening exponent 塑性硬化指数 n
        this->addChiledItem(plastic_item, "plasticHardeningExp_lineEdit", tr("Plastic hardening exponent"), "");
        //添加Plastic子项Maximum stress 最大应力 SIG_max0
        this->addChiledItem(plastic_item, "maximumStress_lineEdit", tr("Maximum stress"), "");
        //添加Plastic子项Strain rate coefficient应变率系数 c
        this->addChiledItem(plastic_item, "strainCoefficient_lineEdit", tr("Strain rate coefficient"), "");
        //添加Plastic子项Reference strain rate参考应变率 EPS_DOT_0
        this->addChiledItem(plastic_item, "ReferenceStrain_lineEdit", tr("Reference strain rate"), "");

        //添加Plastic子项Strain rate computation应变率计算标志ICC
        QWidget* plastic_widget2 = new QWidget(this);
        QHBoxLayout* plastic_layout2 = new QHBoxLayout();
        QComboBox* StrainRateCom_box = new QComboBox(this);
        StrainRateCom_box->addItem(tr("Default"), 0);
        StrainRateCom_box->addItem(tr("Strain rate effect on SIG_max0"), 1);
        StrainRateCom_box->addItem(tr("No strain rate effect on SIG_max0"), 2);
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

        //添加Plastic子项Strain rate smoothing应变率平滑标志Fsmooth
        QWidget* plastic_widget3 = new QWidget(this);
        QHBoxLayout* plastic_layout3 = new QHBoxLayout();
        QComboBox* StrainRateSmo_box = new QComboBox(this);
        StrainRateSmo_box->addItem(tr("Inactive"), 0);
        StrainRateSmo_box->addItem(tr("Active"), 1);
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

        //添加Plastic子项Cut off Frequency应变率平滑截止频率Fcut
        this->addChiledItem(plastic_item, "Fcut_lineEdit", tr("Cut off Frequency"), "");

        //添加Mechanical Properties子项Orthotropic Failure正交失效方向1的参数
        QTreeWidgetItem* orthFail1_item = new QTreeWidgetItem(mechanicalProperties_item);
        orthFail1_item->setText(0, tr("Orthotropic Failure Direction1"));
        orthFail1_item->setExpanded(true);
        //添加Orthotropic Failure子项EPS_t1
        this->addChiledItem(orthFail1_item, "EPS_t1_lineEdit", tr("EPS_t1"), "");
        //添加Orthotropic Failure子项EPS_m1
        this->addChiledItem(orthFail1_item, "EPS_m1_lineEdit", tr("EPS_m1"), "");
        //添加Orthotropic Failure子项d_max1
        this->addChiledItem(orthFail1_item, "d_max1_lineEdit", tr("d_max1"), "");
        //添加Orthotropic Failure子项EPS_f1
        this->addChiledItem(orthFail1_item, "EPS_f1_lineEdit", tr("EPS_f1"), "");

        //添加Mechanical Properties子项Orthotropic Failure正交失效方向2的参数
        QTreeWidgetItem* orthFail2_item = new QTreeWidgetItem(mechanicalProperties_item);
        orthFail2_item->setText(0, tr("Orthotropic Failure Direction2"));
        orthFail2_item->setExpanded(true);
        //添加Orthotropic Failure子项EPS_t2
        this->addChiledItem(orthFail2_item, "EPS_t2_lineEdit", tr("EPS_t2"), "");
        //添加Orthotropic Failure子项EPS_m2                             
        this->addChiledItem(orthFail2_item, "EPS_m2_lineEdit", tr("EPS_m2"), "");
        //添加Orthotropic Failure子项d_max2                             
        this->addChiledItem(orthFail2_item, "d_max2_lineEdit", tr("d_max2"), "");
        //添加Orthotropic Failure子项EPS_f2                             
        this->addChiledItem(orthFail2_item, "EPS_f2_lineEdit", tr("EPS_f2"), "");

        _ui->treeWidget_parameter->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    void GUIMaterialPlasBritDialog::init()
    {
        Radioss::FITKMaterialPlasBrit* brit = dynamic_cast<Radioss::FITKMaterialPlasBrit*>(_obj);
        if (!brit) return;
        //通用初始化
        this->GUIMaterialPropDialog::init();

        Radioss::FITKMaterialPlasBritData britData = brit->getMaterialDataReadOnly();

        _ui->lineEdit_matID->setReadOnly(true);
        _ui->lineEdit_model->setReadOnly(true);
        //设置名称
        _ui->lineEdit_name->setText(brit->getDataObjectName());
        //设置材料ID
        _ui->lineEdit_matID->setText(brit->GetFITKMaterialPlasBritRadiossKeyWord());
        //设置材料类型 正交各向异性脆性失效
        _ui->lineEdit_model->setText("PLAS_BRIT");
        //材料密度
        QLineEdit* density_lineEdit = this->findChild<QLineEdit*>("density_lineEdit");
        if (!density_lineEdit)return;
        density_lineEdit->setText(QString::number(britData.m_density));
        //杨氏模量
        QLineEdit* youngsModulus_lineEdit = this->findChild<QLineEdit*>("youngsModulus_lineEdit");
        if (!youngsModulus_lineEdit)return;
        youngsModulus_lineEdit->setText(QString::number(britData.m_youngsModulus));
        //泊松比
        QLineEdit* poissonsRatio_lineEdit = this->findChild<QLineEdit*>("poissonsRatio_lineEdit");
        if (!poissonsRatio_lineEdit)return;
        poissonsRatio_lineEdit->setText(QString::number(britData.m_poissonsRatio));
        //屈服应力
        QLineEdit* yieldStress_lineEdit = this->findChild<QLineEdit*>("yieldStress_lineEdit");
        if (!yieldStress_lineEdit)return;
        yieldStress_lineEdit->setText(QString::number(britData.m_plasticityYieldStress));
        //塑性硬化参数
        QLineEdit* plasticHardening_lineEdit = this->findChild<QLineEdit*>("plasticHardening_lineEdit");
        if (!plasticHardening_lineEdit)return;
        plasticHardening_lineEdit->setText(QString::number(britData.m_plasticityHardeningParameter));
        //塑性硬化指数
        QLineEdit* plasticHardeningExp_lineEdit = this->findChild<QLineEdit*>("plasticHardeningExp_lineEdit");
        if (!plasticHardeningExp_lineEdit)return;
        plasticHardeningExp_lineEdit->setText(QString::number(britData.m_plasticityHardeningExponent));
        //最大应力
        QLineEdit* maximumStress_lineEdit = this->findChild<QLineEdit*>("maximumStress_lineEdit");
        if (!maximumStress_lineEdit)return;
        maximumStress_lineEdit->setText(QString::number(britData.m_plasticityMaximumStress));
        //应变率系数
        QLineEdit* strainCoefficient_lineEdit = this->findChild<QLineEdit*>("strainCoefficient_lineEdit");
        if (!strainCoefficient_lineEdit)return;
        strainCoefficient_lineEdit->setText(QString::number(britData.m_strainRateCoefficient));
        //参考应变率
        QLineEdit* ReferenceStrain_lineEdit = this->findChild<QLineEdit*>("ReferenceStrain_lineEdit");
        if (!ReferenceStrain_lineEdit)return;
        ReferenceStrain_lineEdit->setText(QString::number(britData.m_referenceStrainRate));
        //应变率计算标志
        QComboBox* StrainRateCom_box = this->findChild<QComboBox*>("StrainRateCom_box");
        int index1 = StrainRateCom_box->findData(britData.m_strainRateComputation);
        StrainRateCom_box->setCurrentIndex(index1);
        //应变率平滑标志
        QComboBox* StrainRateSmo_box = this->findChild<QComboBox*>("StrainRateSmo_box");
        int index2 = StrainRateSmo_box->findData(britData.m_strainRateSmoothing);
        StrainRateSmo_box->setCurrentIndex(index2);
        //应变率平滑截止频率
        QLineEdit* Fcut_lineEdit = this->findChild<QLineEdit*>("Fcut_lineEdit");
        if (!Fcut_lineEdit)return;
        Fcut_lineEdit->setText(QString::number(britData.m_cutoffFreq));
        //正交失效方向1 EPS_t1
        QLineEdit* EPS_t1_lineEdit = this->findChild<QLineEdit*>("EPS_t1_lineEdit");
        if (!EPS_t1_lineEdit)return;
        EPS_t1_lineEdit->setText(QString::number(britData.m_tensileFilureStrain1));
        //正交失效方向1 EPS_m1
        QLineEdit* EPS_m1_lineEdit = this->findChild<QLineEdit*>("EPS_m1_lineEdit");
        if (!EPS_m1_lineEdit)return;
        EPS_m1_lineEdit->setText(QString::number(britData.m_maximumTensileFilureStrain1));
        //正交失效方向1 d_max1
        QLineEdit* d_max1_lineEdit = this->findChild<QLineEdit*>("d_max1_lineEdit");
        if (!d_max1_lineEdit)return;
        d_max1_lineEdit->setText(QString::number(britData.m_maximumDamageFactor1));
        //正交失效方向1 EPS_f1
        QLineEdit* EPS_f1_lineEdit = this->findChild<QLineEdit*>("EPS_f1_lineEdit");
        if (!EPS_f1_lineEdit)return;
        EPS_f1_lineEdit->setText(QString::number(britData.m_maximumTensileStrain1));
        //正交失效方向2 EPS_t2
        QLineEdit* EPS_t2_lineEdit = this->findChild<QLineEdit*>("EPS_t2_lineEdit");
        if (!EPS_t2_lineEdit)return;
        EPS_t2_lineEdit->setText(QString::number(britData.m_tensileFilureStrain2));
        //正交失效方向2 EPS_m2
        QLineEdit* EPS_m2_lineEdit = this->findChild<QLineEdit*>("EPS_m2_lineEdit");
        if (!EPS_m2_lineEdit)return;
        EPS_m2_lineEdit->setText(QString::number(britData.m_maximumTensileFilureStrain2));
        //正交失效方向2 d_max2
        QLineEdit* d_max2_lineEdit = this->findChild<QLineEdit*>("d_max2_lineEdit");
        if (!d_max2_lineEdit)return;
        d_max2_lineEdit->setText(QString::number(britData.m_maximumDamageFactor2));
        //正交失效方向2 EPS_f2
        QLineEdit* EPS_f2_lineEdit = this->findChild<QLineEdit*>("EPS_f2_lineEdit");
        if (!EPS_f2_lineEdit)return;
        EPS_f2_lineEdit->setText(QString::number(britData.m_maximumTensileStrain2));
    }
    bool GUIMaterialPlasBritDialog::ckeckData()
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
        return true;
    }
    void GUIMaterialPlasBritDialog::getDataFormWidget()
    {
        Radioss::FITKMaterialPlasBrit* brit = dynamic_cast<Radioss::FITKMaterialPlasBrit*>(_obj);
        if (!brit) return;
        Radioss::FITKMaterialPlasBritData& britData = brit->getMaterialData();
        //材料密度
        QLineEdit* density_lineEdit = this->findChild<QLineEdit*>("density_lineEdit");
        if (!density_lineEdit)return;
        britData.m_density = density_lineEdit->text().toDouble();
        //杨氏模量
        QLineEdit* youngsModulus_lineEdit = this->findChild<QLineEdit*>("youngsModulus_lineEdit");
        if (!youngsModulus_lineEdit)return;
        britData.m_youngsModulus = youngsModulus_lineEdit->text().toDouble();
        //泊松比
        QLineEdit* poissonsRatio_lineEdit = this->findChild<QLineEdit*>("poissonsRatio_lineEdit");
        if (!poissonsRatio_lineEdit)return;
        britData.m_poissonsRatio = poissonsRatio_lineEdit->text().toDouble();
        //屈服应力
        QLineEdit* yieldStress_lineEdit = this->findChild<QLineEdit*>("yieldStress_lineEdit");
        if (!yieldStress_lineEdit)return;
        britData.m_plasticityYieldStress = yieldStress_lineEdit->text().toDouble();
        //塑性硬化参数
        QLineEdit* plasticHardening_lineEdit = this->findChild<QLineEdit*>("plasticHardening_lineEdit");
        if (!plasticHardening_lineEdit)return;
        britData.m_plasticityHardeningParameter = plasticHardening_lineEdit->text().toDouble();
        //塑性硬化指数
        QLineEdit* plasticHardeningExp_lineEdit = this->findChild<QLineEdit*>("plasticHardeningExp_lineEdit");
        if (!plasticHardeningExp_lineEdit)return;
        britData.m_plasticityHardeningExponent = plasticHardeningExp_lineEdit->text().toDouble();
        //最大应力
        QLineEdit* maximumStress_lineEdit = this->findChild<QLineEdit*>("maximumStress_lineEdit");
        if (!maximumStress_lineEdit)return;
        britData.m_plasticityMaximumStress = maximumStress_lineEdit->text().toDouble();
        //应变率系数
        QLineEdit* strainCoefficient_lineEdit = this->findChild<QLineEdit*>("strainCoefficient_lineEdit");
        if (!strainCoefficient_lineEdit)return;
        britData.m_strainRateCoefficient = strainCoefficient_lineEdit->text().toDouble();
        //参考应变率
        QLineEdit* ReferenceStrain_lineEdit = this->findChild<QLineEdit*>("ReferenceStrain_lineEdit");
        if (!ReferenceStrain_lineEdit)return;
        britData.m_referenceStrainRate = ReferenceStrain_lineEdit->text().toDouble();
        //应变率计算标志
        QComboBox* StrainRateCom_box = this->findChild<QComboBox*>("StrainRateCom_box");
        britData.m_strainRateComputation = StrainRateCom_box->currentData().toInt();
        //应变率平滑标志
        QComboBox* StrainRateSmo_box = this->findChild<QComboBox*>("StrainRateSmo_box");
        britData.m_strainRateSmoothing = StrainRateSmo_box->currentData().toInt();
        //应变率平滑截止频率
        QLineEdit* Fcut_lineEdit = this->findChild<QLineEdit*>("Fcut_lineEdit");
        if (!Fcut_lineEdit)return;
        britData.m_cutoffFreq = Fcut_lineEdit->text().toDouble();
        //正交失效方向1 EPS_t1
        QLineEdit* EPS_t1_lineEdit = this->findChild<QLineEdit*>("EPS_t1_lineEdit");
        if (!EPS_t1_lineEdit)return;
        britData.m_tensileFilureStrain1 = EPS_t1_lineEdit->text().toDouble();
        //正交失效方向1 EPS_m1
        QLineEdit* EPS_m1_lineEdit = this->findChild<QLineEdit*>("EPS_m1_lineEdit");
        if (!EPS_m1_lineEdit)return;
        britData.m_maximumTensileFilureStrain1 = EPS_m1_lineEdit->text().toDouble();
        //正交失效方向1 d_max1
        QLineEdit* d_max1_lineEdit = this->findChild<QLineEdit*>("d_max1_lineEdit");
        if (!d_max1_lineEdit)return;
        britData.m_maximumDamageFactor1 = d_max1_lineEdit->text().toDouble();
        //正交失效方向1 EPS_f1
        QLineEdit* EPS_f1_lineEdit = this->findChild<QLineEdit*>("EPS_f1_lineEdit");
        if (!EPS_f1_lineEdit)return;
        britData.m_maximumTensileStrain1 = EPS_f1_lineEdit->text().toDouble();
        //正交失效方向2 EPS_t2
        QLineEdit* EPS_t2_lineEdit = this->findChild<QLineEdit*>("EPS_t2_lineEdit");
        if (!EPS_t2_lineEdit)return;
        britData.m_tensileFilureStrain2 = EPS_t2_lineEdit->text().toDouble();
        //正交失效方向2 EPS_m2
        QLineEdit* EPS_m2_lineEdit = this->findChild<QLineEdit*>("EPS_m2_lineEdit");
        if (!EPS_m2_lineEdit)return;
        britData.m_maximumTensileFilureStrain2 = EPS_m2_lineEdit->text().toDouble();
        //正交失效方向2 d_max2
        QLineEdit* d_max2_lineEdit = this->findChild<QLineEdit*>("d_max2_lineEdit");
        if (!d_max2_lineEdit)return;
        britData.m_maximumDamageFactor2 = d_max2_lineEdit->text().toDouble();
        //正交失效方向2 EPS_f2
        QLineEdit* EPS_f2_lineEdit = this->findChild<QLineEdit*>("EPS_f2_lineEdit");
        if (!EPS_f2_lineEdit)return;
        britData.m_maximumTensileStrain2 = EPS_f2_lineEdit->text().toDouble();


    }

    void GUIMaterialPlasBritDialog::writePythonScript()
    {
        Radioss::FITKMaterialPlasBrit* brit = dynamic_cast<Radioss::FITKMaterialPlasBrit*>(_obj);
        if (brit == nullptr)
        {
            return;
        }

        Radioss::FITKMaterialPlasBritData britData = brit->getMaterialDataReadOnly();

        QStringList script;
        if (_isCreate)
        {
            script.append("m = Material.PlasBrit()");
            script.append(QString("m.createMaterial('%1')").arg(brit->getDataObjectName()));
        }
        else
        {
            QString oldName = _oldNameForScript.isEmpty() ? brit->getDataObjectName() : _oldNameForScript;
            script.append(QString("m = Material.PlasBrit.GetPlasBrit('%1')").arg(oldName));
            if (oldName != brit->getDataObjectName())
            {
                script.append(QString("m.setName('%1')").arg(brit->getDataObjectName()));
            }
        }

        script.append(QString("m.setDensity(%1)").arg(britData.m_density));
        script.append(QString("m.setYoungsModulus(%1)").arg(britData.m_youngsModulus));
        script.append(QString("m.setPoissonsRatio(%1)").arg(britData.m_poissonsRatio));
        script.append(QString("m.setPlasticityYieldStress(%1)").arg(britData.m_plasticityYieldStress));
        script.append(QString("m.setPlasticityHardeningParameter(%1)").arg(britData.m_plasticityHardeningParameter));
        script.append(QString("m.setPlasticityHardeningExponent(%1)").arg(britData.m_plasticityHardeningExponent));
        script.append(QString("m.setPlasticityMaximumStress(%1)").arg(britData.m_plasticityMaximumStress));
        script.append(QString("m.setStrainRateCoefficient(%1)").arg(britData.m_strainRateCoefficient));
        script.append(QString("m.setReferenceStrainRate(%1)").arg(britData.m_referenceStrainRate));
        script.append(QString("m.setStrainRateComputation(%1)").arg(britData.m_strainRateComputation));
        script.append(QString("m.setStrainRateSmoothing(%1)").arg(britData.m_strainRateSmoothing));
        script.append(QString("m.setCutoffFreq(%1)").arg(britData.m_cutoffFreq));
        script.append(QString("m.setTensileFilureStrain1(%1)").arg(britData.m_tensileFilureStrain1));
        script.append(QString("m.setMaximumTensileFilureStrain1(%1)").arg(britData.m_maximumTensileFilureStrain1));
        script.append(QString("m.setMaximumDamageFactor1(%1)").arg(britData.m_maximumDamageFactor1));
        script.append(QString("m.setMaximumTensileStrain1(%1)").arg(britData.m_maximumTensileStrain1));
        script.append(QString("m.setTensileFilureStrain2(%1)").arg(britData.m_tensileFilureStrain2));
        script.append(QString("m.setMaximumTensileFilureStrain2(%1)").arg(britData.m_maximumTensileFilureStrain2));
        script.append(QString("m.setMaximumDamageFactor2(%1)").arg(britData.m_maximumDamageFactor2));
        script.append(QString("m.setMaximumTensileStrain2(%1)").arg(britData.m_maximumTensileStrain2));

        this->saveScript(script);
    }
}