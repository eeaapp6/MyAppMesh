/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIMaterialElasticDialog.h"
#include "ui_GUIMaterialPropDialog.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialElastic.h"

#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QComboBox>

namespace GUI
{
    GUIMaterialElasticDialog::GUIMaterialElasticDialog(Interface::FITKAbstractMaterial* obj, Core::FITKActionOperator* oper, QWidget* parent)
        : GUIMaterialPropDialog(obj, oper, parent)
    {
        QString MaterialName;
        _oper->argValue("MaterialName", MaterialName);
        if (!_obj) {
            _ui->lineEdit_name->setReadOnly(true);
            _obj = new Radioss::FITKMaterialElastic();
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

    void GUIMaterialElasticDialog::initUI()
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


        _ui->treeWidget_parameter->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    void GUIMaterialElasticDialog::init()
    {
        Radioss::FITKMaterialElastic* elastic = dynamic_cast<Radioss::FITKMaterialElastic*>(_obj);
        if (!elastic) return;
        //通用初始化
        this->GUIMaterialPropDialog::init();

        _ui->lineEdit_matID->setReadOnly(true);
        _ui->lineEdit_model->setReadOnly(true);
        //设置名称
        _ui->lineEdit_name->setText(elastic->getDataObjectName());
        //设置材料ID
        _ui->lineEdit_matID->setText(elastic->GetFITKMaterialElasticRadiossKeyWord());
        //设置材料类型
        _ui->lineEdit_model->setText("ELAST");
        //材料密度
        QLineEdit* density_lineEdit = this->findChild<QLineEdit*>("density_lineEdit");
        if (!density_lineEdit)return;
        density_lineEdit->setText(QString::number(elastic->getDensity()));
        //杨氏模量
        QLineEdit* youngsModulus_lineEdit=this->findChild<QLineEdit*>("youngsModulus_lineEdit");
        if (!youngsModulus_lineEdit)return;
        youngsModulus_lineEdit->setText(QString::number(elastic->getYoungsModulus()));
        //泊松比
        QLineEdit* poissonsRatio_lineEdit = this->findChild<QLineEdit*>("poissonsRatio_lineEdit");
        if (!poissonsRatio_lineEdit)return;
        poissonsRatio_lineEdit->setText(QString::number(elastic->getPoissonsRatio()));
        
    }

    bool GUIMaterialElasticDialog::ckeckData()
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

    void GUIMaterialElasticDialog::getDataFormWidget()
    {
        Radioss::FITKMaterialElastic* elastic = dynamic_cast<Radioss::FITKMaterialElastic*>(_obj);
        if (!elastic) return;
        
        //材料密度
        QLineEdit* density_lineEdit = this->findChild<QLineEdit*>("density_lineEdit");
        if (!density_lineEdit)return;
        double density= density_lineEdit->text().toDouble();
        elastic->setDensity(density);
        //杨氏模量
        QLineEdit* youngsModulus_lineEdit = this->findChild<QLineEdit*>("youngsModulus_lineEdit");
        if (!youngsModulus_lineEdit)return;
        double youngsModulus = youngsModulus_lineEdit->text().toDouble();
        elastic->setYoungsModulus(youngsModulus);
        //泊松比
        QLineEdit* poissonsRatio_lineEdit = this->findChild<QLineEdit*>("poissonsRatio_lineEdit");
        if (!poissonsRatio_lineEdit)return;
        double poissonsRatio = poissonsRatio_lineEdit->text().toDouble();
        elastic->setPoissonsRatio(poissonsRatio);
    }

    void GUIMaterialElasticDialog::writePythonScript()
    {
        Radioss::FITKMaterialElastic* elastic = dynamic_cast<Radioss::FITKMaterialElastic*>(_obj);
        if (elastic == nullptr)
        {
            return;
        }

        QStringList script;
        if (_isCreate)
        {
            script.append("m = Material.Elastic()");
            script.append(QString("m.createMaterial('%1')").arg(elastic->getDataObjectName()));
        }
        else
        {
            QString oldName = _oldNameForScript.isEmpty() ? elastic->getDataObjectName() : _oldNameForScript;
            script.append(QString("m = Material.Elastic.GetElastic('%1')").arg(oldName));
            if (oldName != elastic->getDataObjectName())
            {
                script.append(QString("m.setName('%1')").arg(elastic->getDataObjectName()));
            }
        }

        script.append(QString("m.setDensity(%1)").arg(elastic->getDensity()));
        script.append(QString("m.setYoungsModulus(%1)").arg(elastic->getYoungsModulus()));
        script.append(QString("m.setPoissonsRatio(%1)").arg(elastic->getPoissonsRatio()));

        this->saveScript(script);
    }
}