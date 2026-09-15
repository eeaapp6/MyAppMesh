/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ColorLibaryDialog.h"
#include "ui_ColorLibaryDialog.h"

#include "GUIWidget/LinearGradientWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetHBox.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostColorLibaryDataManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostColorLibaryData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"

#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#define ColorLibaryDialogDataPost Qt::UserRole

namespace GUI
{
    ColorLibaryDialog::ColorLibaryDialog(EventOper::ParaWidgetInterfaceOperator* oper) :
        GUIDialogBase(oper, "ColorLibaryDialog", FITKAPP->getGlobalData()->getMainWindow())
    {
        _ui = new Ui::ColorLibaryDialog();
        _ui->setupUi(this);
        init();
        this->setWindowTitle(tr("Color Libary"));
    }

    ColorLibaryDialog::~ColorLibaryDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void ColorLibaryDialog::init()
    {
        //设置表头内容
        QStringList header;
        header << tr("Presets");
        //设置列数
        _ui->tableWidget->setColumnCount(1);
        _ui->tableWidget->setHorizontalHeaderLabels(header);
        //设置tableWidget的item不可编辑
        _ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //隐藏行表头
        _ui->tableWidget->verticalHeader()->setVisible(false);
        //设置选择行为时每次选择一行
        _ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        //自适应布局（更具表头数量平均分配大小）
        _ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        update();
    }

    void ColorLibaryDialog::update()
    {
        _ui->tableWidget->setRowCount(0);

        int row = 1;
        for (int i = 0; i < _colorLibManager->getDataCount(); i++) {
            Interface::FITKCFDPostColorLibaryData* colorData = _colorLibManager->getDataByIndex(i);
            if(colorData == nullptr)continue;
            QList<QWidget*> widgetList = {};
            LinearGradientWidget* widget = new LinearGradientWidget(colorData, this);
            widgetList.append(widget);
            QLabel* title = new QLabel(colorData->getDataObjectName());
            widgetList.append(title);
            Core::FITKWidgetHBox* hWidget = new Core::FITKWidgetHBox(widgetList, this);
            _ui->tableWidget->setRowCount(row);
            _ui->tableWidget->setCellWidget(row - 1, 0, hWidget);
            row++;
        }
    }

    void ColorLibaryDialog::on_pushButton_Apply_clicked()
    {
        int currentRow = _ui->tableWidget->currentRow();
        Interface::FITKCFDPostColorLibaryData* colorLibData = _colorLibManager->getDataByIndex(currentRow);
        if (colorLibData == nullptr)return;
        for (int i = 0; i < _postDataManager->getDataCount(); i++) {
            Interface::FITKAbstractCFDPostData* data = _postDataManager->getDataByIndex(i);
            if (data == nullptr)continue;
            data->setColorData(colorLibData);
        }
        reRender();
    }

    void ColorLibaryDialog::on_pushButton_Import_clicked()
    {
        QString filePath = QFileDialog::getOpenFileName(this, tr("Import"), "", tr("JSON (*.json)"));
        if (filePath.isEmpty())return;

        Interface::FITKCFDPostColorLibaryData* colorLibData = new Interface::FITKCFDPostColorLibaryData();
        bool isOk = colorLibData->readColorJson(filePath);
        if (isOk) {
            _colorLibManager->appendDataObj(colorLibData);
            update();
        }
        else {
            delete colorLibData;
            colorLibData = nullptr;
            QMessageBox::warning(this, tr("Warning"), tr("File read failure!"), QMessageBox::Ok);
        }
    }

    void ColorLibaryDialog::on_pushButton_Export_clicked()
    {
        int currentRow = _ui->tableWidget->currentRow();
        Interface::FITKCFDPostColorLibaryData* colorLibData = _colorLibManager->getDataByIndex(currentRow);
        if (colorLibData == nullptr)return;
        QString filePath = QFileDialog::getSaveFileName(this, tr("Export"), "", tr("JSON (*.json)"));
        if (filePath.isEmpty())return;
        bool isOk = colorLibData->writeColorJson(filePath);
        if (isOk == false) {
            QMessageBox::warning(this, tr("Warning"), tr("File export failure!"), QMessageBox::Ok);
        }
    }

    void ColorLibaryDialog::on_pushButton_Close_clicked()
    {
        this->close();
    }
}