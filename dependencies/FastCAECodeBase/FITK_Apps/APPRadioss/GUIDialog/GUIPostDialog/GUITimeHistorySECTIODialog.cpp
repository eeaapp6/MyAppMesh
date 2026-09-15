/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUITimeHistorySECTIODialog.h"
#include "ui_GUITimeHistorySECTIODialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractTimeHistory.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistorySECTIO.h"

#include <QMessageBox>
#include <QCheckBox>

namespace GUI
{
    GUITimeHistorySECTIODialog::GUITimeHistorySECTIODialog(Core::FITKActionOperator* oper, QWidget* parent)
        : Core::FITKDialog(parent), _oper(oper)
        , _ui(new Ui::GUITimeHistorySECTIODialog)
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

    GUITimeHistorySECTIODialog::~GUITimeHistorySECTIODialog()
    {
        if (_ui)
            delete _ui;
    }

    void GUITimeHistorySECTIODialog::init()
    {
        setWindowTitle(tr("TimeHistory-SECTIO"));
        _ui->tabWidget->setTabText(0, tr("Time History Sectio"));
        _ui->tabWidget->setTabText(1, tr("Probe Data"));
        // 初始化变量组下拉框
        _ui->comboBox_Type->addItem(tr("Customize"), Radioss::FITKTimeHistorySECTIO::VGSECTIO_Customize);
        _ui->comboBox_Type->addItem(tr("DEF"), Radioss::FITKTimeHistorySECTIO::VGSECTIO_DEF);
        _ui->comboBox_Type->addItem(tr("FN"), Radioss::FITKTimeHistorySECTIO::VGSECTIO_FN);
        _ui->comboBox_Type->addItem(tr("FT"), Radioss::FITKTimeHistorySECTIO::VGSECTIO_FT);
        _ui->comboBox_Type->addItem(tr("M"), Radioss::FITKTimeHistorySECTIO::VGSECTIO_M);
        _ui->comboBox_Type->addItem(tr("CENTER"), Radioss::FITKTimeHistorySECTIO::VGSECTIO_CENTER);
        _ui->comboBox_Type->addItem(tr("GLOBAL"), Radioss::FITKTimeHistorySECTIO::VGSECTIO_GLOBAL);
        _ui->comboBox_Type->addItem(tr("LOCAL"), Radioss::FITKTimeHistorySECTIO::VGSECTIO_LOCAL);
        // 查询已存在的截面力时间历程数据
        QList<Radioss::FITKAbstractTimeHistory*> timeHistoryList = _mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistory_SECTIO);
        if (timeHistoryList.isEmpty())
        {
            _isCreate = true;
            _ui->lineEdit_Name->setText(_mgr->checkName("TimeHistory-SECTIO"));
            // 默认启用自定义复选框
            QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
            for (QCheckBox* box : checkBoxes)
                box->setEnabled(true);
        }
        else
        {
            _isCreate = false;
            _data = dynamic_cast<Radioss::FITKTimeHistorySECTIO*>(timeHistoryList.first());
            if (!_data) return;
            // 加载名称
            _ui->lineEdit_Name->setText(_data->getDataObjectName());
            // 加载变量组类型
            auto varGroup = _data->getVarlableGroupType();
            switch (varGroup)
            {
            case Radioss::FITKTimeHistorySECTIO::VGSECTIO_Customize:
                _ui->comboBox_Type->setCurrentIndex(0);
                loadCustomVariables(); // 加载自定义变量选中状态
                break;
            case Radioss::FITKTimeHistorySECTIO::VGSECTIO_DEF:
                _ui->comboBox_Type->setCurrentIndex(1);
                break;
            case Radioss::FITKTimeHistorySECTIO::VGSECTIO_FN:
                _ui->comboBox_Type->setCurrentIndex(2);
                break;
            case Radioss::FITKTimeHistorySECTIO::VGSECTIO_FT:
                _ui->comboBox_Type->setCurrentIndex(3);
                break;
            case Radioss::FITKTimeHistorySECTIO::VGSECTIO_M:
                _ui->comboBox_Type->setCurrentIndex(4);
                break;
            case Radioss::FITKTimeHistorySECTIO::VGSECTIO_CENTER:
                _ui->comboBox_Type->setCurrentIndex(5);
                break;
            case Radioss::FITKTimeHistorySECTIO::VGSECTIO_GLOBAL:
                _ui->comboBox_Type->setCurrentIndex(6);
                break;
            case Radioss::FITKTimeHistorySECTIO::VGSECTIO_LOCAL:
                _ui->comboBox_Type->setCurrentIndex(7);
                break;
            default:
                break;
            }
        }
        initTableWidget();
    }

    void GUITimeHistorySECTIODialog::initTableWidget()
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
            if (!probe || probe->getType() != Radioss::FITKAbstractProbe::ProbeType::Probe_Section)
                continue;
            // 插入新行
            _ui->tableWidget->insertRow(row);
            // 探头ID列
            QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(probe->getDataObjectID()));
            idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
            _ui->tableWidget->setItem(row, 0, idItem);
            // 探头名称列
            QTableWidgetItem* nameItem = new QTableWidgetItem(probe->getDataObjectName());
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            _ui->tableWidget->setItem(row, 1, nameItem);
            // 选中状态列
            QTableWidgetItem* checkItem = new QTableWidgetItem();
            checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            checkItem->setCheckState(selectedProbeIds.contains(probe->getDataObjectID())
                ? Qt::Checked : Qt::Unchecked);
            _ui->tableWidget->setItem(row, 2, checkItem);
            row++;
        }
        _ui->tableWidget->resizeColumnsToContents();
    }

    void GUITimeHistorySECTIODialog::loadCustomVariables()
    {
        if (!_data) return;
        QList<Radioss::FITKTimeHistorySECTIO::TimeHistorySECTIOVariable> customVars = _data->getCustomizeVariable();
        // 清空所有复选框选中状态
        QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
        for (QCheckBox* box : checkBoxes)
            box->setChecked(false);
        // 根据自定义变量列表设置选中状态
        for (auto var : customVars)
        {
            switch (var)
            {
            case Radioss::FITKTimeHistorySECTIO::FNX: _ui->checkBox_FNX->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::FNY: _ui->checkBox_FNY->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::FNZ: _ui->checkBox_FNZ->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::FTX: _ui->checkBox_FTX->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::FTY: _ui->checkBox_FTY->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::FTZ: _ui->checkBox_FTZ->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::MX: _ui->checkBox_MX->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::MY: _ui->checkBox_MY->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::MZ: _ui->checkBox_MZ->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::F1: _ui->checkBox_F1->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::F2: _ui->checkBox_F2->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::F3: _ui->checkBox_F3->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::M1: _ui->checkBox_M1->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::M2: _ui->checkBox_M2->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::M3: _ui->checkBox_M3->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::CX: _ui->checkBox_CX->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::CY: _ui->checkBox_CY->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::CZ: _ui->checkBox_CZ->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::WORK: _ui->checkBox_WORK->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::WORKR: _ui->checkBox_WORKR->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::DFX: _ui->checkBox_DFX->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::DFY: _ui->checkBox_DFY->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::DFZ: _ui->checkBox_DFZ->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::DMX: _ui->checkBox_DMX->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::DMY: _ui->checkBox_DMY->setChecked(true); break;
            case Radioss::FITKTimeHistorySECTIO::DMZ: _ui->checkBox_DMZ->setChecked(true); break;
            default: break;
            }
        }
    }

    QList<Radioss::FITKTimeHistorySECTIO::TimeHistorySECTIOVariable> GUITimeHistorySECTIODialog::collectCustomVariables()
    {
        QList<Radioss::FITKTimeHistorySECTIO::TimeHistorySECTIOVariable> customVars;
        if (_ui->checkBox_FNX->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::FNX);
        if (_ui->checkBox_FNY->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::FNY);
        if (_ui->checkBox_FNZ->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::FNZ);
        if (_ui->checkBox_FTX->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::FTX);
        if (_ui->checkBox_FTY->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::FTY);
        if (_ui->checkBox_FTZ->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::FTZ);
        if (_ui->checkBox_MX->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::MX);
        if (_ui->checkBox_MY->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::MY);
        if (_ui->checkBox_MZ->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::MZ);
        if (_ui->checkBox_F1->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::F1);
        if (_ui->checkBox_F2->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::F2);
        if (_ui->checkBox_F3->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::F3);
        if (_ui->checkBox_M1->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::M1);
        if (_ui->checkBox_M2->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::M2);
        if (_ui->checkBox_M3->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::M3);
        if (_ui->checkBox_CX->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::CX);
        if (_ui->checkBox_CY->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::CY);
        if (_ui->checkBox_CZ->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::CZ);
        if (_ui->checkBox_WORK->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::WORK);
        if (_ui->checkBox_WORKR->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::WORKR);
        if (_ui->checkBox_DFX->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::DFX);
        if (_ui->checkBox_DFY->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::DFY);
        if (_ui->checkBox_DFZ->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::DFZ);
        if (_ui->checkBox_DMX->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::DMX);
        if (_ui->checkBox_DMY->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::DMY);
        if (_ui->checkBox_DMZ->isChecked()) customVars.append(Radioss::FITKTimeHistorySECTIO::DMZ);
        return customVars;
    }

    QList<int> GUITimeHistorySECTIODialog::collectSelectedProbeIds()
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

    void GUITimeHistorySECTIODialog::on_pushButton_OK_clicked()
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
        Radioss::FITKTimeHistorySECTIO::THSECTIOVariableGroup varGroup = static_cast<Radioss::FITKTimeHistorySECTIO::THSECTIOVariableGroup>(
            _ui->comboBox_Type->currentData().toInt());
        QList<Radioss::FITKTimeHistorySECTIO::TimeHistorySECTIOVariable> customVars;
        if (varGroup == Radioss::FITKTimeHistorySECTIO::VGSECTIO_Customize)
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
            _data = new Radioss::FITKTimeHistorySECTIO();
            _mgr->appendDataObj(_data);
        }
        // 设置数据对象的所有属性
        _data->setDataObjectName(name);
        _data->setVarlableGroupType(varGroup);
        if (varGroup == Radioss::FITKTimeHistorySECTIO::VGSECTIO_Customize)
        {
            _data->setCustomizeVariable(customVars);
        }
        _data->setObjIDs(selectedProbeIds);
        accept();
    }

    void GUITimeHistorySECTIODialog::on_pushButton_Cancle_clicked()
    {
        reject();
    }

    void GUITimeHistorySECTIODialog::on_comboBox_Type_currentIndexChanged(int index)
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
            if (!_isCreate && _data && _data->getVarlableGroupType() == Radioss::FITKTimeHistorySECTIO::VGSECTIO_Customize)
                loadCustomVariables();
            else 
                _ui->checkBox_FNX->setChecked(true);
            break;
        case 1: // DEF模式（FNX,FNY,FNZ,FTX,FTY,FTZ,M1,M2,M3）
            _ui->checkBox_FNX->setChecked(true);
            _ui->checkBox_FNY->setChecked(true);
            _ui->checkBox_FNZ->setChecked(true);
            _ui->checkBox_FTX->setChecked(true);
            _ui->checkBox_FTY->setChecked(true);
            _ui->checkBox_FTZ->setChecked(true);
            _ui->checkBox_M1->setChecked(true);
            _ui->checkBox_M2->setChecked(true);
            _ui->checkBox_M3->setChecked(true);
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
        case 4: // M模式（M1,M2,M3）
            _ui->checkBox_M1->setChecked(true);
            _ui->checkBox_M2->setChecked(true);
            _ui->checkBox_M3->setChecked(true);
            break;
        case 5: // CENTER模式（CX,CY,CZ）
            _ui->checkBox_CX->setChecked(true);
            _ui->checkBox_CY->setChecked(true);
            _ui->checkBox_CZ->setChecked(true);
            break;
        case 6: // GLOBAL模式（FNX,FNY,FNZ,FTX,FTY,FTZ,MX,MY,MZ）
            _ui->checkBox_FNX->setChecked(true);
            _ui->checkBox_FNY->setChecked(true);
            _ui->checkBox_FNZ->setChecked(true);
            _ui->checkBox_FTX->setChecked(true);
            _ui->checkBox_FTY->setChecked(true);
            _ui->checkBox_FTZ->setChecked(true);
            _ui->checkBox_MX->setChecked(true);
            _ui->checkBox_MY->setChecked(true);
            _ui->checkBox_MZ->setChecked(true);
            break;
        case 7: // LOCAL模式（F1,F2,F3,M1,M2,M3）
            _ui->checkBox_F1->setChecked(true);
            _ui->checkBox_F2->setChecked(true);
            _ui->checkBox_F3->setChecked(true);
            _ui->checkBox_M1->setChecked(true);
            _ui->checkBox_M2->setChecked(true);
            _ui->checkBox_M3->setChecked(true);
            break;
        default:
            break;
        }
    }
} // namespace GUI