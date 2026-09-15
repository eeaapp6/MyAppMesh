/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "IsosurfDialog.h"
#include "ui_IsosurfDialog.h"

#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "PostGraphAdaptor/PostGraphObjectBase.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"
#include "PostGraphAdaptor/PostGraphProperty.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostIsosurface.h"

#include <QMessageBox>

namespace GUI
{
    IsosurfDialog::IsosurfDialog(int dataID, EventOper::ParaWidgetInterfaceOperator* oper, bool isCreate) :
        GUIDialogBase(oper,"IsosurfDialog", FITKAPP->getGlobalData()->getMainWindow()),
        _isCreate(isCreate)
    {
        _ui = new Ui::IsosurfDialog();
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

    IsosurfDialog::~IsosurfDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void IsosurfDialog::init()
    {
        QSignalBlocker blocker(_ui->comboBox_Contour);

        _ui->tableWidget->setColumnCount(1);
        QStringList header;
        header << tr("");
        _ui->tableWidget->setHorizontalHeaderLabels(header);
        _ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        //充满表格
        _ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
        //隐藏行表头
        _ui->tableWidget->verticalHeader()->setVisible(false);
        //隐藏列表头
        _ui->tableWidget->horizontalHeader()->setVisible(false);

        if (_isCreate) {
            createInit();
            setWindowTitle(tr("Isosurf Create"));
        }
        else{
            editInit();
            setWindowTitle(tr("Isosurf Edit"));
        }
    }

    void IsosurfDialog::createInit()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;

        //设置名称
        QString name = _postDataManager->checkName("Isosurface-1");
        _ui->lineEdit_Name->setText(name);

        QStringList pointList = parentData->getPointDataArray();
        QStringList contourList = {};
        //点标量数据
        for (QString point : pointList) {
            Interface::FITKPostVariableType type = parentData->getVariableType(Interface::FITKPostFieldType::Post_Point, point);
            if (type == Interface::FITKPostVariableType::Post_Scalar) contourList.append(point);
        }
        _ui->comboBox_Contour->addItems(contourList);
        if (contourList.isEmpty())return;

        _ui->comboBox_Contour->setCurrentIndex(0);
        double rangeD[2] = { 0,0 };
        parentData->getRange(rangeD, Interface::FITKPostFieldType::Post_Point, _ui->comboBox_Contour->currentText());
        QString rangeS = QString("[%1,%2]").arg(QString::number(rangeD[0]), QString::number(rangeD[1]));
        _ui->label_Range->setText(rangeS);

        //初始化表格
        _ui->tableWidget->setRowCount(1);

        QString text = _ui->comboBox_Contour->currentText();
        double range[2] = { 0,0 };
        parentData->getRange(range, Interface::FITKPostFieldType::Post_Point, text);
        double value = (range[0] + range[1]) / 2;
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(value));
        _ui->tableWidget->setItem(0, 0, item);
    }

    void IsosurfDialog::editInit()
    {
        Interface::FITKCFDPostIsosurface* currentData = dynamic_cast<Interface::FITKCFDPostIsosurface*>(_postDataManager->getDataByID(_currentDataID));
        if (currentData == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;

        //设置名称
        _ui->lineEdit_Name->setText(currentData->getDataObjectName());
        _ui->lineEdit_Name->setEnabled(false);

        QStringList pointList = parentData->getPointDataArray();
        QStringList contourList = {};
        //获取所有的点标量数据
        for (QString point : pointList) {
            Interface::FITKPostVariableType type = parentData->getVariableType(Interface::FITKPostFieldType::Post_Point, point);
            if (type == Interface::FITKPostVariableType::Post_Scalar) contourList.append(point);
        }
        _ui->comboBox_Contour->addItems(contourList);
        if (contourList.isEmpty())return;
        //设置当前标量数据
        Interface::FITKPostFieldType currentType = currentData->getFiledType();
        QString filedName = currentData->getFiledName();
        _ui->comboBox_Contour->setCurrentText(filedName);

        //获取标数据范围
        double rangeD[2] = { 0,0 };
        parentData->getRange(rangeD, Interface::FITKPostFieldType::Post_Point, _ui->comboBox_Contour->currentText());
        QString rangeS = QString("[%1,%2]").arg(QString::number(rangeD[0]), QString::number(rangeD[1]));
        _ui->label_Range->setText(rangeS);

        //初始化表格
        for (double value : currentData->getValue()) {
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(value));
            int rowCount = _ui->tableWidget->rowCount();
            _ui->tableWidget->setRowCount(rowCount + 1);
            _ui->tableWidget->setItem(rowCount, 0, item);
        }
    }

    bool IsosurfDialog::createOK()
    {
        QString name = _ui->lineEdit_Name->text();
        if (_postDataManager->getDataByName(name)) {
            QMessageBox box(QMessageBox::Warning, "", QString(tr("\"%1\" Name exists!")).arg(name), QMessageBox::Ok);
            box.exec();
            return false;
        }

        Interface::FITKCFDPostIsosurface* isosurfaceData = new Interface::FITKCFDPostIsosurface(_parentDataID);
        getDataFromWidget(isosurfaceData);
        _currentDataID = isosurfaceData->getDataObjectID();
        _postDataManager->appendDataObj(isosurfaceData);
        return true;
    }

    bool IsosurfDialog::editOK()
    {
        Interface::FITKCFDPostIsosurface* isosurfaceData = dynamic_cast<Interface::FITKCFDPostIsosurface*>(_postDataManager->getDataByID(_currentDataID));
        if (isosurfaceData == nullptr)return false;
        getDataFromWidget(isosurfaceData);
        return true;
    }

    void IsosurfDialog::getDataFromWidget(Interface::FITKCFDPostIsosurface* isoSurData)
    {
        if (isoSurData == nullptr)return;

        QString name = _ui->lineEdit_Name->text();
        isoSurData->setDataObjectName(name);

        QString filedName = _ui->comboBox_Contour->currentText();
        QList<double> values = {};
        for (int i = 0; i < _ui->tableWidget->rowCount(); i++) {
            QTableWidgetItem*  item = _ui->tableWidget->item(i, 0);
            if (item == nullptr) continue;
            double v = item->text().toDouble();
            values.append(v);
        }

        isoSurData->setFiledName(filedName);
        isoSurData->setFiledType(Interface::FITKPostFieldType::Post_Point);
        isoSurData->setValue(values);
    }

    void IsosurfDialog::setDataToWidget(Interface::FITKCFDPostIsosurface* isoSurData)
    {
        if (isoSurData == nullptr)return;
    }

    void IsosurfDialog::on_pushButton_OK_clicked()
    {
        if (_isCreate) {
            if (!createOK())return;
        }
        else {
            if (!editOK())return;
        }

        if (_oper) {
            _oper->setArgs(objID_String, _currentDataID);
            _oper->execProfession();
        }
        this->accept();
    }

    void IsosurfDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void IsosurfDialog::on_comboBox_Contour_currentIndexChanged(int index)
    {
        Q_UNUSED(index);

        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;
        QString text = _ui->comboBox_Contour->currentText();
        double rangeD[2] = { 0,0 };
        parentData->getRange(rangeD, Interface::FITKPostFieldType::Post_Point, text);

        QString rangeS = QString("[%1,%2]").arg(QString::number(rangeD[0]), QString::number(rangeD[1]));
        _ui->label_Range->setText(rangeS);
        _ui->pushButton_Remove->setEnabled(false);
    }

    void IsosurfDialog::on_pushButton_Add_clicked()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;
        QString text = _ui->comboBox_Contour->currentText();
        double range[2] = { 0,0 };
        parentData->getRange(range, Interface::FITKPostFieldType::Post_Point, text);
        double value = (range[0] + range[1]) / 2;
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(value));
        int rowCount = _ui->tableWidget->rowCount();
        _ui->tableWidget->setRowCount(rowCount + 1);
        _ui->tableWidget->setItem(rowCount, 0, item);
    }

    void IsosurfDialog::on_pushButton_Remove_clicked()
    {
        int currentRow = _ui->tableWidget->currentRow();
        int rowCount = _ui->tableWidget->rowCount();
        QStringList valueList = {};
        for (int i = 0; i < rowCount; i++) {
            QTableWidgetItem* item = _ui->tableWidget->item(i, 0);
            if (item == nullptr)continue;
            valueList.append(item->text());
        }
        _ui->tableWidget->clear();
        //清除数据
        valueList.removeAt(currentRow);

        //重新添加数据
        _ui->tableWidget->setRowCount(valueList.size());
        for (int i = 0; i < valueList.size(); i++) {
            QTableWidgetItem* item = new QTableWidgetItem(valueList[i]);
            _ui->tableWidget->setItem(i, 0, item);
        }
    }

    void IsosurfDialog::on_tableWidget_cellClicked(int row, int column)
    {
        Q_UNUSED(row);
        Q_UNUSED(column);

        _ui->pushButton_Remove->setEnabled(true);
    }
}