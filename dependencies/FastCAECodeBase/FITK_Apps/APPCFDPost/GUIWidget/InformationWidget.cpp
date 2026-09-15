/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "InformationWidget.h"
#include "ui_InformationWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"

namespace GUI
{
    InformationWidget::InformationWidget(QWidget * parent) :
        GUIWidgetBase(parent)
    {
        _ui = new Ui::InformationWidget();
        _ui->setupUi(this);
        init();
    }

    InformationWidget::~InformationWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void InformationWidget::init()
    {
        _postDataManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();

        //设置表头内容
        QStringList header;
        header << tr("Name") << tr("Type") << tr("Ranges");
        //设置列数
        _ui->tableWidget_Data->setColumnCount(3);
        _ui->tableWidget_Data->setHorizontalHeaderLabels(header);
        //设置tableWidget的item不可编辑
        _ui->tableWidget_Data->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //隐藏行表头
        _ui->tableWidget_Data->verticalHeader()->setVisible(false);
        //设置选择行为时每次选择一行
        _ui->tableWidget_Data->setSelectionBehavior(QAbstractItemView::SelectRows);
        //自适应布局（更具表头数量平均分配大小）
        _ui->tableWidget_Data->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        //设置第1、2列自适应大小
        _ui->tableWidget_Data->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        _ui->tableWidget_Data->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

        //设置表头内容
        QStringList cellHeader;
        cellHeader << tr("Type") << tr("Num");
        //设置列数
        _ui->tableWidget_Cell->setColumnCount(2);
        _ui->tableWidget_Cell->setHorizontalHeaderLabels(cellHeader);
        //设置tableWidget的item不可编辑
        _ui->tableWidget_Cell->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //隐藏行表头
        _ui->tableWidget_Cell->verticalHeader()->setVisible(false);
        //设置选择行为时每次选择一行
        _ui->tableWidget_Cell->setSelectionBehavior(QAbstractItemView::SelectRows);
        //自适应布局（更具表头数量平均分配大小）
        _ui->tableWidget_Cell->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        //设置第1列自适应大小
        _ui->tableWidget_Cell->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

        updateCurrentGraphObj(-1);
    }

    void InformationWidget::updateCurrentGraphObj(int postDataID)
    {
        _currentDataID = postDataID;

        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(postDataID);
        _ui->labelNameInfo->setText("(n/a)");
        _ui->labelTypeInfo->setText("(n/a)");
        _ui->labelPointInfo->setText("0");
        _ui->labelCellInfo->setText("0");
        if (currentData) {
            QString name = currentData->getDataObjectName();
            _ui->labelNameInfo->setText(name);
            int pointNum = currentData->getPointNumber();
            _ui->labelPointInfo->setText(QString::number(pointNum));
            int cellNum = currentData->getCellNumber();
            _ui->labelCellInfo->setText(QString::number(cellNum));

            Interface::FITKPostGridType gridType = currentData->getGridType();
            QString type = "";
            switch (gridType) {
            case Interface::FITKPostGridType::Post_None:type = tr("None"); break;
            case Interface::FITKPostGridType::Post_Structured:type = tr("Structured Grid"); break;
            case Interface::FITKPostGridType::Post_UnStructured:type = tr("UnStructured Grid"); break;
            }
            _ui->labelTypeInfo->setText(type);
        }

        updateCellTable();
        updateDataTable();
    }

    void InformationWidget::updateCellTable()
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        _ui->tableWidget_Cell->setRowCount(0);
        if (currentData == nullptr)return;
        QHash<int, int> gridCellType = currentData->getGridCellType();
        _ui->tableWidget_Cell->setRowCount(gridCellType.keys().size());

        int currentRow = 0;
        for (int c: gridCellType.keys()) {
            QString cellType = "";
            if (c == 0) cellType = tr("EMPTY_CELL");
            else if (c == 1) cellType = tr("VERTEX");
            else if (c == 2) cellType = tr("POLY_VERTEX");
            else if (c == 3) cellType = tr("LINE");
            else if (c == 4) cellType = tr("POLY_LINE");
            else if (c == 5) cellType = tr("TRIANGLE");
            else if (c == 6) cellType = tr("TRIANGLE_STRIP");
            else if (c == 7) cellType = tr("POLYGON");
            else if (c == 8) cellType = tr("PIXEL");
            else if (c == 9) cellType = tr("QUAD");
            else if (c == 10) cellType = tr("TETRA");
            else if (c == 11) cellType = tr("VOXEL");
            else if (c == 12) cellType = tr("HEXAHEDRON");
            else if (c == 13) cellType = tr("WEDGE");
            else if (c == 14) cellType = tr("PYRAMID");
            else if (c == 15) cellType = tr("PENTAGONAL_PRISM");
            else if (c == 16) cellType = tr("HEXAGONAL_PRISM");
            QTableWidgetItem* item = new QTableWidgetItem(cellType);
            _ui->tableWidget_Cell->setItem(currentRow, 0, item);

            QString cellNum = QString::number(gridCellType.value(c));
            item = new QTableWidgetItem(cellNum);
            _ui->tableWidget_Cell->setItem(currentRow, 1, item);
            currentRow++;
        }
    }

    void InformationWidget::updateDataTable()
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        _ui->tableWidget_Data->setRowCount(0);
        if (currentData == nullptr)return;
        QStringList pointList = currentData->getPointDataArray();
        QStringList cellList = currentData->getCellDataArray();
        _ui->tableWidget_Data->setRowCount(pointList.size() + cellList.size());

        int currentRow = 0;
        for (QString point : pointList) {
            //名称添加
            QTableWidgetItem* item = new QTableWidgetItem(point);
            item->setIcon(QIcon("://icons/nodeValue.png"));
            _ui->tableWidget_Data->setItem(currentRow, 0, item);

            //类型添加
            QString type = "";
            Interface::FITKPostVariableType varType = currentData->getVariableType(Interface::FITKPostFieldType::Post_Point, point);
            switch (varType){
            case Interface::FITKPostVariableType::Post_Error:type = tr("Error"); break;
            case Interface::FITKPostVariableType::Post_Scalar:type = tr("Scalar"); break;
            case Interface::FITKPostVariableType::Post_Vector:type = tr("Vector"); break;
            case Interface::FITKPostVariableType::Post_Tensor:type = tr("Tensor"); break;
            }
            item = new QTableWidgetItem(type);
            _ui->tableWidget_Data->setItem(currentRow, 1, item);

            //范围添加
            double rangeD[2] = { 0,0 };
            currentData->getRange(rangeD, Interface::FITKPostFieldType::Post_Point, point);
            QString rangeS = QString("[%1,%2]").arg(rangeD[0]).arg(rangeD[1]);
            item = new QTableWidgetItem(rangeS);
            _ui->tableWidget_Data->setItem(currentRow, 2, item);

            currentRow++;
        }

        for (QString cell : cellList) {
            //名称添加
            QTableWidgetItem* item = new QTableWidgetItem(cell);
            item->setIcon(QIcon("://icons/cellValue.png"));
            _ui->tableWidget_Data->setItem(currentRow, 0, item);
            //类型添加
            QString type = "";
            Interface::FITKPostVariableType varType = currentData->getVariableType(Interface::FITKPostFieldType::Post_Cell, cell);
            switch (varType) {
            case Interface::FITKPostVariableType::Post_Error:type = tr("Error"); break;
            case Interface::FITKPostVariableType::Post_Scalar:type = tr("Scalar"); break;
            case Interface::FITKPostVariableType::Post_Vector:type = tr("Vector"); break;
            case Interface::FITKPostVariableType::Post_Tensor:type = tr("Tensor"); break;
            }
            item = new QTableWidgetItem(type);
            _ui->tableWidget_Data->setItem(currentRow, 1, item);

            //范围添加
            double rangeD[2] = { 0,0 };
            currentData->getRange(rangeD, Interface::FITKPostFieldType::Post_Cell, cell);
            QString rangeS = QString("[%1,%2]").arg(rangeD[0]).arg(rangeD[1]);
            item = new QTableWidgetItem(rangeS);
            _ui->tableWidget_Data->setItem(currentRow, 2, item);

            currentRow++;
        }
    }
}