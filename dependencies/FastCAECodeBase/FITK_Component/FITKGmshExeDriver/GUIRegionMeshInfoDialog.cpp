/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIRegionMeshInfoDialog.h"
#include "ui_GUIRegionMeshInfoDialog.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSize.h"
#include <QMessageBox>
namespace GUI {
    GUIRegionMeshInfoDialog::GUIRegionMeshInfoDialog(QWidget* parent)
        :Core::FITKDialog(parent)
    {
        _ui = new Ui::GUIRegionMeshInfoDialog();
        _ui->setupUi(this);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        this->init();
    }

    GUIRegionMeshInfoDialog::~GUIRegionMeshInfoDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void GUIRegionMeshInfoDialog::init()
    { 
        setWindowTitle(tr("Region Info"));
        //获取局部区域尺寸管理器
        Interface::FITKRegionMeshSizeManager* regionMeshSizeManager = Interface::FITKMeshGenInterface::getInstance()->getRegionMeshSizeMgr("GmshExec");
        if (regionMeshSizeManager == nullptr) return;
        int count = regionMeshSizeManager->getDataCount();
        //将表格初始化为一列
        _ui->tableWidget->setColumnCount(1);
        // 设置表格行数
        _ui->tableWidget->setRowCount(count);
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKAbstractRegionMeshSize* regionMeshSize = regionMeshSizeManager->getDataByIndex(i);
            QString dataName = regionMeshSize->getDataObjectName();
            //QString dataName = QString("Region-%1").arg(i);
            // 创建表格项并设置文本
            QTableWidgetItem* item = new QTableWidgetItem(dataName);
            // 将表格项设置到第i行第0列
            _ui->tableWidget->setItem(i, 0, item);
        }

        _ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 列宽自动拉伸
        _ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑

        // 设置单选模式
        _ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  // 整行选择
        _ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // 单选模式
    }

    void GUIRegionMeshInfoDialog::on_pushButton_OK_clicked()
    {
        if (_ui->tableWidget->selectedItems().isEmpty()) 
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please select a region first!"));
            return;
        }
        Interface::FITKRegionMeshSizeManager* regionMeshSizeManager = Interface::FITKMeshGenInterface::getInstance()->getRegionMeshSizeMgr("GmshExec");
        if (regionMeshSizeManager == nullptr) return;
        int selectedRow = _ui->tableWidget->selectedItems().first()->row();
        regionMeshSizeManager->removeDataByIndex(selectedRow);
        this->accept();
    }
    void GUIRegionMeshInfoDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }
}