/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIMaterialPlasTabDialog.h"
#include "ui_GUIMaterialPropDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasTab.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QComboBox>

namespace GUI
{
    GUIMaterialPlasTabDialog::GUIMaterialPlasTabDialog(Interface::FITKAbstractMaterial* obj, Core::FITKActionOperator* oper, QWidget* parent)
        : GUIMaterialPropDialog(obj, oper, parent)
    {
        QString MaterialName;
        _oper->argValue("MaterialName", MaterialName);
        if (!_obj) {
            _ui->lineEdit_name->setReadOnly(true);
            _obj = new Radioss::FITKMaterialPlasTab();
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

    void GUIMaterialPlasTabDialog::initUI()
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
        //添加Plastic子项失效应变
        this->addChiledItem(plastic_item, "failureStrain_lineEdit", tr("Failure Plastic Strain"));
        //添加Plastic子项拉伸失效应变
        this->addChiledItem(plastic_item, "tensileFailureStrain_lineEdit", tr("Tensile Failure Strain"));
        //添加Plastic子项最大拉伸失效应力
        this->addChiledItem(plastic_item, "maxTensileFailureStrain_lineEdit", tr("Max Tensile Failure Strain"),"");
        //添加Plastic子项函数数量
        QWidget* plastic_widget2 = new QWidget(this);
        QHBoxLayout* plastic_layout2 = new QHBoxLayout();
        QComboBox* NOF_box = new QComboBox(this);
        for (int i = 0; i < 101; ++i) {
            NOF_box->addItem(QString::number(i),i);
        }
        NOF_box->setObjectName("NOF_box");
        QLabel* plastic_label2 = new QLabel(this);
        plastic_label2->setText("");
        plastic_layout2->addWidget(NOF_box);
        plastic_layout2->addWidget(plastic_label2);
        plastic_layout2->setStretch(0, 1);
        plastic_layout2->setStretch(1, 1);
        plastic_widget2->setLayout(plastic_layout2);
        QTreeWidgetItem* NOF_item = new QTreeWidgetItem(plastic_item);
        NOF_item->setText(0, tr("Number Of Functions"));
        _ui->treeWidget_parameter->setItemWidget(NOF_item, 1, plastic_widget2);
        connect(NOF_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
            updateFuncSlot(index-1);
        });
        //添加Plastic子项平滑应变率选项标志
        QWidget* plastic_widget3 = new QWidget(this);
        QHBoxLayout* plastic_layout3 = new QHBoxLayout();
        QComboBox* smoothStrainRate_box = new QComboBox(this);
        smoothStrainRate_box->addItem("Inactive", 0);
        smoothStrainRate_box->addItem("Active", 1);
        smoothStrainRate_box->setObjectName("smoothStrainRate_box");
        QLabel* plastic_label3 = new QLabel(this);
        plastic_label3->setText("");
        plastic_layout3->addWidget(smoothStrainRate_box);
        plastic_layout3->addWidget(plastic_label3);
        plastic_layout3->setStretch(0, 1);
        plastic_layout3->setStretch(1, 1);
        plastic_widget3->setLayout(plastic_layout3);
        QTreeWidgetItem* smoothStrainRate_item = new QTreeWidgetItem(plastic_item);
        smoothStrainRate_item->setText(0, tr("Smooth Strain Rate Option"));
        _ui->treeWidget_parameter->setItemWidget(smoothStrainRate_item, 1, plastic_widget3);


        //添加Plastic子项硬化系数
        this->addChiledItem(plastic_item, "HardeningCoe_lineEdit", tr("Hardening coefficient"), "");
        // 添加Plastic子项Cut off Frequency应变率平滑截止频率
        this->addChiledItem(plastic_item, "Fcut_lineEdit", tr("Cut off Frequency"), "");
        // 添加Plastic子项strain rate choice flag
        QWidget* plastic_widget4 = new QWidget(this);
        QHBoxLayout* plastic_layout4 = new QHBoxLayout();
        QComboBox* strainRateChoice_box = new QComboBox(this);
        strainRateChoice_box->addItem("Stress-Total Stain", 0);
        strainRateChoice_box->addItem("Stress-Plastic Stain", 1);
        strainRateChoice_box->setObjectName("strainRateChoice_box");
        QLabel* plastic_label4 = new QLabel(this);
        plastic_label4->setText("");
        plastic_layout4->addWidget(strainRateChoice_box);
        plastic_layout4->addWidget(plastic_label4);
        plastic_layout4->setStretch(0, 1);
        plastic_layout4->setStretch(1, 1);
        plastic_widget4->setLayout(plastic_layout4);
        QTreeWidgetItem* strainRateChoice_item = new QTreeWidgetItem(plastic_item);
        strainRateChoice_item->setText(0, tr("Strain Rate Choice Flag"));
        _ui->treeWidget_parameter->setItemWidget(strainRateChoice_item, 1, plastic_widget4);

        // 添加Plastic子项元素删除的拉伸应变
        this->addChiledItem(plastic_item, "tensileStrainForElementDeletion_lineEdit", tr("Tensile Strain For Element Deletion"));

        // 添加Plastic子项 Yield factor versus pressure function
        QTreeWidgetItem* yieldFactor_item = new QTreeWidgetItem(plastic_item);
        yieldFactor_item->setText(0, tr("Yield Factor Versus Pressure Function"));
        yieldFactor_item->setExpanded(true);
        //添加Yield factor versus pressure function子项曲线列表
        QWidget* plastic_widget1 = new QWidget(this);
        QHBoxLayout* plastic_layout1 = new QHBoxLayout();
        QComboBox* curve_box1 = new QComboBox(this);
        this->setCurveBox(curve_box1);
        curve_box1->setObjectName("yieldFactor_box");
        QLabel* plastic_label = new QLabel(this);
        plastic_label->setText("");
        plastic_layout1->addWidget(curve_box1);
        plastic_layout1->addWidget(plastic_label);
        plastic_layout1->setStretch(0, 1);
        plastic_layout1->setStretch(1, 1);
        plastic_widget1->setLayout(plastic_layout1);
        QTreeWidgetItem* inputMethod_item = new QTreeWidgetItem(yieldFactor_item);
        inputMethod_item->setText(0, tr("curve"));
        _ui->treeWidget_parameter->setItemWidget(inputMethod_item, 1, plastic_widget1);
        // 添加Yield factor versus pressure function子项 m_fctIDp 缩放因子 [Pa]
        this->addChiledItem(yieldFactor_item, "scaleFactorYieldFactor_lineEdit", tr("Scale Factor Yield Factor"),"");

        // 添加Plastic子项 Function identifier for the scale factor of Young's modulus,when Young's modulus is function of the plastic strain.
        this->addChiledItem(plastic_item, "factorIdentifier_lineEdit", tr("Factor Identifier"));
        // 添加Plastic子项 Saturated Young's modulus for infinitive plastic strain.
        this->addChiledItem(plastic_item, "saturatedYoungsModulus_lineEdit", tr("Saturated Young's Modulus"));
        // 添加Plastic子项 parameter for the evolution of Young's modulus
        this->addChiledItem(plastic_item, "parameterForTheEvolution_lineEdit", tr("Parameter for The Evolution"));

        //添加“Number Of Functions”联动项
        QTreeWidgetItem* YSS_item = new QTreeWidgetItem(plastic_item);
        YSS_item->setText(0, tr("YieldStressFunction_ScaleFactor_StrainRate"));
        YSS_item->setExpanded(true);


        _ui->treeWidget_parameter->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    void GUIMaterialPlasTabDialog::init()
    {
        Radioss::FITKMaterialPlasTab* plasTab = dynamic_cast<Radioss::FITKMaterialPlasTab*>(_obj);
        if (!plasTab) return;
        //通用初始化
        this->GUIMaterialPropDialog::init();

        Radioss::FITKMaterialPlasTabData  plasTabData = plasTab->getMaterialDataReadOnly();

        _ui->lineEdit_matID->setReadOnly(true);
        _ui->lineEdit_model->setReadOnly(true);
        //设置名称
        _ui->lineEdit_name->setText(plasTab->getDataObjectName());
        //设置材料ID
        _ui->lineEdit_matID->setText(plasTab->GetFITKMaterialPlasTabRadiossKeyWord());
        //设置材料类型
        _ui->lineEdit_model->setText("PLAS_TAB");
        //材料密度
        QLineEdit* density_lineEdit = this->findChild<QLineEdit*>("density_lineEdit");
        if (!density_lineEdit)return;
        density_lineEdit->setText(QString::number(plasTabData.m_density));
        //杨氏模量
        QLineEdit* youngsModulus_lineEdit=this->findChild<QLineEdit*>("youngsModulus_lineEdit");
        if (!youngsModulus_lineEdit)return;
        youngsModulus_lineEdit->setText(QString::number(plasTabData.m_youngsModulus));
        //泊松比
        QLineEdit* poissonsRatio_lineEdit = this->findChild<QLineEdit*>("poissonsRatio_lineEdit");
        if (!poissonsRatio_lineEdit)return;
        poissonsRatio_lineEdit->setText(QString::number(plasTabData.m_poissonsRatio));
        //失效应变
        QLineEdit* failureStrain_lineEdit = this->findChild<QLineEdit*>("failureStrain_lineEdit");
        if (!failureStrain_lineEdit)return;
        failureStrain_lineEdit->setText(QString::number(plasTabData.m_failureStrain));
        //拉伸失效应变
        QLineEdit* tensileFailureStrain_lineEdit = this->findChild<QLineEdit*>("tensileFailureStrain_lineEdit");
        if (!tensileFailureStrain_lineEdit)return;
        tensileFailureStrain_lineEdit->setText(QString::number(plasTabData.m_tensileFailureStrain));
        //最大拉伸失效应力
        QLineEdit* maxTensileFailureStrain_lineEdit = this->findChild<QLineEdit*>("maxTensileFailureStrain_lineEdit");
        if (!maxTensileFailureStrain_lineEdit)return;
        maxTensileFailureStrain_lineEdit->setText(QString::number(plasTabData.m_maxTensileFailureStrain));
        //函数数量
        QComboBox* NOF_box = this->findChild<QComboBox*>("NOF_box");
        if (!NOF_box)return;
        int index2 = NOF_box->findData(plasTabData.m_numberOfFunctions);
        NOF_box->setCurrentIndex(index2);
        //平滑应变率选项标志
        QComboBox* smoothStrainRate_box = this->findChild<QComboBox*>("smoothStrainRate_box");
        if (!smoothStrainRate_box)return;
        int index3 = smoothStrainRate_box->findData(plasTabData.m_smoothStrainRateOptFlag);
        smoothStrainRate_box->setCurrentIndex(index3);

        //硬化系数
        QLineEdit* HardeningCoe_lineEdit = this->findChild<QLineEdit*>("HardeningCoe_lineEdit");
        if (!HardeningCoe_lineEdit)return;
        HardeningCoe_lineEdit->setText(QString::number(plasTabData.m_Chard));
        //Cut off Frequency应变率平滑截止频率
        QLineEdit* Fcut_lineEdit = this->findChild<QLineEdit*>("Fcut_lineEdit");
        if (!Fcut_lineEdit)return;
        Fcut_lineEdit->setText(QString::number(plasTabData.m_Fcut));
        //strain rate choice flag
        QComboBox* strainRateChoice_box = this->findChild<QComboBox*>("strainRateChoice_box");
        if (!strainRateChoice_box)return;
        int index4 = strainRateChoice_box->findData(plasTabData.m_vpFlag);
        strainRateChoice_box->setCurrentIndex(index4);


        //元素删除的拉伸应变
        QLineEdit* tensileStrainForElementDeletion_lineEdit = this->findChild<QLineEdit*>("tensileStrainForElementDeletion_lineEdit");
        if (!tensileStrainForElementDeletion_lineEdit)return;
        tensileStrainForElementDeletion_lineEdit->setText(QString::number(plasTabData.m_tensileStrainForElementDeletion));
        //Yield factor versus pressure function
        QComboBox* yieldFactor_box = this->findChild<QComboBox*>("yieldFactor_box");
        if (!yieldFactor_box)return;
        int index1 = yieldFactor_box->findData(plasTabData.m_fctIDp);
        yieldFactor_box->setCurrentIndex(index1);
        //m_fctIDp 缩放因子 [Pa]
        QLineEdit* scaleFactorYieldFactor_lineEdit = this->findChild<QLineEdit*>("scaleFactorYieldFactor_lineEdit");
        if (!scaleFactorYieldFactor_lineEdit)return;
        scaleFactorYieldFactor_lineEdit->setText(QString::number(plasTabData.m_fscaleFactor));
        // Function identifier for the scale factor of Young's modulus,when Young's modulus is function of the plastic strain.
        QLineEdit* factorIdentifier_lineEdit = this->findChild<QLineEdit*>("factorIdentifier_lineEdit");
        if (!factorIdentifier_lineEdit)return;
        factorIdentifier_lineEdit->setText(QString::number(plasTabData.m_fctIODE));
        // Saturated Young's modulus for infinitive plastic strain.
        QLineEdit* saturatedYoungsModulus_lineEdit = this->findChild<QLineEdit*>("saturatedYoungsModulus_lineEdit");
        if (!saturatedYoungsModulus_lineEdit)return;
        saturatedYoungsModulus_lineEdit->setText(QString::number(plasTabData.m_Eint));
        // parameter for the evolution of Young's modulus
        QLineEdit* parameterForTheEvolution_lineEdit = this->findChild<QLineEdit*>("parameterForTheEvolution_lineEdit");
        if (!parameterForTheEvolution_lineEdit)return;
        parameterForTheEvolution_lineEdit->setText(QString::number(plasTabData.m_Ce));

        // Yield Stress Function------Scale Factor------Strain Rate
        for (int i = 0; i < plasTabData.m_numberOfFunctions; ++i)
        {
            QComboBox* yieldStressFunctionIdentifier_box = this->findChild<QComboBox*>(QString("yieldStressFunctionIdentifier_box%1").arg(i+1));
            if (!yieldStressFunctionIdentifier_box)return;
            int index = yieldStressFunctionIdentifier_box->findData(plasTabData.m_fctID.value(i,-1));
            yieldStressFunctionIdentifier_box->setCurrentIndex(index);

            QLineEdit* lineEdit = this->findChild<QLineEdit*>
                (QString("scaleFactorForOrdinate_lineEdit%1").arg(i+1));
            if (!lineEdit)return;
            lineEdit->setText(QString::number(plasTabData.m_fctScale.value(i, 1.0)));

            lineEdit = this->findChild<QLineEdit*>
                (QString("plasticStrainRate_lineEdit%1").arg(i+1));
            if (!lineEdit)return;
            lineEdit->setText(QString::number(plasTabData.m_epsilon.value(i, 0.0)));
        }
    }

    bool GUIMaterialPlasTabDialog::ckeckData()
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
        ////硬化系数
        //QLineEdit* HardeningCoe_lineEdit = this->findChild<QLineEdit*>("HardeningCoe_lineEdit");
        //if (!HardeningCoe_lineEdit)return false;
        //if (HardeningCoe_lineEdit->text().toDouble() > 1) {
        //    QMessageBox::warning(this, "", tr("The Hardening coefficient ranges from 0 to 1"), QMessageBox::Ok);
        //    return false;
        //};
        return true;
    }

    void GUIMaterialPlasTabDialog::getDataFormWidget()
    {
        Radioss::FITKMaterialPlasTab* plasTab = dynamic_cast<Radioss::FITKMaterialPlasTab*>(_obj);
        if (!plasTab) return;
        Radioss::FITKMaterialPlasTabData &plasTabData = plasTab->getMaterialData();
        //材料密度
        QLineEdit* density_lineEdit = this->findChild<QLineEdit*>("density_lineEdit");
        if (!density_lineEdit)return;
        plasTabData.m_density= density_lineEdit->text().toDouble();
        //杨氏模量
        QLineEdit* youngsModulus_lineEdit = this->findChild<QLineEdit*>("youngsModulus_lineEdit");
        if (!youngsModulus_lineEdit)return;
        plasTabData.m_youngsModulus = youngsModulus_lineEdit->text().toDouble();
        //泊松比
        QLineEdit* poissonsRatio_lineEdit = this->findChild<QLineEdit*>("poissonsRatio_lineEdit");
        if (!poissonsRatio_lineEdit)return;
        plasTabData.m_poissonsRatio = poissonsRatio_lineEdit->text().toDouble();
        
        //失效应变
        QLineEdit* failureStrain_lineEdit = this->findChild<QLineEdit*>("failureStrain_lineEdit");
        if (!failureStrain_lineEdit)return;
        plasTabData.m_failureStrain = failureStrain_lineEdit->text().toDouble();
        //拉伸失效应变
        QLineEdit* tensileFailureStrain_lineEdit = this->findChild<QLineEdit*>("tensileFailureStrain_lineEdit");
        if (!tensileFailureStrain_lineEdit)return;
        plasTabData.m_tensileFailureStrain = tensileFailureStrain_lineEdit->text().toDouble();
        //最大拉伸失效应力
        QLineEdit* maxTensileFailureStrain_lineEdit = this->findChild<QLineEdit*>("maxTensileFailureStrain_lineEdit");
        if (!maxTensileFailureStrain_lineEdit)return;
        plasTabData.m_maxTensileFailureStrain = maxTensileFailureStrain_lineEdit->text().toDouble();

        //函数数量
        QComboBox* NOF_box = this->findChild<QComboBox*>("NOF_box");
        if (!NOF_box)return;
        plasTabData.m_numberOfFunctions= NOF_box->currentData().toInt();

        //平滑应变率选项标志
        QComboBox* smoothStrainRate_box = this->findChild<QComboBox*>("smoothStrainRate_box");
        if (!smoothStrainRate_box)return;
        plasTabData.m_smoothStrainRateOptFlag = smoothStrainRate_box->currentData().toInt();

        //硬化系数
        QLineEdit* HardeningCoe_lineEdit = this->findChild<QLineEdit*>("HardeningCoe_lineEdit");
        if (!HardeningCoe_lineEdit)return;
        plasTabData.m_Chard = HardeningCoe_lineEdit->text().toDouble();

        //Cut off Frequency应变率平滑截止频率
        QLineEdit* Fcut_lineEdit = this->findChild<QLineEdit*>("Fcut_lineEdit");
        if (!Fcut_lineEdit)return;
        plasTabData.m_Fcut = Fcut_lineEdit->text().toDouble();

        //strain rate choice flag
        QComboBox* strainRateChoice_box = this->findChild<QComboBox*>("strainRateChoice_box");
        if (!strainRateChoice_box)return;
        plasTabData.m_vpFlag = strainRateChoice_box->currentData().toInt();

        //元素删除的拉伸应变
        QLineEdit* tensileStrainForElementDeletion_lineEdit = this->findChild<QLineEdit*>("tensileStrainForElementDeletion_lineEdit");
        if (!tensileStrainForElementDeletion_lineEdit)return;
        plasTabData.m_tensileStrainForElementDeletion = tensileStrainForElementDeletion_lineEdit->text().toDouble();

        //Yield factor versus pressure function
        QComboBox* yieldFactor_box = this->findChild<QComboBox*>("yieldFactor_box");
        if (!yieldFactor_box)return;
        plasTabData.m_fctIDp = yieldFactor_box->currentData().toInt();

        //m_fctIDp 缩放因子 [Pa]
        QLineEdit* scaleFactorYieldFactor_lineEdit = this->findChild<QLineEdit*>("scaleFactorYieldFactor_lineEdit");
        if (!scaleFactorYieldFactor_lineEdit)return;
        plasTabData.m_fscaleFactor = scaleFactorYieldFactor_lineEdit->text().toDouble();

        // Function identifier for the scale factor of Young's modulus,when Young's modulus is function of the plastic strain.
        QLineEdit* factorIdentifier_lineEdit = this->findChild<QLineEdit*>("factorIdentifier_lineEdit");
        if (!factorIdentifier_lineEdit)return;
        plasTabData.m_fctIODE = factorIdentifier_lineEdit->text().toDouble();

        // Saturated Young's modulus for infinitive plastic strain.
        QLineEdit* saturatedYoungsModulus_lineEdit = this->findChild<QLineEdit*>("saturatedYoungsModulus_lineEdit");
        if (!saturatedYoungsModulus_lineEdit)return;
        plasTabData.m_Eint = saturatedYoungsModulus_lineEdit->text().toDouble();

        // parameter for the evolution of Young's modulus
        QLineEdit* parameterForTheEvolution_lineEdit = this->findChild<QLineEdit*>("parameterForTheEvolution_lineEdit");
        if (!parameterForTheEvolution_lineEdit)return;
        plasTabData.m_Ce = parameterForTheEvolution_lineEdit->text().toDouble();

        // Yield Stress Function------Scale Factor------Strain Rate
        plasTabData.m_fctID.clear();
        plasTabData.m_fctScale.clear();
        plasTabData.m_epsilon.clear();
        for (int i = 1; i <= plasTabData.m_numberOfFunctions; ++i) 
        {
            QComboBox* yieldStressFunctionIdentifier_box = this->findChild<QComboBox*>(QString("yieldStressFunctionIdentifier_box%1").arg(i));
            if (!yieldStressFunctionIdentifier_box)return;
            int fctID = yieldStressFunctionIdentifier_box->currentData().toInt();
            plasTabData.m_fctID.append(fctID);

            QLineEdit* lineEdit = this->findChild<QLineEdit*>
                (QString("scaleFactorForOrdinate_lineEdit%1").arg(i));
            if (!lineEdit)return;
            double scaleFactor= lineEdit->text().toDouble();
            plasTabData.m_fctScale.append(scaleFactor);

            lineEdit = this->findChild<QLineEdit*>
                (QString("plasticStrainRate_lineEdit%1").arg(i));
            if (!lineEdit)return;
            double strainRate = lineEdit->text().toDouble();
            plasTabData.m_epsilon.append(strainRate);
        }
    }

    void GUIMaterialPlasTabDialog::writePythonScript()
    {
        Radioss::FITKMaterialPlasTab* plasTab = dynamic_cast<Radioss::FITKMaterialPlasTab*>(_obj);
        if (plasTab == nullptr)
        {
            return;
        }

        Radioss::FITKMaterialPlasTabData plasTabData = plasTab->getMaterialDataReadOnly();

        Radioss::FITKNonLinerCurveManager* curveManager = nullptr;
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData != nullptr)
        {
            curveManager = caseData->getNonLinerCurveManager();
        }

        QStringList script;
        if (_isCreate)
        {
            script.append("m = Material.PlasTab()");
            script.append(QString("m.createMaterial('%1')").arg(plasTab->getDataObjectName()));
        }
        else
        {
            QString oldName = _oldNameForScript.isEmpty() ? plasTab->getDataObjectName() : _oldNameForScript;
            script.append(QString("m = Material.PlasTab.GetPlasTab('%1')").arg(oldName));
            if (oldName != plasTab->getDataObjectName())
            {
                script.append(QString("m.setName('%1')").arg(plasTab->getDataObjectName()));
            }
        }

        script.append(QString("m.setDensity(%1)").arg(plasTabData.m_density));
        script.append(QString("m.setYoungsModulus(%1)").arg(plasTabData.m_youngsModulus));
        script.append(QString("m.setPoissonsRatio(%1)").arg(plasTabData.m_poissonsRatio));
        script.append(QString("m.setFailureStrain(%1)").arg(plasTabData.m_failureStrain));
        script.append(QString("m.setTensileFailureStrain(%1)").arg(plasTabData.m_tensileFailureStrain));
        script.append(QString("m.setMaxTensileFailureStrain(%1)").arg(plasTabData.m_maxTensileFailureStrain));
        script.append(QString("m.setNumberOfFunctions(%1)").arg(plasTabData.m_numberOfFunctions));
        script.append(QString("m.setSmoothStrainRateOptFlag(%1)").arg(plasTabData.m_smoothStrainRateOptFlag));
        script.append(QString("m.setChard(%1)").arg(plasTabData.m_Chard));
        script.append(QString("m.setFcut(%1)").arg(plasTabData.m_Fcut));
        script.append(QString("m.setVpFlag(%1)").arg(plasTabData.m_vpFlag));
        script.append(QString("m.setTensileStrainForElementDeletion(%1)").arg(plasTabData.m_tensileStrainForElementDeletion));
        script.append(QString("m.setFctIDp(%1)").arg(plasTabData.m_fctIDp));
        script.append(QString("m.setFscaleFactor(%1)").arg(plasTabData.m_fscaleFactor));
        script.append(QString("m.setFctIODE(%1)").arg(plasTabData.m_fctIODE));
        script.append(QString("m.setEint(%1)").arg(plasTabData.m_Eint));
        script.append(QString("m.setCe(%1)").arg(plasTabData.m_Ce));

        for (int i = 0; i < plasTabData.m_numberOfFunctions; ++i)
        {
            int curveId = plasTabData.m_fctID.value(i, -1);
            if (curveManager != nullptr)
            {
                Radioss::FITKNonLinerCurve* curve = curveManager->getDataByID(curveId);
                if (curve != nullptr)
                {
                    script.append(QString("m.setFunction(%1, '%2')").arg(i + 1).arg(curve->getDataObjectName()));
                }
            }

            script.append(QString("m.setFctScale(%1, %2)").arg(i + 1).arg(plasTabData.m_fctScale.value(i, 1.0)));
            script.append(QString("m.setEpsilon(%1, %2)").arg(i + 1).arg(plasTabData.m_epsilon.value(i, 0.0)));
        }

        this->saveScript(script);
    }

    void GUIMaterialPlasTabDialog::setCurveBox(QComboBox * curveBox,bool isDefault)
    {
        if (!curveBox)return;
        // 获取 Radioss Case
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return;
        // 获取曲线管理器
        Radioss::FITKNonLinerCurveManager* curveManager = caseData->getNonLinerCurveManager();
        if (curveManager == nullptr) return;

        if(isDefault)
            curveBox->addItem("Default",0);

        for (int i = 0; i < curveManager->getDataCount();++i) {
            Radioss::FITKNonLinerCurve* curve = curveManager->getDataByIndex(i);
            if (!curve)continue;
            curveBox->addItem(curve->getDataObjectName(),curve->getDataObjectID());
        }
    }
    void GUIMaterialPlasTabDialog::updateFuncSlot(int index)
    {
        QList<QTreeWidgetItem*> items = _ui->treeWidget_parameter->
            findItems("YieldStressFunction_ScaleFactor_StrainRate", Qt::MatchContains | Qt::MatchRecursive );
        if (items.isEmpty())return;

        //原始数量
        QTreeWidgetItem* YSS_item = items[0];
        int count = YSS_item->childCount();

        if (index > count-1) {
            for (int i = count + 1; i <= index+1; ++i) {
                QTreeWidgetItem*  func_item = new QTreeWidgetItem(YSS_item);
                func_item->setText(0, QString("func-%1").arg(i));
                func_item->setExpanded(true);

                QWidget* plastic_widget1 = new QWidget(this);
                QHBoxLayout* plastic_layout1 = new QHBoxLayout();
                QComboBox* curve_box1 = new QComboBox(this);
                this->setCurveBox(curve_box1,false);
                curve_box1->setCurrentIndex(-1);
                curve_box1->setObjectName(QString("yieldStressFunctionIdentifier_box%1").arg(i));
                QLabel* plastic_label = new QLabel(this);
                plastic_label->setText("");
                plastic_layout1->addWidget(curve_box1);
                plastic_layout1->addWidget(plastic_label);
                plastic_layout1->setStretch(0, 1);
                plastic_layout1->setStretch(1, 1);
                plastic_widget1->setLayout(plastic_layout1);
                QTreeWidgetItem* inputMethod_item = new QTreeWidgetItem(func_item);
                inputMethod_item->setText(0, tr("Yield Stress Function Identifier"));
                _ui->treeWidget_parameter->setItemWidget(inputMethod_item, 1, plastic_widget1);

                this->addChiledItem(func_item, QString("scaleFactorForOrdinate_lineEdit%1").arg(i), tr("Scale Factor For Ordinate(stress)"), "");
                QLineEdit* factorIdentifier_lineEdit = this->findChild<QLineEdit*>(QString("scaleFactorForOrdinate_lineEdit%1").arg(i));
                factorIdentifier_lineEdit->setText("1");
                this->addChiledItem(func_item, QString("plasticStrainRate_lineEdit%1").arg(i), tr("Plastic/Total Strain Rate"), "");
                QLineEdit* plasticStrainRate_lineEdit = this->findChild<QLineEdit*>(QString("plasticStrainRate_lineEdit%1").arg(i));
                plasticStrainRate_lineEdit->setText("0");
            }
        }
        else if (index < count-1) {
            int deletCount = count - 1 - index;
            for (int i=0; i < deletCount; ++i) 
            {
                QTreeWidgetItem*  func_item = YSS_item->takeChild(index+1);
                if (func_item) {
                    delete func_item;
                    func_item = nullptr;
                }
            }
        }
    }
}