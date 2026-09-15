/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ProbeDialog.h"
#include "ui_ProbeDialog.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "GUIWidget/GUIPickInfo.h"
#include "GUIWidget/PickedDataProvider.h"
#include "GUIWidget/ProbeCurveWidget.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostProbe.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include <QMessageBox>
#include <QDebug>

#define QtreeItemPointPostType Qt::UserRole
#define QtreeItemPointPostX Qt::UserRole+1
#define QtreeItemPointPostY Qt::UserRole+2
#define QtreeItemPointPostZ Qt::UserRole+3
#define QtreeItemPointPostID Qt::UserRole+4

namespace GUI
{
    ProbeDialog::ProbeDialog(int dataID, EventOper::ParaWidgetInterfaceOperator * oper, bool isCreate) :
        GUI::GUIDialogBase(oper,"ProbeDialog",FITKAPP->getGlobalData()->getMainWindow()), _isCreate(isCreate)
    {
        _ui = new Ui::ProbeDialog();
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

    ProbeDialog::~ProbeDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void ProbeDialog::init()
    {
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

        _ui->comboBox_Type->addItem(tr("Point"));
        _ui->comboBox_Type->addItem(tr("Cell"));
        _ui->comboBox_Type->setCurrentIndex(0);
        _ui->groupBox_Cell->hide();

        _ui->pushButton_PointSelectCancel->hide();
        _ui->pushButton_PointSelectCancel->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
        _ui->pushButton_CellSelectCancel->hide();
        _ui->pushButton_CellSelectCancel->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));

        if (_isCreate) {
            InitCreate();
            setWindowTitle(tr("Probe Create"));
        }
        else {
            InitEdit();
            setWindowTitle(tr("Probe Edit"));
        }
    }

    void ProbeDialog::insertPoint(double* point, int pointID)
    {
        _ui->pushButton_PointSelect->setEnabled(true);
        _ui->pushButton_PointSelectCancel->hide();
        tableAddPoint(point, pointID);
    }

    void ProbeDialog::insertCell(int cellID)
    {
        _ui->pushButton_CellSelect->setEnabled(true);
        _ui->pushButton_CellSelectCancel->hide();
        tableAddCell(cellID);
    }

    void ProbeDialog::closeEvent(QCloseEvent * event)
    {
        //清除拾取
        if (_oper) {
            _oper->eventProcess(2);
            _oper->eventProcess(3);
        }
        GUIDialogBase::closeEvent(event);
    }

    void ProbeDialog::on_pushButton_OK_clicked()
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

    void ProbeDialog::on_pushButton_Cancel_clicked()
    {
        this->close();
    }

    void GUI::ProbeDialog::on_pushButton_PointAdd_clicked()
    {
        double point[3] = { 0,0,0 };
        point[0] = _ui->lineEdit_PointX->value();
        point[1] = _ui->lineEdit_PointY->value();
        point[2] = _ui->lineEdit_PointZ->value();

        tableAddPoint(point, -1);
    }

    void GUI::ProbeDialog::on_pushButton_PointSelect_clicked()
    {
        if (_oper) {
            _ui->pushButton_PointSelectCancel->show();
            _ui->pushButton_PointSelect->setEnabled(false);
            _oper->eventProcess(0);
        }
    }

    void ProbeDialog::on_pushButton_PointSelectCancel_clicked()
    {
        if (_oper) {
            _ui->pushButton_PointSelectCancel->hide();
            _ui->pushButton_PointSelect->setEnabled(true);
            _oper->eventProcess(2);
        }
    }

    void ProbeDialog::on_pushButton_CellSelect_clicked()
    {
        if (_oper) {
            _ui->pushButton_CellSelectCancel->show();
            _ui->pushButton_CellSelect->setEnabled(false);
            _oper->eventProcess(1);
        }
    }

    void ProbeDialog::on_pushButton_CellSelectCancel_clicked()
    {
        if (_oper) {
            _ui->pushButton_CellSelectCancel->hide();
            _ui->pushButton_CellSelect->setEnabled(true);
            _oper->eventProcess(3);
        }
    }

    void ProbeDialog::on_pushButton_Remove_clicked()
    {
        int rowToRemove = _ui->tableWidget->currentRow();
        int rowCount = _ui->tableWidget->rowCount();

        if (rowToRemove < _ui->tableWidget->rowCount()) 
        {
            _ui->tableWidget->removeRow(rowToRemove);
            // 调整表格的行数
            _ui->tableWidget->setRowCount(rowCount - 1);
        }
    }

    void ProbeDialog::on_pushButton_Clear_clicked()
    {
        _ui->tableWidget->setRowCount(0);
    }

    void ProbeDialog::on_comboBox_Type_currentIndexChanged(int index)
    {
        _ui->groupBox_Cell->hide();
        _ui->groupBox_Point->hide();
        _ui->pushButton_PointSelect->setEnabled(true);  
        _ui->pushButton_PointSelectCancel->hide();
        _ui->pushButton_CellSelect->setEnabled(true);
        _ui->pushButton_CellSelectCancel->hide();

        if (_oper) {
            _oper->eventProcess(2);
            _oper->eventProcess(3);
        }

        if (index == 0) {
            _ui->groupBox_Point->show();
        }
        else if (index == 1) {
            _ui->groupBox_Cell->show();
        }
    }

    void ProbeDialog::InitCreate()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;

        //设置名称
        QString name = _postDataManager->checkName("Porbe-0");
        _ui->lineEdit_Name->setText(name);
    }

    void ProbeDialog::InitEdit()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;
        Interface::FITKCFDPostProbe* currentData = dynamic_cast<Interface::FITKCFDPostProbe*>(_postDataManager->getDataByID(_currentDataID));
        if (currentData == nullptr)return;

        QString name = currentData->getDataObjectName();
        _ui->lineEdit_Name->setText(name);
        _ui->lineEdit_Name->setEnabled(false);

        setDataToWidget(currentData);
    }

    bool ProbeDialog::OKCreate()
    {
        QString name = _ui->lineEdit_Name->text();
        if (_postDataManager->getDataByName(name)) {
            QMessageBox::warning(this,"", QString(tr("\"%1\" Name exists!")).arg(name), QMessageBox::Ok);
            return false;
        }

        Interface::FITKCFDPostProbe* data = new Interface::FITKCFDPostProbe(_parentDataID);
        data->setDataObjectName(name);
        getDataFromWidget(data);

        _currentDataID = data->getDataObjectID();
        _postDataManager->appendDataObj(data);

        if (_mainWin->getEventWidget()) {
            data->update();
            ProbeCurveWidget* probeWidget = new ProbeCurveWidget(_currentDataID, FITKAPP->getGlobalData()->getMainWindow());
            _mainWin->getEventWidget()->addTab(probeWidget, name);
            if (_mainWin->getEventWidget()->count() > 0)_mainWin->getEventWidget()->show();
            else _mainWin->getEventWidget()->hide();
        }

        return true;
    }

    bool ProbeDialog::OKEdit()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return false;
        Interface::FITKCFDPostProbe* currentData = dynamic_cast<Interface::FITKCFDPostProbe*>(_postDataManager->getDataByID(_currentDataID));
        if (currentData == nullptr)return false;
        getDataFromWidget(currentData);
        
        return true;
    }

    void ProbeDialog::getDataFromWidget(Interface::FITKAbstractCFDPostData * glyphData)
    {
        Interface::FITKCFDPostProbe* probeData = dynamic_cast<Interface::FITKCFDPostProbe*> (glyphData);
        if (probeData == nullptr)return;

        //数据点清空
        probeData->reset();

        int rowCount = _ui->tableWidget->rowCount();
        for (int i = 0; i < rowCount; i++) {
            QTableWidgetItem* item = _ui->tableWidget->item(i, 0);
            if (item == nullptr)continue;
            double point[3] = { 0,0,0 };
            QString type = item->data(QtreeItemPointPostType).toString();
            if (type == "point") {
                point[0] = item->data(QtreeItemPointPostX).toDouble();
                point[1] = item->data(QtreeItemPointPostY).toDouble();
                point[2] = item->data(QtreeItemPointPostZ).toDouble();
                int pointID = item->data(QtreeItemPointPostID).toInt();
                probeData->appendPoint(point, pointID);
            }
            else if (type == "cell") {
                int cellID = item->data(QtreeItemPointPostID).toInt();
                probeData->appendCell(cellID);
            }
        }
    }

    void ProbeDialog::setDataToWidget(Interface::FITKAbstractCFDPostData * glyphData)
    {
        Interface::FITKCFDPostProbe* probeData = dynamic_cast<Interface::FITKCFDPostProbe*> (glyphData);
        if (probeData == nullptr)return;
        
        int pointCount = probeData->getPointCount();
        for (int i = 0; i < pointCount; i++) {
            double point[3] = { 0,0,0 };
            int pointID = probeData->getPointAtInter(point, i);
            tableAddPoint(point, pointID);
        }

        QList<int> cells = probeData->getCellAll();
        for (int cellID : cells) {
            tableAddCell(cellID);
        }
    }

    void ProbeDialog::tableAddPoint(double* point, int pointID)
    {
        int curRowCount = _ui->tableWidget->rowCount();
        QString pointString = QString("Point:[%1,%2,%3]").arg(point[0]).arg(point[1]).arg(point[2]);
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(pointString);
        item->setData(QtreeItemPointPostType, "point");
        item->setData(QtreeItemPointPostX, point[0]);
        item->setData(QtreeItemPointPostY, point[1]);
        item->setData(QtreeItemPointPostZ, point[2]);
        item->setData(QtreeItemPointPostID, pointID);

        _ui->tableWidget->setRowCount(curRowCount + 1);
        _ui->tableWidget->setItem(curRowCount, 0, item);
    }

    void ProbeDialog::tableAddCell(int cellID)
    {
        int curRowCount = _ui->tableWidget->rowCount();
        QString pointString = QString("Cell:[%1]").arg(QString::number(cellID));
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(pointString);
        item->setData(QtreeItemPointPostType, "cell");
        item->setData(QtreeItemPointPostID, cellID);

        _ui->tableWidget->setRowCount(curRowCount + 1);
        _ui->tableWidget->setItem(curRowCount, 0, item);
    }
}


