/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "NewProjectDialog.h"
#include "ui_NewProjectDialog.h"

#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QMessageBox>

#include <QPushButton>
#include <QLineEdit>
#include <QIcon>

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIBoundary.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEISpatialManager.h"


namespace GUI
{
    NewProjectDialog::NewProjectDialog()   
    {
        _ui = new Ui::NewProjectDialog();
        _ui->setupUi(this);
		btn_import = _ui->btn_import;
		btn_import->setIcon(QIcon("D:/FastCAE/AppFlowPH/AppFlowPHengLEI/Resources/icons/add.png"));// 设置按钮图标
		btn_import->setIconSize(QSize(50, 50)); // 设置图标大小
		btn_import->setText(""); // 清空按钮文本
		btn_import->setStyleSheet("border: none;"); // 去掉边框
		init();


        this->setWindowTitle(tr("New Project"));
    }

    NewProjectDialog::~NewProjectDialog()
    {
        if (_ui)delete _ui;

    }

    void NewProjectDialog::init()
    {
      
    }


    void NewProjectDialog::hideEvent(QHideEvent * event)
    {

    }

    void NewProjectDialog::showEvent(QShowEvent * event)
    {

    }


	void NewProjectDialog::on_btn_import_clicked()
	{
		// 打开文件夹选择对话框
		QString directoryPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"));

		// 检查用户是否选择了路径
		if (directoryPath.isEmpty())
			return; // 如果用户取消选择，直接返回

		// 显示选择的路径到lineWorkingDirectory
		_ui->lineWorkingDirectory->setText(directoryPath);

	}

	void NewProjectDialog::on_pushButton_Start_clicked()
	{
	
		// 获取项目名称、工作目录、Spatial 类型
		QString projectName = _ui->lineProjectName->text();
		QString workingDirectory = _ui->lineWorkingDirectory->text();
		QString spatialType = _ui->comboBox->currentText(); //从下拉菜单获取用户选择 

		//检查是否输入了项目名称和工作目录
		if (projectName.isEmpty() || workingDirectory.isEmpty()) {
			QMessageBox::warning(this, tr("Warning"), tr("Please enter a project name and select a directory."));
			return;
		}
		
		Interface::FITKPHengLEIData* physicsData = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();

		// 生成项目文件夹路径
		QString projectPath = QDir(workingDirectory).filePath(projectName);

		// 创建项目文件夹
		QDir dir;
		if (dir.mkpath(projectPath)) {
			QMessageBox::information(this, tr("Success"), tr("Project folder created successfully!"));
			this->accept(); // 项目创建成功，关闭对话框并返回 QDialog::Accepted
		}
		else {
			QMessageBox::warning(this, tr("Error"), tr("Failed to create project folder."));
			this->reject(); // 项目创建失败，关闭对话框并返回 QDialog::Rejected
		}



	}

	void NewProjectDialog::on_pushButton_Cancel_clicked()
	{
		// 调用 reject() 函数以关闭对话框并返回 QDialog::Rejected 状态
		this->reject();
	}

}

