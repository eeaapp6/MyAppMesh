/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIParaWriterPartion.h"
#include <QTextStream>
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIMeshProcess.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamComboxVector.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include <QFileInfo>

namespace Interface
{
    bool FITKPHengLEIParmWriterPartion::writeParamFile()
    {
		//创建文件
		FITKPHengLEIAbsParaWriter::writeParamFile();
		if (!this->openFileStream()) return false;

		//获取网格参数
		Interface::FITKPHengLEIData* physicData = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		Interface::FITKPHengLEIMeshProcess* meshProcess = physicData->getMeshProcess();
		Core::FITKParameter* meshData = meshProcess->getAdditionalMeshData();

		Core::FITKAbstractEasyParam* meshDataGridType = meshData->getDataByName("Grid Type");
		Core::FITKParamCombox* comboxMeshDataGridType = dynamic_cast<Core::FITKParamCombox*>(meshDataGridType);
		QString valueDataGridType = comboxMeshDataGridType->getCurrentValue();
		//获取网格分区参数
		Core::FITKParameter* meshPartionParams = meshProcess->getAdditionalMeshPartion();
		Core::FITKAbstractEasyParam* maxProcessors = meshPartionParams->getDataByName("Max Processors");
		Core::FITKParamInt* doublemaxProcessors = dynamic_cast<Core::FITKParamInt*>(maxProcessors);
		int valuemaxProcessors = doublemaxProcessors->getValue();

		Core::FITKAbstractEasyParam* multiGridCount = meshPartionParams->getDataByName("Multi-grid Count");
		Core::FITKParamInt* doublemultiGridCount = dynamic_cast<Core::FITKParamInt*>(multiGridCount);
		int valuemultiGridCount = doublemultiGridCount->getValue();

        QTextStream* stream = this->getStream();
        if (stream == nullptr) return false;

        *stream << "#########################################################################" << endl;
        *stream << "#                           Partion data type                           #" << endl;
        *stream << "#########################################################################" << endl;
		if (valueDataGridType == "Unstruct")
		{
			*stream << "int pgridtype = 0" << ";" << endl;
		}
		else
		{
			*stream << "int pgridtype = 1" << ";" << endl;
		}
		*stream << "int maxproc   = " << valuemaxProcessors << ";" << endl;
		
		Interface::FITKUnstructuredFluidMeshVTK* mesh = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
		QString fileName = mesh->getFieldMesh()->getUserData(Core::FITKUserData::FITKUserRole + 10).toString();
		
		*stream << "string original_grid_file  = " << "\"./grid/" + QFileInfo(fileName).baseName() << ".fts" << "\";" << endl;
		*stream << "string partition_grid_file = " << "\"./grid/" + QFileInfo(fileName).baseName() << "__" << valuemaxProcessors << ".fts" << "\";" << endl;
		
		*stream << "int numberOfMultifile = 1;" << endl;
		
		if (valueDataGridType == "Struct")
		{
			*stream << "int numberOfMultigrid = " << valuemultiGridCount << ";" << endl;
		}


        this->closeFileStream();

        return true;

    }
}



