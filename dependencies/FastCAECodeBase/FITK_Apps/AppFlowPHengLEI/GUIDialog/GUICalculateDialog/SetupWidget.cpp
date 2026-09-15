/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SetupWidget.h"
#include "ui_SetupWidget.h"

#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEISolverManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIPhysicsHandlerFactory.h"

#include <QButtonGroup>

#define SetupTypePos "setupWidgetSolverTypePost"
#define SetupType Interface::FITKPHengLEIAbsSolver::FITKPHengLEISolverFiltersType

namespace GUI
{
    SetupWidget::SetupWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::SetupWidget();
        _ui->setupUi(this);

        init();
    }

    SetupWidget::~SetupWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void SetupWidget::init()
    {
        _ui->pushButton_SteadyState->setProperty(SetupTypePos, QVariant::fromValue(SetupType::SteadyState));
        _ui->pushButton_SteadyState->setCheckable(true);
        _ui->pushButton_Transient->setProperty(SetupTypePos, QVariant::fromValue(SetupType::Transient));
        _ui->pushButton_Transient->setCheckable(true);
        _ui->pushButton_Compressible->setProperty(SetupTypePos, QVariant::fromValue(SetupType::Compressible));
        _ui->pushButton_Compressible->setCheckable(true);
        _ui->pushButton_Incompressible->setProperty(SetupTypePos, QVariant::fromValue(SetupType::Incompressible));
        _ui->pushButton_Incompressible->setCheckable(true);

        _ui->tableWidget->setRowCount(0);
        _ui->tableWidget->setColumnCount(1);
        //自适应
        _ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        //隐藏行表头
        _ui->tableWidget->verticalHeader()->setVisible(false);
        //隐藏列表头
        _ui->tableWidget->horizontalHeader()->setVisible(false);
        //设置不可编辑
        _ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        initCurrentType();

		initSetupType_PHengLEI();

        updateTableWidget();

		QList<QAbstractButton*> radioList = this->findChildren<QAbstractButton*>();
		for (auto radio : radioList) {
			if (radio == nullptr)continue;
			connect(radio, SIGNAL(clicked()), this, SLOT(slotTypeSelect()));
        }
    }

    void SetupWidget::updateTableWidget()
    {
        _ui->tableWidget->clear();
        _ui->tableWidget->setRowCount(0);

        for (int i = 0; i < _types.size(); i++) {
            QString name = _types[i];
            QTableWidgetItem* item = new QTableWidgetItem(name);
            _ui->tableWidget->insertRow(i);
            _ui->tableWidget->setItem(i, 0, item);
        }
    }

    void SetupWidget::on_pushButton_Select_clicked()
    {
		if (_solverManager_PHengLEI == nullptr) return;
		if (_physicHandleFactory_PHengLEI == nullptr) return;
        QTableWidgetItem* currentItem = _ui->tableWidget->currentItem();
        if (currentItem == nullptr)return;

		QString sloverType =currentItem->text();

        _ui->label_CurrentSolver->setText(currentItem->text());
		_physicHandleFactory_PHengLEI->setSolver(currentItem->text());

        if (_oper) {
            _oper->execProfession();
        }
    }

	void SetupWidget::initSetupType_PHengLEI()
	{
		if (_solverManager_PHengLEI == nullptr) return;
		_types = _solverManager_PHengLEI->getSolvers();
	}



	void SetupWidget::initCurrentType()
	{
		Interface::FITKPHengLEIData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();
		if (physicsData == nullptr)return;
		Interface::FITKPHengLEIAbsSolver* solverData = physicsData->getSolver();
		if (solverData == nullptr)return;
		_ui->label_CurrentSolver->setText(solverData->getDataObjectName());
	}


    void SetupWidget::slotTypeSelect()
    {
		QPushButton* senderObj = dynamic_cast<QPushButton*>(sender());
		if (senderObj == _ui->pushButton_SteadyState) {
			bool isChecked = _ui->pushButton_SteadyState->isChecked();
			if (isChecked == true) {
				_ui->pushButton_Transient->setChecked(false);
			}
		}
		else if (senderObj == _ui->pushButton_Transient) {
			bool isChecked = _ui->pushButton_Transient->isChecked();
			if (isChecked == true) {
				_ui->pushButton_SteadyState->setChecked(false);
			}
		}
		else if (senderObj == _ui->pushButton_Incompressible) {
			bool isChecked = _ui->pushButton_Incompressible->isChecked();
			if (isChecked == true) {
				_ui->pushButton_Compressible->setChecked(false);
			}
		}
		else if (senderObj == _ui->pushButton_Compressible) {
			bool isChecked = _ui->pushButton_Compressible->isChecked();
			if (isChecked == true) {
				_ui->pushButton_Incompressible->setChecked(false);
			}
		}

		if (_solverManager_PHengLEI == nullptr)return;
		_types.clear();
		QList<SetupType> typeList = {};
		QList<QAbstractButton*> radioList = this->findChildren<QAbstractButton*>();
		for (auto radio : radioList) {
			if (radio == nullptr)continue;
			if (radio->isChecked()) {
				typeList.append(radio->property(SetupTypePos).value<SetupType>());
			}
		}

		_types = _solverManager_PHengLEI->filterSolvers(typeList);
		updateTableWidget();
    }
}

