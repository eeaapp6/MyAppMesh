/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIEOSPolynomialDialog.h"
#include "ui_GUIEOSDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKEquationOfStatePolynomial.h"

#include <QMessageBox>

namespace GUI
{
    GUIEOSPolynomialDialog::GUIEOSPolynomialDialog(Radioss::FITKAbstractEquationOfState* obj, Core::FITKActionOperator* oper, QWidget* parent)
        : GUIEOSDialog(obj, oper, parent)
    {
        QString EOSName;
        _oper->argValue("EOSName", EOSName);
        if (!_obj) {
            _ui->lineEdit_name->setReadOnly(true);
            _obj = new Radioss::FITKEquationOfStatePolynomial();
            _obj->setDataObjectName(EOSName);
            setWindowTitle(tr("Create EOS"));
        }
        else
            setWindowTitle(tr("Edit EOS"));

        this->init();
    }
    void GUIEOSPolynomialDialog::init()
    {
        //初始化表格
        QStringList headerLabels;
        headerLabels << "C0" << "C1" << "C2" << "C3" << "C4" << "C5" << "E0" << "Psh"<< "RHO_0";
        this->createTable(headerLabels);
        Radioss::FITKEquationOfStatePolynomial* eosPoly = dynamic_cast<Radioss::FITKEquationOfStatePolynomial*>(_obj);
        if (!eosPoly) return;
        //设置名称类型
        _ui->lineEditType->setReadOnly(true);
        _ui->lineEdit_name->setText(eosPoly->getDataObjectName());
        _ui->lineEditType->setText("/EOS/POLYNOMIAL(Polynomial)");

        //设置参数
        this->setItemValueByVarName("C0", eosPoly->getC0());
        this->setItemValueByVarName("C1", eosPoly->getC1());
        this->setItemValueByVarName("C2", eosPoly->getC2());
        this->setItemValueByVarName("C3", eosPoly->getC3());
        this->setItemValueByVarName("C4", eosPoly->getC4());
        this->setItemValueByVarName("C5", eosPoly->getC5());
        this->setItemValueByVarName("E0", eosPoly->getE0());
        this->setItemValueByVarName("Psh", eosPoly->getPsh());
        this->setItemValueByVarName("RHO_0", eosPoly->getP0());

        //调整列宽
        _ui->tableWidget->resizeColumnsToContents();
        for (int i = 0; i < _ui->tableWidget->columnCount(); ++i)
        {
            int currentWidth = _ui->tableWidget->columnWidth(i);
            if (currentWidth < 60) 
                _ui->tableWidget->setColumnWidth(i, 60);
            else if (currentWidth > 100)
                _ui->tableWidget->setColumnWidth(i, 100);
        }
    }
    bool GUIEOSPolynomialDialog::ckeckData()
    {
        if(!_EOSMgr||!_obj)return false;
        //检查数据合法性
        bool ok = false;
        QStringList headerLabels;
        headerLabels << "C0" << "C1" << "C2" << "C3" << "C4" << "C5" << "E0" << "Psh" << "RHO_0";
        for (int i = 0; i < headerLabels.size();++i)
        {
            double value = this->getItemValueByVarName(headerLabels[i], &ok);
            if (!ok){
                QMessageBox::warning(this, "", tr("Please input a valid number for %1.").arg(headerLabels[i]), QMessageBox::Ok);
                return false;
            }
        }
        //检查名称
        QString name = _ui->lineEdit_name->text();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "", tr("Name cannot be empty."), QMessageBox::Ok);
            return false;
        }
        else if (_EOSMgr->getDataByName(name) && name != _obj->getDataObjectName()) {
            QMessageBox::warning(this, "", tr("\"%1\" The name already exists").arg(name), QMessageBox::Ok);
            return false;
        }
        return true;

    }
    void GUIEOSPolynomialDialog::getDataFormWidget()
    {
        Radioss::FITKEquationOfStatePolynomial* eosPoly = dynamic_cast<Radioss::FITKEquationOfStatePolynomial*>(_obj);
        if (!eosPoly) return;

        //设置参数
        eosPoly->setC0(this->getItemValueByVarName("C0"));
        eosPoly->setC1(this->getItemValueByVarName("C1"));
        eosPoly->setC2(this->getItemValueByVarName("C2"));
        eosPoly->setC3(this->getItemValueByVarName("C3"));
        eosPoly->setC4(this->getItemValueByVarName("C4"));
        eosPoly->setC5(this->getItemValueByVarName("C5"));
        eosPoly->setE0(this->getItemValueByVarName("E0"));
        eosPoly->setPsh(this->getItemValueByVarName("Psh"));
        eosPoly->setP0(this->getItemValueByVarName("RHO_0"));
    }

    void GUIEOSPolynomialDialog::writePythonScript()
    {
        Radioss::FITKEquationOfStatePolynomial* eosPoly =
            dynamic_cast<Radioss::FITKEquationOfStatePolynomial*>(_obj);
        if (eosPoly == nullptr)
        {
            return;
        }

        QStringList script;
        if (_isCreate)
        {
            script.append("e=EOS.EOSPolynomial()");
            script.append(QString("e.createEquationOfState('%1')").arg(eosPoly->getDataObjectName()));
        }
        else
        {
            QString oldName = _oldNameForScript.isEmpty() ? eosPoly->getDataObjectName() : _oldNameForScript;
            script.append(QString("e=EOS.EOSPolynomial.GetEOSPolynomial('%1')").arg(oldName));
            if (oldName != eosPoly->getDataObjectName())
            {
                script.append(QString("e.setName('%1')").arg(eosPoly->getDataObjectName()));
            }
        }

        script.append(QString("e.setC0(%1)").arg(eosPoly->getC0()));
        script.append(QString("e.setC1(%1)").arg(eosPoly->getC1()));
        script.append(QString("e.setC2(%1)").arg(eosPoly->getC2()));
        script.append(QString("e.setC3(%1)").arg(eosPoly->getC3()));
        script.append(QString("e.setC4(%1)").arg(eosPoly->getC4()));
        script.append(QString("e.setC5(%1)").arg(eosPoly->getC5()));
        script.append(QString("e.setE0(%1)").arg(eosPoly->getE0()));
        script.append(QString("e.setPsh(%1)").arg(eosPoly->getPsh()));
        script.append(QString("e.setP0(%1)").arg(eosPoly->getP0()));

        this->saveScript(script);
    }
}