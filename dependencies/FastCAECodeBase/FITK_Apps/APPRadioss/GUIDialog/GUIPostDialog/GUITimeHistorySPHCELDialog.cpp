/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUITimeHistorySPHCELDialog.h"
#include "ui_GUITimeHistorySPHCELDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractTimeHistory.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistorySPHCEL.h"

#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/PickedDataProvider.h"
#include "PickDataProvider/PickedData.h"
#include <QMessageBox>
#include <QCheckBox>

namespace GUI
{
    GUITimeHistorySPHCELDialog::GUITimeHistorySPHCELDialog(Core::FITKActionOperator* oper, QWidget* parent)
        : Core::FITKDialog(parent), _oper(oper)
        , _ui(new Ui::GUITimeHistorySPHCELDialog)
    {
        _ui->setupUi(this);

        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKRadiossSolution* solution = caseObj->getCurrentSolution();
        if (!solution)return;
        _mgr = solution->getTimeHietoryManager();
        if (!_mgr)return;

        //获取拾取器，绑定拾取结束信号
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        //清除拾取对象所有连接
        if (pickProvider != nullptr)
        {
            QObject::disconnect(pickProvider, SIGNAL(sigDataPicked()), nullptr, nullptr);
            connect(pickProvider, &GraphData::PickedDataProvider::sigPickOver, this, &GUITimeHistorySPHCELDialog::pickFinishedOper);
        }

        init();
    }

    GUITimeHistorySPHCELDialog::~GUITimeHistorySPHCELDialog()
    {
        if (_ui)
            delete _ui;
        //所有的类在执行完毕后，将清空对3维的操作
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        if (!pickProvider) return;
        pickProvider->clearPickedData();
        ////拾取对象 关闭
        ////拾取方式 空
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBNone, GUI::GUIPickInfo::PickMethod::PMNone);
    }

    void GUITimeHistorySPHCELDialog::enablePickSig(GUI::GUIPickInfo::PickObjType objType, GUI::GUIPickInfo::PickMethod methodType)
    {
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType(objType);
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod(methodType);
        //设置拾取方式
        GUI::GUIPickInfo::SetPickInfo(pickInfo);
    }

    void GUITimeHistorySPHCELDialog::pickFinishedOper()
    {
        //获取拾取数据提供者实例
        GraphData::PickedDataProvider* provider = GraphData::PickedDataProvider::getInstance();
        if (provider == nullptr) return;
        //获取拾取列表
        QList<GraphData::PickedData*> pickList = provider->getPickedList();
        //计算拾取到的总数量
        int count = 0;
        for (GraphData::PickedData* pickData : pickList)
        {
            if (pickData == nullptr) continue;
            QList<GraphData::PickDataInfo> pickInfo = pickData->getPickOtherInfos();
            if (pickInfo.size() == 0) return;
            int id = pickInfo.first()._id;
            _ui->lineEdit_NodeID->setText(QString::number(id));
        }
        return;
    }

    void GUITimeHistorySPHCELDialog::init()
    {
        setWindowTitle(tr("TimeHistory-SPHCEL"));
        // 初始化变量组下拉框
        _ui->comboBox_Type->addItem(tr("Customize"), Radioss::FITKTimeHistorySPHCEL::VGSPHCEL_Customize);
        _ui->comboBox_Type->addItem(tr("ALL"), Radioss::FITKTimeHistorySPHCEL::VGSPHCEL_ALL);
        _ui->comboBox_Type->addItem(tr("DEF"), Radioss::FITKTimeHistorySPHCEL::VGSPHCEL_DEF);
        // 查询已存在的SPH粒子时间历程数据
        QList<Radioss::FITKAbstractTimeHistory*> timeHistoryList = _mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistory_SPHCEL);
        if (timeHistoryList.isEmpty())
        {
            _isCreate = true;
            _ui->lineEdit_Name->setText(_mgr->checkName("TimeHistory-SPHCEL"));
            // 默认启用自定义复选框
            QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
            for (QCheckBox* box : checkBoxes)
                box->setEnabled(true);
        }
        else
        {
            _isCreate = false;
            _data = dynamic_cast<Radioss::FITKTimeHistorySPHCEL*>(timeHistoryList.first());
            if (!_data) return;
            // 加载名称
            _ui->lineEdit_Name->setText(_data->getDataObjectName());
            // 加载节点ID和名称
            _ui->lineEdit_NodeID->setText(QString::number(_data->getSPHNodeID()));
            _ui->lineEdit_NodeName->setText(_data->getSPHNodeName());
            // 加载变量组类型
            auto varGroup = _data->getVarlableGroupType();
            switch (varGroup)
            {
            case Radioss::FITKTimeHistorySPHCEL::VGSPHCEL_Customize:
                _ui->comboBox_Type->setCurrentIndex(0);
                loadCustomVariables(); // 加载自定义变量选中状态
                break;
            case Radioss::FITKTimeHistorySPHCEL::VGSPHCEL_ALL:
                _ui->comboBox_Type->setCurrentIndex(1);
                break;
            case Radioss::FITKTimeHistorySPHCEL::VGSPHCEL_DEF:
                _ui->comboBox_Type->setCurrentIndex(2);
                break;
            default:
                break;
            }
        }
    }

    void GUITimeHistorySPHCELDialog::loadCustomVariables()
    {
        if (!_data) return;
        QList<Radioss::FITKTimeHistorySPHCEL::TimeHistorySPHCELVariable> customVars = _data->getCustomizeVariable();
        // 清空所有复选框选中状态
        QList<QCheckBox*> checkBoxes = _ui->groupBox->findChildren<QCheckBox*>();
        for (QCheckBox* box : checkBoxes)
            box->setChecked(false);
        // 根据自定义变量列表设置选中状态
        for (auto var : customVars)
        {
            switch (var)
            {
            case Radioss::FITKTimeHistorySPHCEL::OFF: _ui->checkBox_OFF->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::SX: _ui->checkBox_SX->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::SY: _ui->checkBox_SY->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::SZ: _ui->checkBox_SZ->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::SXY: _ui->checkBox_SXY->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::SYZ: _ui->checkBox_SYZ->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::SXZ: _ui->checkBox_SXZ->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::IE: _ui->checkBox_IE->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::DENS: _ui->checkBox_DENS->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::VOL: _ui->checkBox_VOL->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::WFVIS: _ui->checkBox_WFVIS->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::DIAMETER: _ui->checkBox_DIAMETER->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::TEMP: _ui->checkBox_TEMP->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::PLAS: _ui->checkBox_PLAS->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::PLSR: _ui->checkBox_PLSR->setChecked(true); break;
            case Radioss::FITKTimeHistorySPHCEL::VPLA: _ui->checkBox_VPLA->setChecked(true); break;
            default: break;
            }
        }
    }

    QList<Radioss::FITKTimeHistorySPHCEL::TimeHistorySPHCELVariable> GUITimeHistorySPHCELDialog::collectCustomVariables()
    {
        QList<Radioss::FITKTimeHistorySPHCEL::TimeHistorySPHCELVariable> customVars;
        if (_ui->checkBox_OFF->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::OFF);
        if (_ui->checkBox_SX->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::SX);
        if (_ui->checkBox_SY->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::SY);
        if (_ui->checkBox_SZ->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::SZ);
        if (_ui->checkBox_SXY->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::SXY);
        if (_ui->checkBox_SYZ->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::SYZ);
        if (_ui->checkBox_SXZ->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::SXZ);
        if (_ui->checkBox_IE->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::IE);
        if (_ui->checkBox_DENS->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::DENS);
        if (_ui->checkBox_VOL->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::VOL);
        if (_ui->checkBox_WFVIS->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::WFVIS);
        if (_ui->checkBox_DIAMETER->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::DIAMETER);
        if (_ui->checkBox_TEMP->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::TEMP);
        if (_ui->checkBox_PLAS->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::PLAS);
        if (_ui->checkBox_PLSR->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::PLSR);
        if (_ui->checkBox_VPLA->isChecked()) customVars.append(Radioss::FITKTimeHistorySPHCEL::VPLA);
        return customVars;
    }

    void GUITimeHistorySPHCELDialog::on_pushButton_Pick_clicked()
    {
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBMeshVert, GUI::GUIPickInfo::PickMethod::PMSingle);
    }

    void GUITimeHistorySPHCELDialog::on_pushButton_OK_clicked()
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
        if (_ui->lineEdit_NodeName->text().isEmpty())
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please enter a valid  node name!"), QMessageBox::Ok);
            return;
        }
        // 校验节点ID
        bool nodeIdOk = false;
        int nodeId = _ui->lineEdit_NodeID->text().toInt(&nodeIdOk);
        if (!nodeIdOk || nodeId <= 0)
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please enter a valid node ID!"), QMessageBox::Ok);
            return;
        }
        // 收集并校验自定义变量（仅自定义模式需校验）
        Radioss::FITKTimeHistorySPHCEL::THSPHCELVariableGroup varGroup = static_cast<Radioss::FITKTimeHistorySPHCEL::THSPHCELVariableGroup>(
            _ui->comboBox_Type->currentData().toInt());
        QList<Radioss::FITKTimeHistorySPHCEL::TimeHistorySPHCELVariable> customVars;
        if (varGroup == Radioss::FITKTimeHistorySPHCEL::VGSPHCEL_Customize)
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
            _data = new Radioss::FITKTimeHistorySPHCEL();
            _mgr->appendDataObj(_data);
        }
        // 设置数据对象的所有属性
        _data->setDataObjectName(name);
        _data->setVarlableGroupType(varGroup);
        if (varGroup == Radioss::FITKTimeHistorySPHCEL::VGSPHCEL_Customize)
        {
            _data->setCustomizeVariable(customVars);
        }
        _data->setSPHNodeID(nodeId);
        _data->setSPHNodeName(_ui->lineEdit_NodeName->text());
        accept();
    }
    void GUITimeHistorySPHCELDialog::on_pushButton_Cancle_clicked()
    {
        reject();
    }

    void GUITimeHistorySPHCELDialog::on_comboBox_Type_currentIndexChanged(int index)
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
            // 编辑模式时重新加载自定义变量
            if (!_isCreate && _data && _data->getVarlableGroupType() == Radioss::FITKTimeHistorySPHCEL::VGSPHCEL_Customize)
                loadCustomVariables();
            else
                _ui->checkBox_OFF->setChecked(true);
            break;
        case 1: // ALL模式（所有变量）
            for (QCheckBox* box : checkBoxes)
                box->setChecked(true);
            break;
        case 2: // DEF模式（OFF, SX, SY, SZ, SXY, SYZ, SXZ, IE, DENS, VOL, WFVIS, DIAMETER）
            _ui->checkBox_OFF->setChecked(true);
            _ui->checkBox_SX->setChecked(true);
            _ui->checkBox_SY->setChecked(true);
            _ui->checkBox_SZ->setChecked(true);
            _ui->checkBox_SXY->setChecked(true);
            _ui->checkBox_SYZ->setChecked(true);
            _ui->checkBox_SXZ->setChecked(true);
            _ui->checkBox_IE->setChecked(true);
            _ui->checkBox_DENS->setChecked(true);
            _ui->checkBox_VOL->setChecked(true);
            _ui->checkBox_WFVIS->setChecked(true);
            _ui->checkBox_DIAMETER->setChecked(true);
            break;
        default:
            break;
        }
    }
} // namespace GUI