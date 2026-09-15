/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUITimeHistoryACCELDialog.h"
#include "ui_GUITimeHistoryACCELDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractTimeHistory.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryACCEL.h"

#include <QMessageBox>
#include <QCheckBox>

namespace GUI
{
    GUITimeHistoryACCELDialog::GUITimeHistoryACCELDialog(Core::FITKActionOperator* oper,QWidget* parent)
        : Core::FITKDialog(parent), _oper(oper)
        , _ui(new Ui::GUITimeHistoryACCELDialog)
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

    GUITimeHistoryACCELDialog::~GUITimeHistoryACCELDialog()
    {
        if (_ui)
            delete _ui;
    }

    void GUITimeHistoryACCELDialog::init()
    {
        setWindowTitle(tr("TimeHistory-ACCEL"));
        _ui->tabWidget->setTabText(0, tr("Time History Accel"));
        _ui->tabWidget->setTabText(1, tr("Probe Data"));
        // 初始化变量组下拉框
        _ui->comboBox_Type->addItem(tr("Customize"), Radioss::FITKTimeHistoryACCEL::VGACCEL_Customize);
        _ui->comboBox_Type->addItem(tr("DEF"), Radioss::FITKTimeHistoryACCEL::VGACCEL_DEF);
        _ui->comboBox_Type->addItem(tr("W"), Radioss::FITKTimeHistoryACCEL::VGACCEL_W);
        // 查询已存在的加速度时间历程数据
        QList<Radioss::FITKAbstractTimeHistory*> timeHistoryList = _mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistory_ACCEL);
        if (timeHistoryList.isEmpty())
        {
            _isCreate = true;
            _ui->lineEdit_Name->setText(_mgr->checkName("TimeHistory-ACCEL"));
            // 默认启用自定义复选框
            QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
            for (QCheckBox* box : checkBoxes)
                box->setEnabled(true);
        }
        else
        {
            _isCreate = false;
            _data = dynamic_cast<Radioss::FITKTimeHistoryACCEL*>(timeHistoryList.first());
            if (!_data) return;
            // 加载名称
            _ui->lineEdit_Name->setText(_data->getDataObjectName());
            // 加载变量组类型
            auto varGroup = _data->getVarlableGroupType();
            switch (varGroup)
            {
            case Radioss::FITKTimeHistoryACCEL::VGACCEL_Customize:
                _ui->comboBox_Type->setCurrentIndex(0);
                loadCustomVariables(); // 加载自定义变量选中状态
                break;
            case Radioss::FITKTimeHistoryACCEL::VGACCEL_DEF:
                _ui->comboBox_Type->setCurrentIndex(1);
                break;
            case Radioss::FITKTimeHistoryACCEL::VGACCEL_W:
                _ui->comboBox_Type->setCurrentIndex(2);
                break;
            default:
                break;
            }
        }
        initTableWidget();
    }

    void GUITimeHistoryACCELDialog::initTableWidget()
    {
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (!caseObj) return;
        Radioss::FITKRadiossSolution* solution = caseObj->getCurrentSolution();
        if (!solution) return;
        Radioss::FITKProbeManager* probeManager = solution->getProbeManager();
        if (!probeManager) return;
        int probeCount = probeManager->getDataCount();
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
        // 填充探头数据到表格
        for (int i = 0; i < probeCount; ++i)
        {
            Radioss::FITKAbstractProbe* probe = probeManager->getDataByIndex(i);
            if (!probe || probe->getType() != Radioss::FITKAbstractProbe::ProbeType::Probe_Acceleration) continue;
            // 插入新行
            _ui->tableWidget->insertRow(row);
            // 探头ID列
            QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(probe->getDataObjectID()));
            idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
            _ui->tableWidget->setItem(i, 0, idItem);
            // 探头名称列
            QTableWidgetItem* nameItem = new QTableWidgetItem(probe->getDataObjectName());
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            _ui->tableWidget->setItem(i, 1, nameItem);
            // 选中状态列
            QTableWidgetItem* checkItem = new QTableWidgetItem();
            checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            checkItem->setCheckState(selectedProbeIds.contains(probe->getDataObjectID())
                ? Qt::Checked : Qt::Unchecked);
            _ui->tableWidget->setItem(i, 2, checkItem);
            row++;
        }
        _ui->tableWidget->resizeColumnsToContents();
    }

    void GUITimeHistoryACCELDialog::loadCustomVariables()
    {
        if (!_data) return;
        QList<Radioss::FITKTimeHistoryACCEL::TimeHistoryACCELVariable> customVars = _data->getCustomizeVariable();
        // 清空所有复选框选中状态
        QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
        for (QCheckBox* box : checkBoxes)
            box->setChecked(false);
        // 根据自定义变量列表设置选中状态
        for (auto var : customVars)
        {
            switch (var)
            {
            case Radioss::FITKTimeHistoryACCEL::AX: _ui->checkBox_AX->setChecked(true); break;
            case Radioss::FITKTimeHistoryACCEL::AY: _ui->checkBox_AY->setChecked(true); break;
            case Radioss::FITKTimeHistoryACCEL::AZ: _ui->checkBox_AZ->setChecked(true); break;
            case Radioss::FITKTimeHistoryACCEL::WX: _ui->checkBox_WX->setChecked(true); break;
            case Radioss::FITKTimeHistoryACCEL::WY: _ui->checkBox_WY->setChecked(true); break;
            case Radioss::FITKTimeHistoryACCEL::WZ: _ui->checkBox_WZ->setChecked(true); break;
            default: break;
            }
        }
    }

    QList<Radioss::FITKTimeHistoryACCEL::TimeHistoryACCELVariable> GUITimeHistoryACCELDialog::collectCustomVariables()
    {
        QList<Radioss::FITKTimeHistoryACCEL::TimeHistoryACCELVariable> customVars;
        if (_ui->checkBox_AX->isChecked()) customVars.append(Radioss::FITKTimeHistoryACCEL::AX);
        if (_ui->checkBox_AY->isChecked()) customVars.append(Radioss::FITKTimeHistoryACCEL::AY);
        if (_ui->checkBox_AZ->isChecked()) customVars.append(Radioss::FITKTimeHistoryACCEL::AZ);
        if (_ui->checkBox_WX->isChecked()) customVars.append(Radioss::FITKTimeHistoryACCEL::WX);
        if (_ui->checkBox_WY->isChecked()) customVars.append(Radioss::FITKTimeHistoryACCEL::WY);
        if (_ui->checkBox_WZ->isChecked()) customVars.append(Radioss::FITKTimeHistoryACCEL::WZ);
        return customVars;
    }

    QList<int> GUITimeHistoryACCELDialog::collectSelectedProbeIds()
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

    void GUITimeHistoryACCELDialog::on_pushButton_OK_clicked()
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
        // 收集并校验探头ID（必须至少选中一个）
        QList<int> selectedProbeIds = collectSelectedProbeIds();
        if (selectedProbeIds.isEmpty())
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please select at least one probe!"), QMessageBox::Ok);
            return;
        }
        // 收集并校验自定义变量（仅自定义模式需校验）
        Radioss::FITKTimeHistoryACCEL::THACCELVariableGroup varGroup = static_cast<Radioss::FITKTimeHistoryACCEL::THACCELVariableGroup>(
            _ui->comboBox_Type->currentData().toInt());
        QList<Radioss::FITKTimeHistoryACCEL::TimeHistoryACCELVariable> customVars;
        if (varGroup == Radioss::FITKTimeHistoryACCEL::VGACCEL_Customize)
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
            _data = new Radioss::FITKTimeHistoryACCEL();
            _mgr->appendDataObj(_data);
        }
        // 设置数据对象的所有属性
        _data->setDataObjectName(name);
        _data->setVarlableGroupType(varGroup);
        if (varGroup == Radioss::FITKTimeHistoryACCEL::VGACCEL_Customize)
        {
            _data->setCustomizeVariable(customVars);
        }
        _data->setObjIDs(selectedProbeIds);
        accept();
    }

    void GUITimeHistoryACCELDialog::on_pushButton_Cancle_clicked()
    {
        reject();
    }

    void GUITimeHistoryACCELDialog::on_comboBox_Type_currentIndexChanged(int index)
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
            if (!_isCreate && _data && _data->getVarlableGroupType() == Radioss::FITKTimeHistoryACCEL::VGACCEL_Customize)
                loadCustomVariables();
            else
                _ui->checkBox_AX->setChecked(true);
            break;
        case 1: // DEF模式（AX,AY,AZ）
            _ui->checkBox_AX->setChecked(true);
            _ui->checkBox_AY->setChecked(true);
            _ui->checkBox_AZ->setChecked(true);
            break;
        case 2: // W模式（WX,WY,WZ）
            _ui->checkBox_WX->setChecked(true);
            _ui->checkBox_WY->setChecked(true);
            _ui->checkBox_WZ->setChecked(true);
            break;
        default:
            break;
        }
    }
} // namespace GUI
