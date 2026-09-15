/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "TurbulenceWidget.h"
#include "ui_TurbulenceWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEISolverManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIViscousTypeManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIDataManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIModels.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIViscousModel.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIViscousType.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIPhysicsHandlerFactory.h"

#include <QButtonGroup>
#include <QScrollArea>

namespace GUI
{
    TurbulenceWidget::TurbulenceWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::TurbulenceWidget();
        _ui->setupUi(this);
		_viscousTypeManager = _dataManager_PHengLEI->getViscousTypeManager();

		//获取湍流管理器
        init();
    }

    TurbulenceWidget::~TurbulenceWidget()
    {
        if (_radioGroup) delete _radioGroup;
        if (_ui)  delete _ui;
    }

    void TurbulenceWidget::init()
    {
        //界面默认设置
        _radioGroup = new QButtonGroup();
        _radioGroup->addButton(_ui->radioButton_Euler);
        _radioGroup->addButton(_ui->radioButton_Lamilar);
        _radioGroup->addButton(_ui->radioButton_RANS);
        _radioGroup->addButton(_ui->radioButton_DES);
        _radioGroup->addButton(_ui->radioButton_DDES);
        _radioGroup->addButton(_ui->radioButton_IDDES);

        _ui->radioButton_Lamilar->setChecked(true);
        //默认隐藏湍流模型参数
        _ui->pushButton_ModelUnfold->setCheckable(true);
        _ui->widget_sub->hide();
        _ui->widget_Model->hide();
        _ui->widget_ModelSub->hide();

        //更具数据对象初始化界面
        setDataToWidget();

        //信号连接
        connect(_radioGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotRadioButtonClicked()));
        connect(_ui->comboBox_Model, SIGNAL(currentIndexChanged(int)), this, SLOT(slotComboBoxModelChange(int)));
    }

    void TurbulenceWidget::updateSubWidget()
    {
        if (_data_PHengLEI == nullptr)return;
		Interface::FITKPHengLEIModels* models = _data_PHengLEI->getModels();
		Interface::FITKPHengLEIViscousModel* viscousModel = models->getViscousModel();
		Interface::FITKPHengLEIViscousType* viscousType = viscousModel->getViscousType();
		if (viscousType == nullptr) return;

        //清除子参数
        QLayoutItem* item;
        while ((item = _ui->verticalLayout_ModelSub->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater(); // 推荐使用 deleteLater，以确保小部件在适当时机被删除
            }
            delete item; // 删除布局项
        }

        //重新添加子参数
        if (viscousType->getViscousTypeParams()) {
            for (int i = 0; i < viscousType->getViscousTypeParams()->getDataCount(); i++) {
                auto dataBase = viscousType->getViscousTypeParams()->getDataByIndex(i);
                if (dataBase == nullptr)continue;
                _ui->verticalLayout_ModelSub->addWidget(new Core::FITKWidgetComLine(dataBase, this));
            }
        }
    }

    void TurbulenceWidget::setDataToWidget()
    {
        if (_data_PHengLEI == nullptr)return;
        auto solverData = _data_PHengLEI->getSolver();
        if (solverData == nullptr)return;
        
        //湍流数据是否存在
        Interface::FITKPHengLEIModels* models = _data_PHengLEI->getModels();
		if (!models) return;
		Interface::FITKPHengLEIViscousModel* viscousModel = models->getViscousModel();

		// 若ViscousModel获取的湍流类型为Lamilar，则直接返回
        if (viscousModel->getViscousEnumType() == Interface::FITKPHengLEIAbsSolver::Lamilar
            || viscousModel->getViscousEnumType() == Interface::FITKPHengLEIAbsSolver::Eular) return;

        _ui->widget_sub->show();
        _ui->widget_Model->show();
       

		auto type = viscousModel->getViscousEnumType();

        QStringList modelTypes = _viscousTypeManager->filterTurbence(solverData->getSolverType(), type);
        _ui->comboBox_Model->clear();
        _ui->comboBox_Model->addItems(modelTypes);
        _ui->comboBox_Model->setCurrentText(viscousModel->getDataObjectName());

        switch (type){
		case Interface::FITKPHengLEIAbsSolver::Lamilar: {
            _ui->radioButton_Lamilar->setChecked(true);
            break;
        }
        case Interface::FITKPHengLEIAbsSolver::Eular:{
            _ui->radioButton_Euler->setChecked(true);
            break;
        }
        case Interface::FITKPHengLEIAbsSolver::RANS:{
            _ui->radioButton_RANS->setChecked(true);
            break;
        }
		case Interface::FITKPHengLEIAbsSolver::DES: {
			_ui->radioButton_DES->setChecked(true);
			break;
		}
		case Interface::FITKPHengLEIAbsSolver::DDES: {
			_ui->radioButton_DDES->setChecked(true);
			break;
		}
		case Interface::FITKPHengLEIAbsSolver::IDDES: {
			_ui->radioButton_IDDES->setChecked(true);
			break;
		}
        }

        updateSubWidget();
    }

    void TurbulenceWidget::slotRadioButtonClicked()
    {
        if (_viscousTypeManager == nullptr)return;
        if (_data_PHengLEI == nullptr)return;
        if (_physicHandleFactory_PHengLEI == nullptr)return;
        auto solverData = _data_PHengLEI->getSolver();
        if (solverData == nullptr)return;

		Interface::FITKPHengLEIAbsSolver::PHengLEIViscousType type;
        if (_radioGroup->checkedButton() == _ui->radioButton_Lamilar) {
            _ui->widget_sub->hide();
            type = Interface::FITKPHengLEIAbsSolver::Lamilar;

           // _physicHandleFactory_PHengLEI->setViscousType("");
        }
		else if (_radioGroup->checkedButton() == _ui->radioButton_Euler) {
			_ui->widget_sub->hide();
			type = Interface::FITKPHengLEIAbsSolver::Eular;
			//_physicHandleFactory_PHengLEI->setViscousType("");
		}
        else if (_radioGroup->checkedButton() == _ui->radioButton_RANS) {
            _ui->widget_sub->show();
            _ui->widget_Model->show();
            type = Interface::FITKPHengLEIAbsSolver::RANS;
            QStringList modelTypes = _viscousTypeManager->filterTurbence(solverData->getSolverType(), type);
            _ui->comboBox_Model->clear();
            _ui->comboBox_Model->addItems(modelTypes);
            _ui->comboBox_Model->setCurrentIndex(0);

		//	_physicHandleFactory_PHengLEI->setViscousType(_ui->comboBox_Model->currentText());
        }
        else if (_radioGroup->checkedButton() == _ui->radioButton_DES) {
            _ui->widget_sub->show();
            _ui->widget_Model->show();
            type = Interface::FITKPHengLEIAbsSolver::DES;
            QStringList modelTypes = _viscousTypeManager->filterTurbence(solverData->getSolverType(), type);
            _ui->comboBox_Model->clear();
            _ui->comboBox_Model->addItems(modelTypes);
            _ui->comboBox_Model->setCurrentIndex(0);

		//	_physicHandleFactory_PHengLEI->setViscousType(_ui->comboBox_Model->currentText());
        }
		else if (_radioGroup->checkedButton() == _ui->radioButton_DDES) {
			_ui->widget_sub->show();
			_ui->widget_Model->show();
			type = Interface::FITKPHengLEIAbsSolver::DDES;
			QStringList modelTypes = _viscousTypeManager->filterTurbence(solverData->getSolverType(), type);
			_ui->comboBox_Model->clear();
			_ui->comboBox_Model->addItems(modelTypes);
			_ui->comboBox_Model->setCurrentIndex(0);

		//	_physicHandleFactory_PHengLEI->setViscousType(_ui->comboBox_Model->currentText());
		}
		else if (_radioGroup->checkedButton() == _ui->radioButton_IDDES) {
			_ui->widget_sub->show();
			_ui->widget_Model->show();
			type = Interface::FITKPHengLEIAbsSolver::IDDES;
			QStringList modelTypes = _viscousTypeManager->filterTurbence(solverData->getSolverType(), type);
			_ui->comboBox_Model->clear();
			_ui->comboBox_Model->addItems(modelTypes);
			_ui->comboBox_Model->setCurrentIndex(0);

		//	_physicHandleFactory_PHengLEI->setViscousType(_ui->comboBox_Model->currentText());
		}

        updateSubWidget();
    }

    void TurbulenceWidget::slotComboBoxModelChange(int index)
    {
        if (index == -1)return;
        if (_physicHandleFactory_PHengLEI == nullptr)return;
		_physicHandleFactory_PHengLEI->setViscousType(_ui->comboBox_Model->currentText());
        updateSubWidget();
    }

    void TurbulenceWidget::on_pushButton_ModelUnfold_clicked()
    {
        if (_ui->pushButton_ModelUnfold->isChecked()) {
            _ui->widget_ModelSub->show();
        }
        else {
            _ui->widget_ModelSub->hide();
        }

        updateSubWidget();
    }
}
