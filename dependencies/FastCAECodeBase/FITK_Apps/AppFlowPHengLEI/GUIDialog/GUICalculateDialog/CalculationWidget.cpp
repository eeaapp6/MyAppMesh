/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "CalculationWidget.h"
#include "ui_CalculationWidget.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIMeshPara.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEICalculation.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIerDriver.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "PlotMonitorWidget.h"
#include <QToolBox>
#include <QDir>
#include <QMessageBox>

namespace GUI
{
    CalculationWidget::CalculationWidget(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::CalculationWidget();
        _ui->setupUi(this);

        init();
    }

    CalculationWidget::~CalculationWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
		if (_plotMonitorWidget) {
			delete _plotMonitorWidget;
			_plotMonitorWidget = nullptr;
		}
    }

    void CalculationWidget::init()
    {
		if (_data_PHengLEI == nullptr) return;
		_calculation = _data_PHengLEI->getCalculation();
		if (_calculation == nullptr) return;

		Core::FITKParameter* paraData = _calculation->getAdditionalSolverControlData();
		if (paraData)
		{
			int index = 0;
			for (auto d : paraData->getParameter())
			{
				if (d == nullptr) continue;
				_ui->verticalLayout->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		QVBoxLayout* convergenceVLayout = dynamic_cast<QVBoxLayout*>(_ui->gBox_Convergence_2->layout());
		Core::FITKParameter* paraData1 = _calculation->getAdditionalConvergenceData();
		if (paraData1)
		{
			int index = 0;
			for (auto d : paraData1->getParameter())
			{
				if(d == nullptr) continue;
				convergenceVLayout->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		QVBoxLayout* savingIntervalsVLayout = dynamic_cast<QVBoxLayout*>(_ui->gBox_SavingIntervals_2->layout());
		Core::FITKParameter* paraData2 = _calculation->getAdditionalSavingIntervalsData();
		if (paraData2)
		{
			for (auto d : paraData2->getParameter())
			{
				if (d == nullptr) continue;
				savingIntervalsVLayout->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}

		QVBoxLayout* parallelVLayout = dynamic_cast<QVBoxLayout*>(_ui->gBox_Parallel_2->layout());
		Core::FITKParameter* paraData3 = _calculation->getAdditionalParallelData();
		if (paraData3)
		{
			for (auto d : paraData3->getParameter())
			{
				if (d == nullptr) continue;
				parallelVLayout->addWidget(new Core::FITKWidgetComLine(d, this));
			}
		}
    }

	void CalculationWidget::on_btn_Start_clicked()
	{
		Interface::FITKPHengLEIData* data_ = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		Interface::FITKPHengLEIerDriver* driver = data_->getPHengLEIDirver();
		if (driver->isProcessRunning())
		{
			QMessageBox messageBox;
			messageBox.setWindowTitle(tr("Confirm Cancellation"));
			messageBox.setText(tr("The solution is currently in progress. Do you want to cancel the solving process?"));
			messageBox.setIcon(QMessageBox::Question);
			messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			messageBox.setDefaultButton(QMessageBox::No);

			int ret = messageBox.exec();

			if (ret == QMessageBox::Yes)
			{
				_oper->eventProcess(1);
			}
			else
			{
				return;
			}
		}

		QString PHengLEIDir = QApplication::applicationDirPath() + "/../PHengLEI";
		PHengLEIDir = QDir(PHengLEIDir).absolutePath();
		QString PHengLEIBin = PHengLEIDir + "/bin";
		QString PHengLEIGrid = PHengLEIDir + "/grid";
		QString PHengLEIResult = PHengLEIDir + "/results";

		// 删除Bin文件夹下的所有文件
		QDir PHengLEIBinDir(PHengLEIBin);
		Core::RemoveDir(PHengLEIBin);
		Core::RemoveDir(PHengLEIResult);

		QDir PHengLEIGridDir(PHengLEIGrid);
		QStringList gridFiles = PHengLEIGridDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
		for (int i = 0; i < gridFiles.size(); ++i)
		{
			const QString& fileName = gridFiles.at(i);
			QString absoluteFilePath = PHengLEIGridDir.absoluteFilePath(fileName);
			QFile file(absoluteFilePath);
			if (!file.exists()) {
				continue;
			}
			if (!file.remove()) {
				file.setPermissions(QFile::WriteOwner | QFile::ReadOwner);
				file.remove();
			}
		}

		// 创建算例文件夹
		Core::CreateDir(PHengLEIBin);
		Core::CreateDir(PHengLEIGrid);
		Core::CreateDir(PHengLEIResult);

		// 创建残差监控界面
		if (_plotMonitorWidget == nullptr) _plotMonitorWidget = new PlotMonitorWidget();
		// 启动残差文件监控
		if (_plotMonitorWidget)
		{
			QString residualFilePath = PHengLEIResult + "/res.dat";
			_plotMonitorWidget->startMonitoring(residualFilePath);
		}

		if (_oper) {
			_oper->eventProcess(0);
		}
	}

	void CalculationWidget::on_btn_Stop_clicked()
	{
		// 创建一个消息框
		QMessageBox messageBox;
		messageBox.setWindowTitle(tr("Confirm Cancellation"));
		messageBox.setText(tr("Are you sure you want to stop the solver?"));
		messageBox.setIcon(QMessageBox::Question);
		messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		messageBox.setDefaultButton(QMessageBox::No);

		// 显示对话框并等待用户响应
		int ret = messageBox.exec();

		switch (ret) {
		case QMessageBox::Yes: 
		{
			// 停止残差文件监控
			if (_plotMonitorWidget) 
			{
				_plotMonitorWidget->stopMonitoring();
			}
			_oper->eventProcess(1);
			break;
		}
		case QMessageBox::No:
			break;
		default:
			break;
		}
	}
}

