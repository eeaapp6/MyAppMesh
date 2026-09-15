/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPropertyDialog.h"
#include "ui_GUIPropertyDialog.h"
#include "FITK_Component/FITKWidget/FITKSciNotationLineEdit.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKPropAbstract.hpp"
#include <QComboBox>

namespace GUI
{
    GUIPropertyDialog::GUIPropertyDialog(Core::FITKActionOperator * oper, Radioss::FITKPropAbstract * obj, QWidget * parent)
        : _oper(oper), _ui(new Ui::GUIPropertyDialog), Core::FITKDialog(parent), _obj(obj)
    {
        _ui->setupUi(this);
        //去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        // 设置对话框标题
        setWindowTitle(tr("Analysis Property"));
        //setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸
    }

    GUIPropertyDialog::~GUIPropertyDialog()
    {
        // 释放UI
        if (_ui) delete _ui;
        _ui = nullptr;
    }

    void GUIPropertyDialog::init()
    {
        //虚函数
    }

    bool GUIPropertyDialog::ckeckData()
    {
        //虚函数
        return false;
    }

    void GUIPropertyDialog::addLineEdit(const QString & label, const QString & objname)
    {
        int row = _ui->tableWidgetValue->rowCount();
        _ui->tableWidgetValue->insertRow(row);
        //行标题不可编辑
        QTableWidgetItem* item = new QTableWidgetItem(label);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        _ui->tableWidgetValue->setItem(row, 0, item);

        Comp::FITKSciNotationLineEdit* lineEdit = new Comp::FITKSciNotationLineEdit(this);
        lineEdit->setObjectName(objname);
        //lineEdit->setStyleSheet("border: none;");

        _ui->tableWidgetValue->setCellWidget(row, 1, lineEdit);
    }
    QComboBox * GUIPropertyDialog::addComboBox(const QString & label, const QString & objname)
    {
        int row = _ui->tableWidgetValue->rowCount();
        _ui->tableWidgetValue->insertRow(row);
        //行标题不可编辑
        QTableWidgetItem* item = new QTableWidgetItem(label);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        _ui->tableWidgetValue->setItem(row, 0, item);

        QComboBox* comBobox = new QComboBox(this);
        comBobox->setObjectName(objname);
        //comBobox->setStyleSheet("border: none;");

        _ui->tableWidgetValue->setCellWidget(row, 1, comBobox);
        return comBobox;
    }

    void GUIPropertyDialog::writePythonScript()
    {
        //虚函数重写
    }

    Interface::FITKAbstractSectionManager *GUIPropertyDialog::getSectionManager()
    {
        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return nullptr;

        // 获取属性管理器
        return dataCase->getSectionManager();
    }
    void GUIPropertyDialog::on_pushButtonOK_clicked()
    {
        auto mgr = this->getSectionManager();
        if (!_oper || !_obj || !mgr) return;
        if (!this->ckeckData()) return;

        _oldNameForScript = _obj->getDataObjectName();
        //设置名称
        _obj->setDataObjectName(_ui->lineEditName->text());
        //设置界面数据
        this->getDataFormWidget();

        //创建动作
        if (!_isEditMode)
        {
            mgr->appendDataObj(_obj);
        }
        //_oper->setArgs("PropID", _obj->getDataObjectID());
        _oper->execProfession();

        writePythonScript();
        this->accept();
    }
    void GUIPropertyDialog::on_pushButtonCancel_clicked()
    {
        //关闭窗口
       //_oper->setArgs("PropID", -1);
        if (!_isEditMode) {
            //删除对象
            if (_obj) delete _obj;
            _obj = nullptr;
        }
        QDialog::reject();
    }
}