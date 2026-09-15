/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ExtremumDialog.h"
#include "ui_ExtremumDialog.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"

Q_DECLARE_METATYPE(Interface::FITKPostFieldType)


namespace GUI
{
    ExtremumDialog::ExtremumDialog(int dataID, EventOper::ParaWidgetInterfaceOperator * oper) :
        GUIDialogBase(oper,"ExtremumDialog", FITKAPP->getGlobalData()->getMainWindow()),
        _currentDataID(dataID)
    {
        _ui = new Ui::ExtremumDialog();
        _ui->setupUi(this);
        init();
        setWindowTitle(tr("Extremum Dialog"));
    }

    ExtremumDialog::~ExtremumDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void ExtremumDialog::init()
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;

        _ui->tableWidget->setColumnCount(1);
        QStringList header;
        header << tr("");
        _ui->tableWidget->setHorizontalHeaderLabels(header);
        _ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        //设置tableWidget的item不可编辑
        _ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //充满表格
        _ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
        //隐藏列表头
        _ui->tableWidget->horizontalHeader()->setVisible(false);

        //设置名称
        QString name = currentData->getDataObjectName();
        _ui->lineEdit_Name->setText(name);
        _ui->lineEdit_Name->setEnabled(false);

        //设置场量
        QStringList pointData = currentData->getPointDataArray();
        for (auto array : pointData) {
           Interface::FITKPostVariableType variableType = currentData->getVariableType(Interface::FITKPostFieldType::Post_Point, array);
           if (variableType != Interface::FITKPostVariableType::Post_Scalar)continue;
            _ui->comboBox_Field->addItem(QIcon("://icons/nodeValue.png"), array, QVariant::fromValue(Interface::FITKPostFieldType::Post_Point));
        }

        QStringList cellData = currentData->getCellDataArray();
        for (auto array : cellData) {
            Interface::FITKPostVariableType variableType = currentData->getVariableType(Interface::FITKPostFieldType::Post_Cell, array);
            if (variableType != Interface::FITKPostVariableType::Post_Scalar)continue;
            _ui->comboBox_Field->addItem(QIcon("://icons/cellValue.png"), array, QVariant::fromValue(Interface::FITKPostFieldType::Post_Cell));
        }

        if (!pointData[0].isEmpty()) {
            updateWidget(pointData[0], Interface::FITKPostFieldType::Post_Point);
        }
        else if (!cellData[0].isEmpty()) {
            updateWidget(cellData[0], Interface::FITKPostFieldType::Post_Cell);
        }
    }

    void ExtremumDialog::updateWidget(QString fieldName, Interface::FITKPostFieldType fieldType)
    {
        _ui->tableWidget->clear();
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        
        _ui->tableWidget->setRowCount(4);
        QStringList header;
        header << tr("Max Value") << tr("Min Value") << tr("Max Point") << tr("Min Point");
        _ui->tableWidget->setVerticalHeaderLabels(header);

        double range[2] = { 0,0 }, point[6] = { 0,0,0,0,0,0 };
        currentData->getExtremum(range, point, fieldType, fieldName);
        QString maxValue = QString::number(range[1]);
        QTableWidgetItem* item = new QTableWidgetItem(maxValue);
        _ui->tableWidget->setItem(0, 0, item);
        
        QString minValue = QString::number(range[0]);
        item = new QTableWidgetItem(minValue);
        _ui->tableWidget->setItem(1, 0, item);
        
        QString maxPoint = QString("(%1 , %2 , %3)").arg(point[1]).arg(point[3]).arg(point[5]);
        item = new QTableWidgetItem(maxPoint);
        _ui->tableWidget->setItem(2, 0, item);

        QString minPoint = QString("(%1 , %2 , %3)").arg(point[0]).arg(point[2]).arg(point[4]);
        item = new QTableWidgetItem(minPoint);
        _ui->tableWidget->setItem(3, 0, item);
    }

    void ExtremumDialog::on_comboBox_Field_currentIndexChanged(int index)
    {
        Q_UNUSED(index);
        QString fieldName = _ui->comboBox_Field->currentText();
        Interface::FITKPostFieldType fieldType = _ui->comboBox_Field->currentData().value<Interface::FITKPostFieldType>();
        updateWidget(fieldName, fieldType);
    }
}