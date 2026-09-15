/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SwitchCFDPostDialog.h"
#include "ui_SwitchCFDPostDialog.h"
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
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>


namespace GUI
{
    SwitchCFDPostDialog::SwitchCFDPostDialog(EventOper::ParaWidgetInterfaceOperator * oper) :
        GUIDialogBase(FITKAPP->getGlobalData()->getMainWindow()), _oper(oper)
    {
        _ui = new Ui::SwitchCFDPostDialog();
        _ui->setupUi(this);
        init();

        this->setWindowTitle(tr("Switch to CFDPost"));
    }

    SwitchCFDPostDialog::~SwitchCFDPostDialog()
    {
        if (_ui)delete _ui;
    }

    void SwitchCFDPostDialog::init()
    {
		loadPath();
    }

	void SwitchCFDPostDialog::on_btn_Import_clicked()
	{
		QString defaultDir = "C:/Users/YourUsername/Documents";

		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open Executable"), "",
			tr("Executable files (*.exe);;All files (*)"));
		_ui->lineEdit_Name->setText(fileName);
		savePath();
		
	}

    void SwitchCFDPostDialog::on_pushButton_OK_clicked()
    {
		//// 将结果文件转换为dat格式，并且打开cfdPost软件。
		//	//创建CFDPost打开tecplot文件
		//QString PHengLEIDir = QApplication::applicationDirPath() + "/../PHengLEI";
		//QString PHengLEIResult = PHengLEIDir + "/results";

		//QFileInfo fileInfo(PHengLEIResult + "/tecflow.plt");
		//QString newExtension = ".dat";
		//// 构建新文件的完整路径
		//QString newFileName = fileInfo.baseName() + "." + newExtension;
		//QString newFilePath = PHengLEIResult + "/" + newFileName;
		//QFile(newFilePath).remove();
		//// 复制文件
		//if (!QFile::copy(PHengLEIResult + "/tecflow.plt", newFilePath)) return;


		//auto app = dynamic_cast<AppFrame::FITKApplication*>(qApp);
		//auto proGramManager = FITKAPP->getProgramTaskManager();
		//AppFrame::FITKProgramInputInfo* info = new FoamDriver::FITKOFInputInfo();
		//QStringList args;
		//args << "-i" << newFilePath;
		//info->setArgs(args);
		//auto progam = proGramManager->createProgram(1, "CalculateDriver", info);
		//if (!progam) return;
		//CalculateDriver* calDriver = dynamic_cast<CalculateDriver*>(progam);
		//if (calDriver)calDriver->setExecProgram(_ui->lineEdit_Name->text());
		////启动进程
		//progam->start();

  //      this->accept();
    }

    void SwitchCFDPostDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }
	void SwitchCFDPostDialog::loadPath()
	{
		QString PHengLEIDir = QApplication::applicationDirPath() + "/../PHengLEI";
		QString PHengLEIResult = PHengLEIDir + "/CFDPostPath";

		QFile file(PHengLEIResult +"CFDPostPath.json");
		if (file.open(QIODevice::ReadOnly)) {
			QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
			QJsonObject obj = doc.object();
			_ui->lineEdit_Name->setText(obj["path"].toString());
		}
	}
	void SwitchCFDPostDialog::savePath()
	{
		QString PHengLEIDir = QApplication::applicationDirPath() + "/../PHengLEI";
		QString PHengLEIResult = PHengLEIDir + "/CFDPostPath";

		QFile file(PHengLEIResult +"CFDPostPath.json");
		if (file.open(QIODevice::WriteOnly)) {
			QJsonObject obj;
			obj["path"] = _ui->lineEdit_Name->text();
			QJsonDocument doc(obj);
			file.write(doc.toJson());
		}
	}
}