/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIEOSDialog.h"
#include "ui_GUIEOSDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialHydro.h"

namespace GUI
{
    GUIEOSDialog::GUIEOSDialog(Radioss::FITKAbstractEquationOfState * obj, Core::FITKActionOperator * oper, QWidget * parent)
        : Core::FITKDialog(parent), _obj(obj), _oper(oper), _ui(new Ui::GUIEOSDialog)
    {
        _ui->setupUi(this);
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        //获取管理器
        _EOSMgr = caseObj->getEquationOfState();
        if (!_obj)
            _isCreate = true;
        //设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    }
    GUIEOSDialog::~GUIEOSDialog()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
    }
    void GUIEOSDialog::init()
    {
    }
    void GUIEOSDialog::createTable(const QStringList& headerLabels)
    {
         _ui->tableWidget->clear();
         _ui->tableWidget->setColumnCount(0);
         _ui->tableWidget->setRowCount(0);
         _ui->tableWidget->setColumnCount(headerLabels.size());
         _ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
         _ui->tableWidget->setRowCount(1);
         for (int i = 0;i < headerLabels.size();i++)
         {
             _ui->tableWidget->setItem(0, i, new QTableWidgetItem("0"));
             //记录变量名(列表头)和列号映射
             _colMap.insert(headerLabels[i], i);
         }
         //隐藏行号
         _ui->tableWidget->verticalHeader()->setVisible(false);
    }
    QTableWidgetItem* GUIEOSDialog::getItemByVarName(const QString& varName)
    {
        if (!_colMap.contains(varName)) return nullptr;
        int col = _colMap.value(varName);
        return _ui->tableWidget->item(0, col);
    }
    void GUIEOSDialog::setItemValueByVarName(const QString& varName, double value)
    {
        QTableWidgetItem* item = this->getItemByVarName(varName);
        if (!item) return;
        item->setText(QString::number(value));
    }
    double GUIEOSDialog::getItemValueByVarName(const QString& varName, bool* ok)
    {
        QTableWidgetItem* item = this->getItemByVarName(varName);
        if (!item) return 0.0;
        return item->text().toDouble(ok);
    }
    bool GUIEOSDialog::createObjData()
    {
        if (!_EOSMgr) return false;
        if (!editObjData())return false;
        _EOSMgr->appendDataObj(_obj);
        return true;
    }
    bool GUIEOSDialog::editObjData()
    {
        if (!_oper || !_obj || !_EOSMgr) return false;
        if (!this->ckeckData()) return false;

        _oldNameForScript = _obj->getDataObjectName();
        //设置名称
        _obj->setDataObjectName(_ui->lineEdit_name->text());
        //设置界面数据
        this->getDataFormWidget();

        _oper->setArgs("objID", _obj->getDataObjectID());
        return true;
    }
    bool GUIEOSDialog::ckeckData()
    {
        //虚函数重写
        return true;
    }

    void GUIEOSDialog::writePythonScript()
    {
        //虚函数重写
    }

    void GUIEOSDialog::reject()
    {
        //关闭窗口
        _oper->setArgs("objID", -1);
        if (_isCreate) {
            //删除对象
            if (_obj) delete _obj;
            _obj = nullptr;
        }
        QDialog::reject();
    }
    void GUIEOSDialog::on_pushButtonOK_clicked()
    {
        //创建动作
        if (_isCreate)
        {
            if (!this->createObjData())
                return;
            _oper->execProfession();
        }
        //修改动作
        else
        {
            if (!this->editObjData())
                return;
            _oper->execProfession();
        }
        this->writePythonScript();
        this->accept();
    }
    void GUIEOSDialog::on_pushButtonCancel_clicked()
    {
        //点击取消按键动作
        this->reject();
    }
}