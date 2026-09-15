/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ThresholdDialog.h"
#include "ui_ThresholdDialog.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "PostGraphAdaptor/PostGraphWidgetLine.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostThreshold.h"

#include <QMessageBox>

Q_DECLARE_METATYPE(Interface::FITKPostFieldType)

namespace GUI
{
    ThresholdDialog::ThresholdDialog(int dataID, EventOper::ParaWidgetInterfaceOperator * oper, bool isCreate):
        GUIDialogBase(oper,"ThresholdDialog", FITKAPP->getGlobalData()->getMainWindow()), _isCreate(isCreate)
    {
        _ui = new Ui::ThresholdDialog();
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

    ThresholdDialog::~ThresholdDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void ThresholdDialog::init()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;

        //设置场量
        QStringList pointData = parentData->getPointDataArray();
        for (auto array : pointData) {
            Interface::FITKPostVariableType variableType = parentData->getVariableType(Interface::FITKPostFieldType::Post_Point, array);
            if (variableType != Interface::FITKPostVariableType::Post_Scalar)continue;
            _ui->comboBox_Type->addItem(QIcon("://icons/nodeValue.png"), array, QVariant::fromValue(Interface::FITKPostFieldType::Post_Point));
        }

        QStringList cellData = parentData->getCellDataArray();
        for (auto array : cellData) {
            Interface::FITKPostVariableType variableType = parentData->getVariableType(Interface::FITKPostFieldType::Post_Cell, array);
            if (variableType != Interface::FITKPostVariableType::Post_Scalar)continue;
            _ui->comboBox_Type->addItem(QIcon("://icons/cellValue.png"), array, QVariant::fromValue(Interface::FITKPostFieldType::Post_Cell));
        }

        if (_isCreate) {
            InitCreate();
            setWindowTitle(tr("Threshold Create"));
        }
        else {
            InitEdit();
            setWindowTitle(tr("Threshold Edit"));
        }
    }

    void ThresholdDialog::on_pushButton_OK_clicked()
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

    void ThresholdDialog::on_pushButton_Cancel_clicked()
    {
        this->close();
    }

    void ThresholdDialog::on_comboBox_Type_currentIndexChanged(int index)
    {
        Q_UNUSED(index);
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;

        Interface::FITKPostFieldType fieldType = Interface::FITKPostFieldType::Post_None;
        QString fieldName = "";
        double range[2] = { 0,0 };
        getCurrentField(fieldType, fieldName);
        parentData->getRange(range, fieldType, fieldName);
        _ui->label_ValueRange->setText(QString("[%1,%2]").arg(range[0]).arg(range[1]));
        _ui->lineEdit_Lower->setValue(range[0]);
        _ui->lineEdit_Upper->setValue(range[1]);
    }

    void ThresholdDialog::InitCreate()
    {
        QSignalBlocker blocker(_ui->comboBox_Type);
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;
        
        QString name = _postDataManager->checkName("threshold-0");
        _ui->lineEdit_Name->setText(name);

        _ui->comboBox_Type->setCurrentIndex(0);

        //设置阈值范围
        double range[2] = { 0,0 };
        Interface::FITKPostFieldType fieldType = Interface::FITKPostFieldType::Post_None;
        QString fieldName = "";
        getCurrentField(fieldType, fieldName);
        parentData->getRange(range, fieldType, fieldName);
        _ui->label_ValueRange->setText(QString("[%1,%2]").arg(range[0]).arg(range[1]));

        //设置阈值
        _ui->lineEdit_Lower->setText(QString::number(range[0]));
        _ui->lineEdit_Upper->setText(QString::number(range[1]));
    }

    void ThresholdDialog::InitEdit()
    {
        QSignalBlocker blocker(_ui->comboBox_Type);
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;
        Interface::FITKCFDPostThreshold* currentData = dynamic_cast<Interface::FITKCFDPostThreshold*>(_postDataManager->getDataByID(_currentDataID));
        if (currentData == nullptr)return;

        //设置名称
        QString name = currentData->getDataObjectName();
        _ui->lineEdit_Name->setText(name);
        _ui->lineEdit_Name->setEnabled(false);

        setDataToWidget(currentData);
    }

    bool ThresholdDialog::OKCreate()
    {
        QString name = _ui->lineEdit_Name->text();
        if (_postDataManager->getDataByName(name)) {
            QMessageBox box(QMessageBox::Warning, "", QString(tr("\"%1\" Name exists!")).arg(name), QMessageBox::Ok);
            box.exec();
            return false;
        }

        Interface::FITKCFDPostThreshold* streamLine = new Interface::FITKCFDPostThreshold(_parentDataID);
        streamLine->setDataObjectName(name);
        getDataFromWidget(streamLine);
        _currentDataID = streamLine->getDataObjectID();
        _postDataManager->appendDataObj(streamLine);
        return true;
    }

    bool ThresholdDialog::OKEdit()
    {
        Interface::FITKCFDPostThreshold* streamLine = dynamic_cast<Interface::FITKCFDPostThreshold*>(_postDataManager->getDataByID(_currentDataID));
        if (streamLine == nullptr)return false;
        getDataFromWidget(streamLine);
        return true;
    }

    void ThresholdDialog::getDataFromWidget(Interface::FITKCFDPostThreshold * data)
    {
        if (data == nullptr)return;
        Interface::FITKPostFieldType fieldType = Interface::FITKPostFieldType::Post_None;
        QString fieldName = "";
        getCurrentField(fieldType, fieldName);
        data->setValue(_ui->lineEdit_Lower->value(), _ui->lineEdit_Upper->value(), fieldType, fieldName);
    }

    void ThresholdDialog::setDataToWidget(Interface::FITKCFDPostThreshold* data)
    {
        if (data == nullptr)return;
        Interface::FITKPostFieldType fieldType = Interface::FITKPostFieldType::Post_None;
        QString fieldName = "";
        double range[2] = { 0,0 };
        data->getValue(range[0], range[1], fieldType, fieldName);
        _ui->lineEdit_Lower->setText(QString::number(range[0]));
        _ui->lineEdit_Upper->setText(QString::number(range[1]));
        setCurrentField(fieldType, fieldName);

        //设置阈值范围
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(data->getCFDPostParentID());
        if (parentData == nullptr)return;
        parentData->getRange(range, fieldType, fieldName);
        _ui->label_ValueRange->setText(QString("[%1,%2]").arg(range[0]).arg(range[1]));
    }

    void ThresholdDialog::getCurrentField(Interface::FITKPostFieldType & fieldType, QString & fieldName)
    {
        fieldType = _ui->comboBox_Type->currentData().value<Interface::FITKPostFieldType>();
        fieldName = _ui->comboBox_Type->currentText();
    }

    void ThresholdDialog::setCurrentField(Interface::FITKPostFieldType fieldType, QString fieldName)
    {
        //设置场量
        int vectorIndex = -1;
        for (int i = 0; i < _ui->comboBox_Type->count(); i++) {
            QString currentVector = _ui->comboBox_Type->itemText(i);
            Interface::FITKPostFieldType currentType = _ui->comboBox_Type->itemData(i).value<Interface::FITKPostFieldType>();
            if (currentVector == fieldName && currentType == fieldType) {
                vectorIndex = i;
                break;
            }
        }
        _ui->comboBox_Type->setCurrentIndex(vectorIndex);
    }
}
