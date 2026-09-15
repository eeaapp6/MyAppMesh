/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ColorLibaryEditDialog.h"
#include "ui_ColorLibaryEditDialog.h"
#include "GUIWidget/LinearGradientWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostColorLibaryData.h"

namespace GUI
{
    ColorLibaryEditDialog::ColorLibaryEditDialog(int currentDataID, EventOper::ParaWidgetInterfaceOperator* oper) :
        GUIDialogBase(oper, "ColorLibaryEditDialog", FITKAPP->getGlobalData()->getMainWindow())
    {
        _currentDataID = currentDataID;
        _ui = new Ui::ColorLibaryEditDialog();
        _ui->setupUi(this);
        init();

        this->setWindowTitle(tr("Color Edit"));
    }

    ColorLibaryEditDialog::~ColorLibaryEditDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
        if (_linearWidget) {
            delete _linearWidget;
            _linearWidget = nullptr;
        }
        if (_copyColorLibData) {
            delete _copyColorLibData;
            _copyColorLibData = nullptr;
        }
    }

    void ColorLibaryEditDialog::init()
    {
        Interface::FITKAbstractCFDPostData* postData = _postDataManager->getDataByID(_currentDataID);
        if (postData == nullptr)return;
        _copyColorLibData = new Interface::FITKCFDPostColorLibaryData();
        _copyColorLibData->copy(postData->getColorData());
        _linearWidget = new LinearGradientWidget(_copyColorLibData, this);
        _linearWidget->setMinimumSize(QSize(200, 50));
        _ui->gridLayout_LinearGradient->addWidget(_linearWidget);

        double range[2] = { 0,1 };
        _ui->lineEdit_ColorPos->setRange(range);
        _ui->lineEdit_ColorR->setRange(range);
        _ui->lineEdit_ColorG->setRange(range);
        _ui->lineEdit_ColorB->setRange(range);

        //设置表头内容
        QStringList cellHeader;
        cellHeader << tr("Pos") << tr("R") << tr("G") << tr("B");
        //设置列数
        _ui->tableWidget->setColumnCount(4);
        _ui->tableWidget->setHorizontalHeaderLabels(cellHeader);
        //设置tableWidget的item不可编辑
        _ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //隐藏行表头
        _ui->tableWidget->verticalHeader()->setVisible(false);
        //设置选择行为时每次选择一行
        _ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        //自适应布局（更具表头数量平均分配大小）
        _ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        //设置第1列自适应大小
        _ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

        updateTableWidget();
    }

    void ColorLibaryEditDialog::on_pushButton_OK_clicked()
    {
        Interface::FITKAbstractCFDPostData* postData = _postDataManager->getDataByID(_currentDataID);
        if (postData == nullptr || _copyColorLibData == nullptr)return;
        Interface::FITKCFDPostColorLibaryData* posColorLibData = postData->getColorData();
        if (posColorLibData == nullptr)return;
        posColorLibData->copy(_copyColorLibData);
        postData->updateColor();
        this->accept();
    }

    void ColorLibaryEditDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void ColorLibaryEditDialog::on_pushButton_Append_clicked()
    {
        if (_copyColorLibData == nullptr)return;
        double pos = _ui->lineEdit_ColorPos->value();
        Interface::FITKPostColorStr str;
        str._R = _ui->lineEdit_ColorR->value();
        str._G = _ui->lineEdit_ColorG->value();
        str._B = _ui->lineEdit_ColorB->value();
        _copyColorLibData->insertColorPoints(pos, str);
        updateTableWidget();
    }

    void ColorLibaryEditDialog::on_pushButton_Remove_clicked()
    {
        if (_copyColorLibData == nullptr)return;
        int currentIndex = _ui->tableWidget->currentRow();
        QMap<double, Interface::FITKPostColorStr> colorPoints = _copyColorLibData->getColorPoints();
        double pos = colorPoints.keys()[currentIndex];
        colorPoints.remove(pos);
        _copyColorLibData->setColorPoints(colorPoints);
        updateTableWidget();
    }

    void ColorLibaryEditDialog::on_pushButton_Clear_clicked()
    {
        if (_copyColorLibData == nullptr)return;
        _copyColorLibData->clear();
        updateTableWidget();
    }

    void ColorLibaryEditDialog::updateTableWidget()
    {
        _ui->tableWidget->setRowCount(0);
        if (_copyColorLibData == nullptr || _linearWidget == nullptr)return;
        QMap<double, Interface::FITKPostColorStr> colorPoints = _copyColorLibData->getColorPoints();

        for (int i = 0; i < colorPoints.keys().size(); i++) {
            _ui->tableWidget->setRowCount(i + 1);
            
            double pos = colorPoints.keys()[i];
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(QString::number(pos));
            _ui->tableWidget->setItem(i, 0, item);

            double R = colorPoints.value(pos)._R;
            item = new QTableWidgetItem();
            item->setText(QString::number(R));
            _ui->tableWidget->setItem(i, 1, item);

            double G = colorPoints.value(pos)._G;
            item = new QTableWidgetItem();
            item->setText(QString::number(G));
            _ui->tableWidget->setItem(i, 2, item);

            double B = colorPoints.value(pos)._B;
            item = new QTableWidgetItem();
            item->setText(QString::number(B));
            _ui->tableWidget->setItem(i, 3, item);
        }

        _linearWidget->updateWidget();
    }
}

