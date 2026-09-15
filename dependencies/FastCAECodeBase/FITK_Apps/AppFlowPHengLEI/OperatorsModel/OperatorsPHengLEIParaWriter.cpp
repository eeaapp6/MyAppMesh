/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsMeshManager.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/PropertyWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGeometryMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionGeometryRefine.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundary.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIAbsParaWriter.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIParaWriterKey.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIParaWriterGrid.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIParaWriterPartion.h"
#include "OperatorsPHengLEIParaWriter.h"




namespace ModelOper
{

	bool OperatorsPhengLEIParaWrite::execGUI()
	{
		return true;
	}

	bool OperatorsPhengLEIParaWrite::execProfession()
	{
		//写Key文件
		Interface::PHengLEITask task= Interface::PLTGrid; // !接收前端
		/*Interface::FITKPHengLEIAbsParaWriter* writerKEY = new Interface::FITKPHengLEIParaWriterKey();*/
		Interface::FITKPHengLEIParaWriterKey *writer = new Interface::FITKPHengLEIParaWriterKey();
		writer->setTask(task);	//设置任务
		writer->setFileName("../output/PHEngLEI/bin/key.hypara");
		writer->writeParamFile();
		/*writer->WriteCFDParam("./abcdefg/key.hypara");*/
		delete writer;

		if (task == Interface::PLTGrid) {
			Interface::FITKPHengLEIParmWriterGrid* writer = new Interface::FITKPHengLEIParmWriterGrid();
			writer->setFileName("../output/PHEngLEI/bin/grid_para.hypara");
			writer->writeParamFile();
			writer->closeFileStream();
		}

		if (task == Interface::PLTPartion) {
			//网格分区写出
			Interface::FITKPHengLEIParmWriterPartion* writer = new Interface::FITKPHengLEIParmWriterPartion();
			/*writer->WriteCFDParam(".abcdefg/partition.hypara");*/
			writer->setFileName("../output/PHEngLEI/bin/partion.hypara");
			writer->writeParamFile();
			writer->closeFileStream();
		}

		return true;
	}



}