/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUITimeHistoryRWALLDialog.h"
#include "ui_GUITimeHistoryRWALLDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractTimeHistory.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryRWALL.h"

#include <QMessageBox>
#include <QCheckBox>

namespace GUI
{
    GUITimeHistoryRWALLDialog::GUITimeHistoryRWALLDialog(Core::FITKActionOperator* oper, QWidget* parent)
        : Core::FITKDialog(parent), _oper(oper)
        , _ui(new Ui::GUITimeHistoryRWALLDialog)
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

    GUITimeHistoryRWALLDialog::~GUITimeHistoryRWALLDialog()
    {
        if (_ui)
            delete _ui;
    }

    void GUITimeHistoryRWALLDialog::init()
    {
        setWindowTitle(tr("TimeHistory-RWALL"));
        _ui->tabWidget->setTabText(0, tr("Time History Rwall"));
        _ui->tabWidget->setTabText(1, tr("Probe Data"));
        // 初始化变量组下拉框
        _ui->comboBox_Type->addItem(tr("Customize"), Radioss::FITKTimeHistoryRWALL::VGRWALL_Customize);
        _ui->comboBox_Type->addItem(tr("DEF"), Radioss::FITKTimeHistoryRWALL::VGRWALL_DEF);
        _ui->comboBox_Type->addItem(tr("FN"), Radioss::FITKTimeHistoryRWALL::VGRWALL_FN);
        _ui->comboBox_Type->addItem(tr("FT"), Radioss::FITKTimeHistoryRWALL::VGRWALL_FT);
        // 查询已存在的刚性墙时间历程数据
        QList<Radioss::FITKAbstractTimeHistory*> timeHistoryList = _mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistory_RWALL);
        if (timeHistoryList.isEmpty())
        {
            _isCreate = true;
            _ui->lineEdit_Name->setText(_mgr->checkName("TimeHistory-RWALL"));
            // 默认启用自定义复选框
            QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
            for (QCheckBox* box : checkBoxes)
                box->setEnabled(true);
        }
        else
        {
            _isCreate = false;
            _data = dynamic_cast<Radioss::FITKTimeHistoryRWALL*>(timeHistoryList.first());
            if (!_data) return;
            // 加载名称
            _ui->lineEdit_Name->setText(_data->getDataObjectName());
            // 加载变量组类型
            auto varGroup = _data->getVarlableGroupType();
            switch (varGroup)
            {
            case Radioss::FITKTimeHistoryRWALL::VGRWALL_Customize:
                _ui->comboBox_Type->setCurrentIndex(0);
                loadCustomVariables(); // 加载自定义变量选中状态
                break;
            case Radioss::FITKTimeHistoryRWALL::VGRWALL_DEF:
                _ui->comboBox_Type->setCurrentIndex(1);
                break;
            case Radioss::FITKTimeHistoryRWALL::VGRWALL_FN:
                _ui->comboBox_Type->setCurrentIndex(2);
                break;
            case Radioss::FITKTimeHistoryRWALL::VGRWALL_FT:
                _ui->comboBox_Type->setCurrentIndex(3);
                break;
            default:
                break;
            }
        }
        initTableWidget();
    }

    void GUITimeHistoryRWALLDialog::initTableWidget()
    {
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (!caseObj) return;
        Radioss::FITKRadiossSolution* solution = caseObj->getCurrentSolution();
        if (!solution) return;
        Radioss::FITKRadiossRWallManager* rWallManager = solution->getRWallManager();
        if (!rWallManager) return;
        int count = rWallManager->getDataCount();
        // 设置表格列
        _ui->tableWidget->setColumnCount(3);
        _ui->tableWidget->setHorizontalHeaderLabels({ tr("ID"), tr("Name"), tr("IsEnable") });
        _ui->tableWidget->setRowCount(count);
        // 加载ID
        QList<int> selectedProbeIds;
        if (!_isCreate && _data)
        {
            selectedProbeIds = _data->getObjIDs();
        }
        // 填充数据到表格
        for (int i = 0; i < count; ++i)
        {
            Radioss::FITKAbstractRWall* rWall = rWallManager->getDataByIndex(i);
            if (!rWall) continue;
            //ID列
            QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(rWall->getDataObjectID()));
            idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
            _ui->tableWidget->setItem(i, 0, idItem);
            //名称列
            QTableWidgetItem* nameItem = new QTableWidgetItem(rWall->getDataObjectName());
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            _ui->tableWidget->setItem(i, 1, nameItem);
            // 选中状态列
            QTableWidgetItem* checkItem = new QTableWidgetItem();
            checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            checkItem->setCheckState(selectedProbeIds.contains(rWall->getDataObjectID())
                ? Qt::Checked : Qt::Unchecked);
            _ui->tableWidget->setItem(i, 2, checkItem);
        }
        _ui->tableWidget->resizeColumnsToContents();
    }

    void GUITimeHistoryRWALLDialog::loadCustomVariables()
    {
        if (!_data) return;
        QList<Radioss::FITKTimeHistoryRWALL::TimeHistoryRWALLVariable> customVars = _data->getCustomizeVariable();
        // 清空所有复选框选中状态
        QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
        for (QCheckBox* box : checkBoxes)
            box->setChecked(false);
        // 根据自定义变量列表设置选中状态
        for (auto var : customVars)
        {
            switch (var)
            {
            case Radioss::FITKTimeHistoryRWALL::FNX: _ui->checkBox_FNX->setChecked(true); break;
            case Radioss::FITKTimeHistoryRWALL::FNY: _ui->checkBox_FNY->setChecked(true); break;
            case Radioss::FITKTimeHistoryRWALL::FNZ: _ui->checkBox_FNZ->setChecked(true); break;
            case Radioss::FITKTimeHistoryRWALL::FTX: _ui->checkBox_FTX->setChecked(true); break;
            case Radioss::FITKTimeHistoryRWALL::FTY: _ui->checkBox_FTY->setChecked(true); break;
            case Radioss::FITKTimeHistoryRWALL::FTZ: _ui->checkBox_FTZ->setChecked(true); break;
            default: break;
            }
        }
    }

    QList<Radioss::FITKTimeHistoryRWALL::TimeHistoryRWALLVariable> GUITimeHistoryRWALLDialog::collectCustomVariables()
    {
        QList<Radioss::FITKTimeHistoryRWALL::TimeHistoryRWALLVariable> customVars;
        if (_ui->checkBox_FNX->isChecked()) customVars.append(Radioss::FITKTimeHistoryRWALL::FNX);
        if (_ui->checkBox_FNY->isChecked()) customVars.append(Radioss::FITKTimeHistoryRWALL::FNY);
        if (_ui->checkBox_FNZ->isChecked()) customVars.append(Radioss::FITKTimeHistoryRWALL::FNZ);
        if (_ui->checkBox_FTX->isChecked()) customVars.append(Radioss::FITKTimeHistoryRWALL::FTX);
        if (_ui->checkBox_FTY->isChecked()) customVars.append(Radioss::FITKTimeHistoryRWALL::FTY);
        if (_ui->checkBox_FTZ->isChecked()) customVars.append(Radioss::FITKTimeHistoryRWALL::FTZ);
        return customVars;
    }

    QList<int> GUITimeHistoryRWALLDialog::collectSelectedProbeIds()
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

    void GUITimeHistoryRWALLDialog::on_pushButton_OK_clicked()
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
        Radioss::FITKTimeHistoryRWALL::THRWALLVariableGroup varGroup = static_cast<Radioss::FITKTimeHistoryRWALL::THRWALLVariableGroup>(
            _ui->comboBox_Type->currentData().toInt());
        QList<Radioss::FITKTimeHistoryRWALL::TimeHistoryRWALLVariable> customVars;
        if (varGroup == Radioss::FITKTimeHistoryRWALL::VGRWALL_Customize)
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
            _data = new Radioss::FITKTimeHistoryRWALL();
            _mgr->appendDataObj(_data);
        }
        // 设置数据对象的所有属性
        _data->setDataObjectName(name);
        _data->setVarlableGroupType(varGroup);
        if (varGroup == Radioss::FITKTimeHistoryRWALL::VGRWALL_Customize)
        {
            _data->setCustomizeVariable(customVars);
        }
        _data->setObjIDs(selectedProbeIds);
        accept();
    }

    void GUITimeHistoryRWALLDialog::on_pushButton_Cancle_clicked()
    {
        reject();
    }

    void GUITimeHistoryRWALLDialog::on_comboBox_Type_currentIndexChanged(int index)
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
            //  编辑模式时重新加载自定义变量,否则默认勾选第一个变量
            if (!_isCreate && _data && _data->getVarlableGroupType() == Radioss::FITKTimeHistoryRWALL::VGRWALL_Customize)
                loadCustomVariables();
            else 
                _ui->checkBox_FNX->setChecked(true);
            break;
        case 1: // DEF模式（FNX,FNY,FNZ,FTX,FTY,FTZ）
            _ui->checkBox_FNX->setChecked(true);
            _ui->checkBox_FNY->setChecked(true);
            _ui->checkBox_FNZ->setChecked(true);
            _ui->checkBox_FTX->setChecked(true);
            _ui->checkBox_FTY->setChecked(true);
            _ui->checkBox_FTZ->setChecked(true);
            break;
        case 2: // FN模式（FNX,FNY,FNZ）
            _ui->checkBox_FNX->setChecked(true);
            _ui->checkBox_FNY->setChecked(true);
            _ui->checkBox_FNZ->setChecked(true);
            break;
        case 3: // FT模式（FTX,FTY,FTZ）
            _ui->checkBox_FTX->setChecked(true);
            _ui->checkBox_FTY->setChecked(true);
            _ui->checkBox_FTZ->setChecked(true);
            break;
        default:
            break;
        }
    }
} // namespace GUI