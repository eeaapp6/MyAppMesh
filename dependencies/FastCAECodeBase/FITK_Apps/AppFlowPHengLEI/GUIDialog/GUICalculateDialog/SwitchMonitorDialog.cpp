/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SwitchMonitorDialog.h"
#include "ui_SwitchMonitorDialog.h"
#include "CalculateDriver.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFEnum.hpp"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundary.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramTaskManager.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIResidaulProcess.h"
#include <QFileDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QVBoxLayout>


namespace GUI
{
    SwitchMonitorDialog::SwitchMonitorDialog(EventOper::ParaWidgetInterfaceOperator * oper) :
        GUIDialogBase(FITKAPP->getGlobalData()->getMainWindow()), _oper(oper)
    {
        _ui = new Ui::SwitchMonitorDialog();
        _ui->setupUi(this);
        init();

        this->setWindowTitle(tr(" Residual Monitoring Settings"));
    }

    SwitchMonitorDialog::~SwitchMonitorDialog()
    {
        if (_ui)delete _ui;
    }

    void SwitchMonitorDialog::init()
    {
		//// 创建一个 QTableWidget，设置行数为 2，列数为 6
		//QTableWidget *tableWidget = new QTableWidget(6, 2, this);

		//// 设置表头
		//tableWidget->setHorizontalHeaderLabels({ "Variables", "Monitor"});

		//// 填充每一行
		//for (int i = 0; i < 6; ++i) {
		//	// 设置变量名称
		//	QTableWidgetItem *variableItem = new QTableWidgetItem(QString("Variable %1").arg(i + 1));
		//	tableWidget->setItem(i, 0, variableItem);

		//	// 创建 QCheckBox 控件并添加到第二列
		//	QCheckBox *checkBox = new QCheckBox();
		//	tableWidget->setCellWidget(i, 1, checkBox);
		//}

		//_ui->verticalLayout_2->addWidget(tableWidget);

		// 设置布局
		//QVBoxLayout *layout = new QVBoxLayout(this);
		//layout->addWidget(tableWidget);
		//setLayout(layout);
    }

	//void SwitchMonitorDialog::on_btn_Import_clicked()
	//{
	//	QString defaultDir = "C:/Users/YourUsername/Documents";

	//	QString fileName = QFileDialog::getOpenFileName(this,
	//		tr("Open Executable"), "",
	//		tr("Executable files (*.exe);;All files (*)"));
	//	_ui->lineEdit_Name->setText(fileName);
	//}

    void SwitchMonitorDialog::on_pushButton_OK_clicked()
    {
		Interface::FITKPHengLEIData* data_ = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		if (!data_) return;
		Interface::FITKPHengLEIResidaulProcess* residaulProcess = data_->getResidaulProcess();

		residaulProcess->SetEnableAverageRes(_ui->averagerRes->isChecked());
		residaulProcess->SetEnableMaxRes(_ui->maxRes->isChecked());
		residaulProcess->SetEnableMaxResCoorX(_ui->maxResCoorX->isChecked());
		residaulProcess->SetEnableMaxResCoorY(_ui->maxResCoorY->isChecked());
		residaulProcess->SetEnableMaxResCoorZ(_ui->maxResCoorZ->isChecked());
        this->accept();
    }

    void SwitchMonitorDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }
}