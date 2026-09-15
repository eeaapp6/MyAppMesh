/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUITimeHistoryRBODYDialog.h"
#include "ui_GUITimeHistoryRBODYDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractTimeHistory.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryRBODY.h"

#include <QMessageBox>
#include <QCheckBox>

namespace GUI
{
    GUITimeHistoryRBODYDialog::GUITimeHistoryRBODYDialog(Core::FITKActionOperator* oper, QWidget* parent)
        : Core::FITKDialog(parent), _oper(oper)
        , _ui(new Ui::GUITimeHistoryRBODYDialog)
    {
        _ui->setupUi(this);

        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKRadiossSolution* solution = caseObj->getCurrentSolution();
        if (!solution)return;
        _mgr = solution->getTimeHietoryManager();
        if (!_mgr)return;
        init();
    }

    GUITimeHistoryRBODYDialog::~GUITimeHistoryRBODYDialog()
    {
        if (_ui)
            delete _ui;
    }

    void GUITimeHistoryRBODYDialog::init()
    {
        setWindowTitle(tr("TimeHistory-RBODY"));
        _ui->tabWidget->setTabText(0, tr("Time History Rbody"));
        _ui->tabWidget->setTabText(1, tr("Probe Data"));
        // 初始化变量组下拉框
        _ui->comboBox_Type->addItem(tr("Customize"), Radioss::FITKTimeHistoryRBODY::VGRBODY_Customize);
        _ui->comboBox_Type->addItem(tr("DEF"), Radioss::FITKTimeHistoryRBODY::VGRBODY_DEF);
        _ui->comboBox_Type->addItem(tr("F"), Radioss::FITKTimeHistoryRBODY::VGRBODY_F);
        _ui->comboBox_Type->addItem(tr("M"), Radioss::FITKTimeHistoryRBODY::VGRBODY_M);
        _ui->comboBox_Type->addItem(tr("R"), Radioss::FITKTimeHistoryRBODY::VGRBODY_R);
        _ui->comboBox_Type->addItem(tr("FI"), Radioss::FITKTimeHistoryRBODY::VGRBODY_FI);
        _ui->comboBox_Type->addItem(tr("MI"), Radioss::FITKTimeHistoryRBODY::VGRBODY_MI);
        // 查询已存在的刚体时间历程数据
        QList<Radioss::FITKAbstractTimeHistory*> timeHistoryList = _mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistory_RBODY);
        if (timeHistoryList.isEmpty())
        {
            _isCreate = true;
            _ui->lineEdit_Name->setText(_mgr->checkName("TimeHistory-RBODY"));
            // 默认启用自定义复选框
            QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
            for (QCheckBox* box : checkBoxes)
                box->setEnabled(true);
        }
        else
        {
            _isCreate = false;
            _data = dynamic_cast<Radioss::FITKTimeHistoryRBODY*>(timeHistoryList.first());
            if (!_data) return;
            // 加载名称
            _ui->lineEdit_Name->setText(_data->getDataObjectName());
            // 加载变量组类型
            auto varGroup = _data->getVarlableGroupType();
            switch (varGroup)
            {
            case Radioss::FITKTimeHistoryRBODY::VGRBODY_Customize:
                _ui->comboBox_Type->setCurrentIndex(0);
                loadCustomVariables(); // 加载自定义变量选中状态
                break;
            case Radioss::FITKTimeHistoryRBODY::VGRBODY_DEF:
                _ui->comboBox_Type->setCurrentIndex(1);
                break;
            case Radioss::FITKTimeHistoryRBODY::VGRBODY_F:
                _ui->comboBox_Type->setCurrentIndex(2);
                break;
            case Radioss::FITKTimeHistoryRBODY::VGRBODY_M:
                _ui->comboBox_Type->setCurrentIndex(3);
                break;
            case Radioss::FITKTimeHistoryRBODY::VGRBODY_R:
                _ui->comboBox_Type->setCurrentIndex(4);
                break;
            case Radioss::FITKTimeHistoryRBODY::VGRBODY_FI:
                _ui->comboBox_Type->setCurrentIndex(5);
                break;
            case Radioss::FITKTimeHistoryRBODY::VGRBODY_MI:
                _ui->comboBox_Type->setCurrentIndex(6);
                break;
            default:
                break;
            }
        }
        initTableWidget();
    }

    void GUITimeHistoryRBODYDialog::initTableWidget()
    {
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (!caseObj) return;
        Radioss::FITKRadiossSolution* solution = caseObj->getCurrentSolution();
        if (!solution) return;
        Radioss::FITKConnectionManager* connectionManager = solution->getConnectionManager();
        if (!connectionManager) return;
        int count = connectionManager->getDataCount();
        // 先清空表格所有内容
        _ui->tableWidget->clear();
        _ui->tableWidget->setRowCount(0);
        // 设置表格列
        _ui->tableWidget->setColumnCount(3);
        _ui->tableWidget->setHorizontalHeaderLabels({ tr("ID"), tr("Name"), tr("IsEnable") });
        // 加载探头ID
        QList<int> selectedProbeIds;
        if (!_isCreate && _data)
        {
            selectedProbeIds = _data->getObjIDs();
        }
        int row = 0;
        // 填充数据到表格
        for (int i = 0; i < count; ++i)
        {
            Radioss::FITKAbstractConnection* connection = connectionManager->getDataByIndex(i);
            if (!connection || connection->getType()!= Radioss::FITKAbstractConnection::ConnectionType::Connection_RBODY) continue;
            // 插入新行
            _ui->tableWidget->insertRow(row);
            // ID列
            QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(connection->getDataObjectID()));
            idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
            _ui->tableWidget->setItem(i, 0, idItem);
            // 名称列
            QTableWidgetItem* nameItem = new QTableWidgetItem(connection->getDataObjectName());
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            _ui->tableWidget->setItem(i, 1, nameItem);
            // 状态列
            QTableWidgetItem* checkItem = new QTableWidgetItem();
            checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            checkItem->setCheckState(selectedProbeIds.contains(connection->getDataObjectID())
                ? Qt::Checked : Qt::Unchecked);
            _ui->tableWidget->setItem(i, 2, checkItem);
            row++;
        }
        _ui->tableWidget->resizeColumnsToContents();
    }

    void GUITimeHistoryRBODYDialog::loadCustomVariables()
    {
        if (!_data) return;
        QList<Radioss::FITKTimeHistoryRBODY::TimeHistoryRBODYVariable> customVars = _data->getCustomizeVariable();
        // 清空所有复选框选中状态
        QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
        for (QCheckBox* box : checkBoxes)
            box->setChecked(false);
        // 根据自定义变量列表设置选中状态
        for (auto var : customVars)
        {
            switch (var)
            {
            case Radioss::FITKTimeHistoryRBODY::FX: _ui->checkBox_FX->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::FY: _ui->checkBox_FY->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::FZ: _ui->checkBox_FZ->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::MX: _ui->checkBox_MX->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::MY: _ui->checkBox_MY->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::MZ: _ui->checkBox_MZ->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::RX: _ui->checkBox_RX->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::RY: _ui->checkBox_RY->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::RZ: _ui->checkBox_RZ->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::FXI: _ui->checkBox_FXI->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::FYI: _ui->checkBox_FYI->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::FZI: _ui->checkBox_FZI->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::MXI: _ui->checkBox_MXI->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::MYI: _ui->checkBox_MYI->setChecked(true); break;
            case Radioss::FITKTimeHistoryRBODY::MZI: _ui->checkBox_MZI->setChecked(true); break;
            default: break;
            }
        }
    }

    QList<Radioss::FITKTimeHistoryRBODY::TimeHistoryRBODYVariable> GUITimeHistoryRBODYDialog::collectCustomVariables()
    {
        QList<Radioss::FITKTimeHistoryRBODY::TimeHistoryRBODYVariable> customVars;
        if (_ui->checkBox_FX->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::FX);
        if (_ui->checkBox_FY->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::FY);
        if (_ui->checkBox_FZ->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::FZ);
        if (_ui->checkBox_MX->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::MX);
        if (_ui->checkBox_MY->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::MY);
        if (_ui->checkBox_MZ->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::MZ);
        if (_ui->checkBox_RX->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::RX);
        if (_ui->checkBox_RY->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::RY);
        if (_ui->checkBox_RZ->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::RZ);
        if (_ui->checkBox_FXI->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::FXI);
        if (_ui->checkBox_FYI->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::FYI);
        if (_ui->checkBox_FZI->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::FZI);
        if (_ui->checkBox_MXI->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::MXI);
        if (_ui->checkBox_MYI->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::MYI);
        if (_ui->checkBox_MZI->isChecked()) customVars.append(Radioss::FITKTimeHistoryRBODY::MZI);
        return customVars;
    }

    QList<int> GUITimeHistoryRBODYDialog::collectSelectedProbeIds()
    {
        QList<int> selectedIds{};
        int rowCount = _ui->tableWidget->rowCount();
        for (int i = 0; i < rowCount; ++i)
        {
            QTableWidgetItem* checkItem = _ui->tableWidget->item(i, 2);
            if (checkItem && checkItem->checkState() == Qt::Checked)
            {
                int probeId = _ui->tableWidget->item(i, 0)->text().toInt();
                selectedIds.append(probeId);
            }
        }
        return selectedIds;
    }

    void GUITimeHistoryRBODYDialog::on_pushButton_OK_clicked()
    {
        if (!_mgr) return;
        QString name = _ui->lineEdit_Name->text();
        //名称非空且创建模式下名称未被占用
        if (name.isEmpty())
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please enter a valid name!"), QMessageBox::Ok);
            return;
        }
        if (_isCreate && _mgr->getDataByName(name))
        {
            QMessageBox::warning(this, "", tr("\"%1\" already exists and cannot be overwritten.").arg(name), QMessageBox::Ok);
            return;
        }
        // 收集并校验ID（必须至少选中一个）
        QList<int> selectedProbeIds = collectSelectedProbeIds();
        if (selectedProbeIds.isEmpty())
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please select at least one probe!"), QMessageBox::Ok);
            return;
        }
        // 收集并校验自定义变量（仅自定义模式需校验）
        Radioss::FITKTimeHistoryRBODY::THRBODYVariableGroup varGroup = static_cast<Radioss::FITKTimeHistoryRBODY::THRBODYVariableGroup>(
            _ui->comboBox_Type->currentData().toInt());
        QList<Radioss::FITKTimeHistoryRBODY::TimeHistoryRBODYVariable> customVars;
        if (varGroup == Radioss::FITKTimeHistoryRBODY::VGRBODY_Customize)
        {
            customVars = collectCustomVariables();
            if (customVars.isEmpty())
            {
                QMessageBox::warning(this, tr("Warning"), tr("Please select at least one variable in Customize mode!"), QMessageBox::Ok);
                return;
            }
        }
        // 所有校验通过后，再处理创建/更新逻辑
        if (_isCreate)
        {
            // 创建新对象并添加到管理器
            _data = new Radioss::FITKTimeHistoryRBODY();
            _mgr->appendDataObj(_data);
        }
        // 设置数据对象的所有属性
        _data->setDataObjectName(name);
        _data->setVarlableGroupType(varGroup);
        if (varGroup == Radioss::FITKTimeHistoryRBODY::VGRBODY_Customize)
        {
            _data->setCustomizeVariable(customVars);
        }
        _data->setObjIDs(selectedProbeIds);
        accept();
    }

    void GUITimeHistoryRBODYDialog::on_pushButton_Cancle_clicked()
    {
        reject();
    }

    void GUITimeHistoryRBODYDialog::on_comboBox_Type_currentIndexChanged(int index)
    {
        QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
        // 重置所有复选框
        for (QCheckBox* box : checkBoxes)
        {
            box->setChecked(false);
            box->setEnabled(false);
        }
        switch (index)
        {
        case 0: // 自定义模式
            for (QCheckBox* box : checkBoxes)
                box->setEnabled(true);
            // 编辑模式时重新加载自定义变量,否则默认勾选第一个变量
            if (!_isCreate && _data && _data->getVarlableGroupType() == Radioss::FITKTimeHistoryRBODY::VGRBODY_Customize)
                loadCustomVariables();
            else
                _ui->checkBox_FX->setChecked(true);
            break;
        case 1: // DEF模式（FX,FY,FZ,MX,MY,MZ,RX,RY,RZ）
            _ui->checkBox_FX->setChecked(true);
            _ui->checkBox_FY->setChecked(true);
            _ui->checkBox_FZ->setChecked(true);
            _ui->checkBox_MX->setChecked(true);
            _ui->checkBox_MY->setChecked(true);
            _ui->checkBox_MZ->setChecked(true);
            _ui->checkBox_RX->setChecked(true);
            _ui->checkBox_RY->setChecked(true);
            _ui->checkBox_RZ->setChecked(true);
            break;
        case 2: // F模式（FX,FY,FZ）
            _ui->checkBox_FX->setChecked(true);
            _ui->checkBox_FY->setChecked(true);
            _ui->checkBox_FZ->setChecked(true);
            break;
        case 3: // M模式（MX,MY,MZ）
            _ui->checkBox_MX->setChecked(true);
            _ui->checkBox_MY->setChecked(true);
            _ui->checkBox_MZ->setChecked(true);
            break;
        case 4: // R模式（RX,RY,RZ）
            _ui->checkBox_RX->setChecked(true);
            _ui->checkBox_RY->setChecked(true);
            _ui->checkBox_RZ->setChecked(true);
            break;
        case 5: // FI模式（FXI,FYI,FZI）
            _ui->checkBox_FXI->setChecked(true);
            _ui->checkBox_FYI->setChecked(true);
            _ui->checkBox_FZI->setChecked(true);
            break;
        case 6: // MI模式（MXI,MYI,MZI）
            _ui->checkBox_MXI->setChecked(true);
            _ui->checkBox_MYI->setChecked(true);
            _ui->checkBox_MZI->setChecked(true);
            break;
        default:
            break;
        }
    }
} // namespace GUI