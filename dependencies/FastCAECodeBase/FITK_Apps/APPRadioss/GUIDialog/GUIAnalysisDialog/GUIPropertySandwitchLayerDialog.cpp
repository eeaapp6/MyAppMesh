/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPropertySandwitchLayerDialog.h"
#include "ui_GUIPropertySandwitchLayerDialog.h"
#include "FITK_Component/FITKRadiossData/FITKPropSandwichShell.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include <QComboBox>

namespace GUI
{
    GUIPropertySandwitchLayerDialog::GUIPropertySandwitchLayerDialog(Radioss::FITKPropSandwichShell* obj, QWidget* parent) 
        :QDialog(parent) , _ui(new Ui::GUIPropertySandwitchLayerDialog), _obj(obj)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸
        init();
    }
    GUIPropertySandwitchLayerDialog::~GUIPropertySandwitchLayerDialog()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
    }
    void GUIPropertySandwitchLayerDialog::init()
    {
        if (!_obj)return;
        Radioss::FITKPropSandwichShellValue* value = _obj->getValue();
        if (!value)return;

        for (int i = 0; i < value->getN();++i) {
            _ui->tableWidget->insertRow(i);
            Radioss::SandwichShellLayerValue layValue = value->getLayerValue(i);
            _ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(layValue._angle)));
            _ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(layValue._thickness)));
            _ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(layValue._positionZ)));
            _ui->tableWidget->setCellWidget(i, 3, createMatBox(layValue._matID));
            _ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(layValue._relFWeight)));
        }
        //调整列宽
        _ui->tableWidget->resizeColumnsToContents();
        for (int i = 0; i < _ui->tableWidget->columnCount(); ++i)
        {
            int currentWidth = _ui->tableWidget->columnWidth(i);
            if (currentWidth < 60)
                _ui->tableWidget->setColumnWidth(i, 60);
            else if (currentWidth > 100)
                _ui->tableWidget->setColumnWidth(i, 100);
        }

    }

    QComboBox * GUIPropertySandwitchLayerDialog::createMatBox(int matID)
    {
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return nullptr;
        //获取材料管理器
        Interface::FITKMaterialManager* matMgr = caseObj->getMaterialManager();
        if (!matMgr)return nullptr;
        QComboBox* comBobox = new QComboBox(_ui->tableWidget);
        for (int i = 0; i < matMgr->getDataCount(); ++i)
        {
            Interface::FITKAbstractMaterial* mat = matMgr->getDataByIndex(i);
            if (!mat)continue;
            comBobox->addItem(mat->getDataObjectName(), mat->getDataObjectID());
        }
        if (matID > 0) {
            int index = comBobox->findData(matID);
            comBobox->setCurrentIndex(index);
        }
        return comBobox;
    }

    void GUI::GUIPropertySandwitchLayerDialog::on_pushButtonOK_clicked()
    {
        if (!_obj)return;
        Radioss::FITKPropSandwichShellValue* value = _obj->getValue();
        if (!value)return;

        int row = _ui->tableWidget->rowCount();
        value->setN(row);
        for (int i = 0; i < row; ++i) 
        {
            Radioss::SandwichShellLayerValue layValue = value->getLayerValue(i);
            layValue._angle = _ui->tableWidget->item(i, 0)->text().toDouble();
            layValue._thickness = _ui->tableWidget->item(i, 1)->text().toDouble();
            layValue._positionZ = _ui->tableWidget->item(i, 2)->text().toDouble();
            QComboBox* matBox = dynamic_cast<QComboBox*>(_ui->tableWidget->cellWidget(i, 3));
            if (matBox)
                layValue._matID = matBox->currentData().toInt();
            layValue._relFWeight = _ui->tableWidget->item(i, 4)->text().toDouble();
            value->setLayerValue(layValue, i);
        }
        accept();
    }

    void GUI::GUIPropertySandwitchLayerDialog::on_pushButtonCancel_clicked()
    {
        reject();
    }

    void GUIPropertySandwitchLayerDialog::on_pushButtonAdd_clicked()
    {
        int row = _ui->tableWidget->rowCount();
        _ui->tableWidget->insertRow(row);
        _ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(0.0), row));
        _ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(0.0), row));
        _ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(0.0), row));
        _ui->tableWidget->setCellWidget(row, 3, createMatBox(-1));
        _ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(0.0), row));
    }

    void GUIPropertySandwitchLayerDialog::on_pushButtonDelete_clicked()
    {
        int currentRow = _ui->tableWidget->currentRow();
        if (currentRow == -1) return;
        _ui->tableWidget->removeRow(currentRow);

        // 重新选择删除行的下行
        int rowCount = _ui->tableWidget->rowCount();
        if (rowCount > 0) {
            int newRow = qMin(currentRow, rowCount - 1); 
            _ui->tableWidget->selectRow(newRow);
        }
    }
}