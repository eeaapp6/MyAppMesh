/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "DeformDialog.h"
#include "ui_DeformDialog.h"

#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostDeform.h"

#include <QMessageBox>

namespace GUI
{
    DeformDialog::DeformDialog(int dataID, EventOper::ParaWidgetInterfaceOperator * oper, bool isCreate) :
        GUIDialogBase(oper,"DeformDialog", FITKAPP->getGlobalData()->getMainWindow()),
        _isCreate(isCreate)
    {
        _ui = new Ui::DeformDialog();
        _ui->setupUi(this);
        if (isCreate) {
            _parentDataID = dataID;
        }
        else {
            _currentDataID = dataID;
            if (_postDataManager->getDataByID(_currentDataID)) {
                _parentDataID = _postDataManager->getDataByID(_currentDataID)->getCFDPostParentID();
            }
        }
        init();
    }

    DeformDialog::~DeformDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void DeformDialog::init()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;

        //添加变量
        QStringList pointDataArray = parentData->getPointDataArray();
        for (QString pointData : pointDataArray) {
            Interface::FITKPostVariableType type = parentData->getVariableType(Interface::FITKPostFieldType::Post_Point, pointData);
            if (type == Interface::FITKPostVariableType::Post_Error || type == Interface::FITKPostVariableType::Post_Scalar)continue;
            _ui->comboBox_Vectors->addItem(pointData);
        }

        if (_isCreate) {
            InitCreate();
            setWindowTitle(tr("Deform Create"));
        }
        else {
            InitEdit();
            setWindowTitle(tr("Deform Edit"));
        }
    }

    void DeformDialog::on_pushButton_OK_clicked()
    {
        if (_isCreate) {
            if (!OKCreate())return;
        }
        else {
            if (!OKEdit())return;
        }

        if (_oper) {
            _oper->setArgs(objID_String, _currentDataID);
            _oper->execProfession();
        }

        this->close();
    }

    void DeformDialog::on_pushButton_Cancel_clicked()
    {
        this->close();
    }

    void DeformDialog::InitCreate()
    {
        QString name = _postDataManager->checkName("Deform-0");
        _ui->lineEdit_Name->setText(name);
        _ui->comboBox_Vectors->setCurrentIndex(0);
        _ui->lineEdit_ScaleFactor->setValue(0);
    }

    void DeformDialog::InitEdit()
    {
        Interface::FITKCFDPostDeform* currentData = dynamic_cast<Interface::FITKCFDPostDeform*>(_postDataManager->getDataByID(_currentDataID));
        if (currentData == nullptr)return;
        QString name = currentData->getDataObjectName();
        _ui->lineEdit_Name->setText(name);
        _ui->lineEdit_Name->setEnabled(false);

        setDataToWidget(currentData);
    }

    bool DeformDialog::OKCreate()
    {
        QString name = _ui->lineEdit_Name->text();
        if (_postDataManager->getDataByName(name)) {
            QMessageBox box(QMessageBox::Warning, "", QString(tr("\"%1\" Name exists!")).arg(name), QMessageBox::Ok);
            box.exec();
            return false;
        }

        Interface::FITKCFDPostDeform* currentData = new Interface::FITKCFDPostDeform(_parentDataID);
        currentData->setDataObjectName(name);
        getDataFromWidget(currentData);
        _currentDataID = currentData->getDataObjectID();

        _postDataManager->appendDataObj(currentData);
        return true;
    }

    bool DeformDialog::OKEdit()
    {
        Interface::FITKCFDPostDeform* currentData = dynamic_cast<Interface::FITKCFDPostDeform*>(_postDataManager->getDataByID(_currentDataID));
        if (currentData == nullptr)return false;
        getDataFromWidget(currentData);
        return true;
    }

    void DeformDialog::getDataFromWidget(Interface::FITKCFDPostDeform* data)
    {
        if (data == nullptr)return;
        QString vectors = _ui->comboBox_Vectors->currentText();
        data->setDeformVariable(vectors);

        double factor = _ui->lineEdit_ScaleFactor->value();
        data->setDeformFactor(factor);
    }

    void DeformDialog::setDataToWidget(Interface::FITKCFDPostDeform* data)
    {
        if (data == nullptr)return;

        QString vectors = data->getDeformVariable();
        _ui->comboBox_Vectors->setCurrentText(vectors);

        double factor = data->getDeformFactor();
        _ui->lineEdit_ScaleFactor->setValue(factor);
    }
}