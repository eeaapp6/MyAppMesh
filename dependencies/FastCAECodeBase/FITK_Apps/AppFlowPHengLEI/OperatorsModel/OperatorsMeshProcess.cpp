/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsMeshProcess.h"

#include "GUIFrame/PropertyWidget.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIDialog/GUICalculateDialog/MeshProcessWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIMeshProcess.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKAbstractEasyParam.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIerDriver.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIParaWriterKey.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIParaWriterGrid.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIParaWriterPartion.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include <QDir>

namespace ModelOper
{
    OperatorsMeshProcess::OperatorsMeshProcess()
    {

    }

    OperatorsMeshProcess::~OperatorsMeshProcess()
    {

    }

    bool OperatorsMeshProcess::execGUI()
    {
        GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWindow == nullptr)return false;
        GUI::PropertyWidget* propertyWidget = mainWindow->getPropertyWidget();
        if (propertyWidget == nullptr)return false;

        GUI::MeshProcessWidget* widget = new GUI::MeshProcessWidget(this, propertyWidget);
        propertyWidget->setWidget(widget);

        return false;
    }

    bool OperatorsMeshProcess::execProfession()
    {
		QString PHengLEIDir = QApplication::applicationDirPath() + "/../PHengLEI";
		PHengLEIDir = QDir(PHengLEIDir).absolutePath();
		if (PHengLEIDir.isEmpty() || !Core::CreateDir(PHengLEIDir)) return false;

		QString PHengLEIBin = PHengLEIDir + "/bin";
		if (PHengLEIBin.isEmpty() || !Core::CreateDir(PHengLEIBin)) return false;
		
		QString PHengLEIGrid = PHengLEIDir + "/grid";
		if (PHengLEIGrid.isEmpty() || !Core::CreateDir(PHengLEIGrid)) return false;

		QString PHengLEIResult = PHengLEIDir + "/results";
		if (PHengLEIResult.isEmpty() || !Core::CreateDir(PHengLEIResult)) return false;

		// 删除Bin文件夹下的所有文件
		QDir PHengLEIBinDir(PHengLEIBin);
		QStringList files = PHengLEIBinDir.entryList(QDir::Files);
		foreach(QString file, files) {
			if (file == "cfd_para.hypara") continue;
			if (!PHengLEIBinDir.remove(file)) return false;
		}

		QDir PHengLEIGridDir(PHengLEIGrid);
		QFileInfoList fileList = PHengLEIGridDir.entryInfoList(QStringList() << "*.*", QDir::Files);

		for (const QFileInfo &file : fileList) {
			// 删除不是.cgns扩展名的文件
			if (file.suffix() != "cgns") {
				QFile::remove(file.absoluteFilePath());
			}
		}


		Interface::FITKPHengLEIData* physicData = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		Interface::FITKPHengLEIerDriver* driver = physicData->getPHengLEIDirver();
		if (driver == nullptr) return false;

		driver->setValue("PHengLEI", PHengLEIDir);
		driver->setValue("PHengLEIBin", PHengLEIBin);
		driver->setValue("PHengLEIGrid", PHengLEIGrid);
		driver->setValue("PHengLEIResults", PHengLEIResult);
		driver->setTaskType(Interface::FITKPHengLEIerDriver::MeshProcess);
		driver->startDriver();

        return true;
    }
}

