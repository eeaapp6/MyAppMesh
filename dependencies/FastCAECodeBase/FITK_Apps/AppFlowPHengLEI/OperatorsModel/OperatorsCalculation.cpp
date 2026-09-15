/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsCalculation.h"

#include "GUIFrame/PropertyWidget.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIDialog/GUICalculateDialog/CalculationWidget.h"
#include "GUIDialog/GUICalculateDialog/PlotMonitorWidget.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIerDriver.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIParaWriterKey.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDHypersonsic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDSubsonic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDSupersonic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDTransonic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIAbsSolver.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "GUIDialog/GUICalculateDialog/PlotMonitorWidget.h"
#include <QDir>

namespace ModelOper
{
    OperatorsCalculation::OperatorsCalculation()
    {

    }

    OperatorsCalculation::~OperatorsCalculation()
    {

    }

    bool OperatorsCalculation::execGUI()
    {
        GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWindow == nullptr)return false;
        GUI::PropertyWidget* propertyWidget = mainWindow->getPropertyWidget();
        if (propertyWidget == nullptr)return false;

        GUI::CalculationWidget* widget = new GUI::CalculationWidget(this, propertyWidget);
        propertyWidget->setWidget(widget);

        return false;
    }

    bool OperatorsCalculation::execProfession()
    {
		QString PHengLEIDir = QApplication::applicationDirPath() + "/../PHengLEI";
		PHengLEIDir = QDir(PHengLEIDir).absolutePath();
		QString PHengLEIBin = PHengLEIDir + "/bin";
		QString PHengLEIGrid = PHengLEIDir + "/grid";
		QString PHengLEIResult = PHengLEIDir + "/results";

		//拷贝网格文件到grid文件夹
		Interface::FITKUnstructuredFluidMeshVTK* mesh = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
		QString fileName = mesh->getFieldMesh()->getUserData(Core::FITKUserData::FITKUserRole + 10).toString();

		if (!fileName.isEmpty()) 
		{
			QFileInfo fileInfo(fileName);
			QString destFileName = PHengLEIGrid + "/" + fileInfo.fileName();

			if (!QFile::copy(fileName, destFileName)) 
			{
				return false;
			}
		}

		Interface::FITKPHengLEIData* data_ = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		Interface::FITKPHengLEIerDriver* driver = data_->getPHengLEIDirver();

		driver->setValue("PHengLEI", PHengLEIDir);
		driver->setValue("PHengLEIBin", PHengLEIBin);
		driver->setValue("PHengLEIGrid", PHengLEIGrid);
		driver->setValue("PHengLEIResults",PHengLEIResult );

		

		driver->startDriver();
        return true;
    }

	void OperatorsCalculation::eventProcess(int index, QVariant value)
	{
		//0 == select 1== stop
		if (index == 0) {
			execProfession();
		}
		else if (index == 1) {
			Interface::FITKPHengLEIData* data_ = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
			Interface::FITKPHengLEIerDriver* driver = data_->getPHengLEIDirver();
			driver->stopDriver();
		}
	}


}

