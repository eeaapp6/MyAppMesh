/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUITimeHistoryINTERDialog.h"
#include "ui_GUITimeHistoryINTERDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractTimeHistory.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryINTER.h"

#include <QMessageBox>
#include <QCheckBox>

namespace GUI
{
    GUITimeHistoryINTERDialog::GUITimeHistoryINTERDialog(Core::FITKActionOperator* oper,QWidget* parent)
        : Core::FITKDialog(parent), _oper(oper)
        , _ui(new Ui::GUITimeHistoryINTERDialog)
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

    GUITimeHistoryINTERDialog::~GUITimeHistoryINTERDialog()
    {
        if (_ui)
            delete _ui;
    }

    void GUITimeHistoryINTERDialog::init()
    {
        setWindowTitle(tr("TimeHistory-INTER"));
        _ui->tabWidget->setTabText(0, tr("Time History Inter"));
        _ui->tabWidget->setTabText(1, tr("Probe Data"));
        // 初始化变量组下拉框
        _ui->comboBox_Type->addItem(tr("Customize"), Radioss::FITKTimeHistoryINTER::VGINTER_Customize);
        _ui->comboBox_Type->addItem(tr("DEF"), Radioss::FITKTimeHistoryINTER::VGINTER_DEF);
        _ui->comboBox_Type->addItem(tr("FN"), Radioss::FITKTimeHistoryINTER::VGINTER_FN);
        _ui->comboBox_Type->addItem(tr("FT"), Radioss::FITKTimeHistoryINTER::VGINTER_FT);
        _ui->comboBox_Type->addItem(tr("AbsoluteFN"), Radioss::FITKTimeHistoryINTER::VGINTER_AbsoluteFN);
        _ui->comboBox_Type->addItem(tr("AbsoluteF"), Radioss::FITKTimeHistoryINTER::VGINTER_AbsoluteF);
        // 查询已存在的接触力时间历程数据
        QList<Radioss::FITKAbstractTimeHistory*> timeHistoryList = _mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistory_INTER);
        if (timeHistoryList.isEmpty())
        {
            _isCreate = true;
            _ui->lineEdit_Name->setText(_mgr->checkName("TimeHistory-INTER"));
            // 默认启用自定义复选框
            QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
            for (QCheckBox* box : checkBoxes)
                box->setEnabled(true);
        }
        else
        {
            _isCreate = false;
            _data = dynamic_cast<Radioss::FITKTimeHistoryINTER*>(timeHistoryList.first());
            if (!_data) return;
            // 加载名称
            _ui->lineEdit_Name->setText(_data->getDataObjectName());
            // 加载变量组类型
            auto varGroup = _data->getVarlableGroupType();
            switch (varGroup)
            {
            case Radioss::FITKTimeHistoryINTER::VGINTER_Customize:
                _ui->comboBox_Type->setCurrentIndex(0);
                loadCustomVariables(); // 加载自定义变量选中状态
                break;
            case Radioss::FITKTimeHistoryINTER::VGINTER_DEF:
                _ui->comboBox_Type->setCurrentIndex(1);
                break;
            case Radioss::FITKTimeHistoryINTER::VGINTER_FN:
                _ui->comboBox_Type->setCurrentIndex(2);
                break;
            case Radioss::FITKTimeHistoryINTER::VGINTER_FT:
                _ui->comboBox_Type->setCurrentIndex(3);
                break;
            case Radioss::FITKTimeHistoryINTER::VGINTER_AbsoluteFN:
                _ui->comboBox_Type->setCurrentIndex(4);
                break;
            case Radioss::FITKTimeHistoryINTER::VGINTER_AbsoluteF:
                _ui->comboBox_Type->setCurrentIndex(5);
                break;
            default:
                break;
            }
        }
        initTableWidget();
    }

    void GUITimeHistoryINTERDialog::initTableWidget()
    {
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (!caseObj) return;
        Radioss::FITKRadiossSolution* solution = caseObj->getCurrentSolution();
        if (!solution) return;
        Radioss::FITKInteractionManager* interManager = solution->getInteractionManager();
        if (!interManager) return;
        int count = interManager->getDataCount();
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
            Radioss::FITKAbstractInteraction* inter = interManager->getDataByIndex(i);
            if (!inter || inter->getInteractionType()!= Radioss::FITKAbstractInteraction::InteractionType::IT_MultiUsageImpact) continue;
            // 插入新行
            _ui->tableWidget->insertRow(row);
            // ID列
            QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(inter->getDataObjectID()));
            idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
            _ui->tableWidget->setItem(i, 0, idItem);
            // 名称列
            QTableWidgetItem* nameItem = new QTableWidgetItem(inter->getDataObjectName());
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            _ui->tableWidget->setItem(i, 1, nameItem);
            // 选中状态列
            QTableWidgetItem* checkItem = new QTableWidgetItem();
            checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            checkItem->setCheckState(selectedProbeIds.contains(inter->getDataObjectID())
                ? Qt::Checked : Qt::Unchecked);
            _ui->tableWidget->setItem(i, 2, checkItem);
            row++;
        }
        _ui->tableWidget->resizeColumnsToContents();
    }

    void GUITimeHistoryINTERDialog::loadCustomVariables()
    {
        if (!_data) return;
        QList<Radioss::FITKTimeHistoryINTER::TimeHistoryINTERVariable> customVars = _data->getCustomizeVariable();
        // 清空所有复选框选中状态
        QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
        for (QCheckBox* box : checkBoxes)
            box->setChecked(false);
        // 根据自定义变量列表设置选中状态
        for (auto var : customVars)
        {
            switch (var)
            {
            case Radioss::FITKTimeHistoryINTER::FNX: _ui->checkBox_FNX->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::FNY: _ui->checkBox_FNY->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::FNZ: _ui->checkBox_FNZ->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::FTX: _ui->checkBox_FTX->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::FTY: _ui->checkBox_FTY->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::FTZ: _ui->checkBox_FTZ->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::SFW: _ui->checkBox_SFW->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::MX: _ui->checkBox_MX->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::MY: _ui->checkBox_MY->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::MZ: _ui->checkBox_MZ->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::_FNX_: _ui->checkBox_FNX_Abs->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::_FNY_: _ui->checkBox_FNY_Abs->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::_FNZ_: _ui->checkBox_FNZ_Abs->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::__FN__: _ui->checkBox_FN->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::_FX_: _ui->checkBox_FX->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::_FY_: _ui->checkBox_FY->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::_FZ_: _ui->checkBox_FZ->setChecked(true); break;
            case Radioss::FITKTimeHistoryINTER::__F__: _ui->checkBox_F->setChecked(true); break;
            default: break;
            }
        }
    }

    QList<Radioss::FITKTimeHistoryINTER::TimeHistoryINTERVariable> GUITimeHistoryINTERDialog::collectCustomVariables()
    {
        QList<Radioss::FITKTimeHistoryINTER::TimeHistoryINTERVariable> customVars;
        if (_ui->checkBox_FNX->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::FNX);
        if (_ui->checkBox_FNY->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::FNY);
        if (_ui->checkBox_FNZ->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::FNZ);
        if (_ui->checkBox_FTX->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::FTX);
        if (_ui->checkBox_FTY->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::FTY);
        if (_ui->checkBox_FTZ->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::FTZ);
        if (_ui->checkBox_SFW->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::SFW);
        if (_ui->checkBox_MX->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::MX);
        if (_ui->checkBox_MY->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::MY);
        if (_ui->checkBox_MZ->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::MZ);
        if (_ui->checkBox_FNX_Abs->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::_FNX_);
        if (_ui->checkBox_FNY_Abs->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::_FNY_);
        if (_ui->checkBox_FNZ_Abs->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::_FNZ_);
        if (_ui->checkBox_FN->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::__FN__);
        if (_ui->checkBox_FX->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::_FX_);
        if (_ui->checkBox_FY->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::_FY_);
        if (_ui->checkBox_FZ->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::_FZ_);
        if (_ui->checkBox_F->isChecked()) customVars.append(Radioss::FITKTimeHistoryINTER::__F__);
        return customVars;
    }

    QList<int> GUITimeHistoryINTERDialog::collectSelectedProbeIds()
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

    void GUITimeHistoryINTERDialog::on_pushButton_OK_clicked()
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
        Radioss::FITKTimeHistoryINTER::THINTERVariableGroup varGroup = static_cast<Radioss::FITKTimeHistoryINTER::THINTERVariableGroup>(
            _ui->comboBox_Type->currentData().toInt());
        QList<Radioss::FITKTimeHistoryINTER::TimeHistoryINTERVariable> customVars;
        if (varGroup == Radioss::FITKTimeHistoryINTER::VGINTER_Customize)
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
            _data = new Radioss::FITKTimeHistoryINTER();
            _mgr->appendDataObj(_data);
        }
        // 设置数据对象的所有属性
        _data->setDataObjectName(name);
        _data->setVarlableGroupType(varGroup);
        if (varGroup == Radioss::FITKTimeHistoryINTER::VGINTER_Customize)
        {
            _data->setCustomizeVariable(customVars);
        }
        _data->setObjIDs(selectedProbeIds);
        accept();
    }

    void GUITimeHistoryINTERDialog::on_pushButton_Cancle_clicked()
    {
        reject();
    }

    void GUITimeHistoryINTERDialog::on_comboBox_Type_currentIndexChanged(int index)
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
            if (!_isCreate && _data && _data->getVarlableGroupType() == Radioss::FITKTimeHistoryINTER::VGINTER_Customize)
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
        case 4: // AbsoluteFN模式（_FNX_,_FNY_,_FNZ_,__FN__）
            _ui->checkBox_FNX_Abs->setChecked(true);
            _ui->checkBox_FNY_Abs->setChecked(true);
            _ui->checkBox_FNZ_Abs->setChecked(true);
            _ui->checkBox_FN->setChecked(true);
            break;
        case 5: // AbsoluteF模式（_FX_,_FY_,_FZ_,__F__）
            _ui->checkBox_FX->setChecked(true);
            _ui->checkBox_FY->setChecked(true);
            _ui->checkBox_FZ->setChecked(true);
            _ui->checkBox_F->setChecked(true);
            break;
        default:
            break;
        }
    }
} // namespace GUI
