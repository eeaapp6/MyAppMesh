/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIParaWriterGrid.h"
#include <QTextStream>
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIMeshProcess.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamComboxVector.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDoubleList.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include <QFileInfo>

namespace Interface
{
    bool FITKPHengLEIParmWriterGrid::writeParamFile()
    {
		//创建文件
		FITKPHengLEIAbsParaWriter::writeParamFile();
		if (!this->openFileStream()) return false;

        //获取网格转换参数
		Interface::FITKPHengLEIData* physicData = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		Interface::FITKPHengLEIMeshProcess* meshProcess = physicData->getMeshProcess();
		Core::FITKParameter* meshData = meshProcess->getAdditionalMeshData();

		Core::FITKAbstractEasyParam* meshDataGridDimension = meshData->getDataByName("Grid Dimension");
		Core::FITKParamCombox* comboxMeshDataGridDimension = dynamic_cast<Core::FITKParamCombox*>(meshDataGridDimension);
		QString valueDataGridDimension = comboxMeshDataGridDimension->getCurrentValue();

		Core::FITKAbstractEasyParam* meshDataGridType = meshData->getDataByName("Grid Type");
		Core::FITKParamCombox* comboxMeshDataGridType = dynamic_cast<Core::FITKParamCombox*>(meshDataGridType);
		QString valueDataGridType = comboxMeshDataGridType->getCurrentValue();

		Core::FITKParameter* meshConversionParams = meshProcess->getAdditionalMeshConversion();
		Core::FITKAbstractEasyParam* axisRotateCount = meshConversionParams->getDataByName("Axis Rotate Count");
		Core::FITKParamInt* doubleAxisRotateCount = dynamic_cast<Core::FITKParamInt*>(axisRotateCount);
		int valueAxisRotateCount = doubleAxisRotateCount->getValue();

		Core::FITKAbstractEasyParam* rotateOrderParam = meshConversionParams->getDataByName("Axis Rotate Order");
		Core::FITKParamComboxVector* rotateOrderComboxVector = dynamic_cast<Core::FITKParamComboxVector*>(rotateOrderParam);
		QList<Core::FITKParamCombox*> rotateOrderComboxList = rotateOrderComboxVector->getDatas();

		Core::FITKAbstractEasyParam* rotateAnglesParam = meshConversionParams->getDataByName("Axis Rotate Angles");
		Core::FITKParamDoubleList* rotateAnglesComboxVector = dynamic_cast<Core::FITKParamDoubleList*>(rotateAnglesParam);
		QList<Core::FITKParamDouble*> rotateAnglesComboxList = rotateAnglesComboxVector->getDoubleDatas();

		// 提取旋转顺序（轴名称）的值
		QStringList rotateOrderValues;
		//for (Core::FITKParamCombox* combox : rotateOrderComboxList) {
		//	if (combox) {
		//		rotateOrderValues.append(QString::number(combox->getIndex()+1));
		//	}
		//}
		for (int i = 0; i < valueAxisRotateCount; i++)
		{
			Core::FITKParamCombox* combox = rotateOrderComboxList[i];
			rotateOrderValues.append(QString::number(combox->getIndex() + 1));
		}


		// 提取旋转角度的值
		QStringList rotateAngleValues;
		//for (Core::FITKParamCombox* combox : rotateAnglesComboxList) {
		//	if (combox) {
		//		rotateAngleValues.append(combox->getCurrentValue());
		//	}
		//}
		for (int i = 0; i < valueAxisRotateCount; i++)
		{
			Core::FITKParamDouble* rotateAngles = rotateAnglesComboxList[i];
			rotateAngleValues.append(QString::number(rotateAngles->getValue()));
		}

		// 将 QStringList 转换为字符串，并用逗号分隔
		QString rotateOrderString = rotateOrderValues.join(", ");
		QString rotateAnglesString = rotateAngleValues.join(", ");

        QTextStream* stream = this->getStream();
        if (stream == nullptr) return false;

        *stream << "#########################################################################" << endl;
        *stream << "#                           Grid data type                              #" << endl;
        *stream << "#########################################################################" << endl;
		if (valueDataGridType == "Unstruct")
		{
			*stream << "int gridtype   = 0" << ";" << endl;
		}
		else
		{
			*stream << "int gridtype   = 1" << ";" << endl;
		}

		if (valueAxisRotateCount == 0)
		{
			*stream << "int nAxisRotateTimes      = " << valueAxisRotateCount << ";" << endl;
		}
		else
		{
			*stream << "int nAxisRotateTimes      = " << valueAxisRotateCount << ";" << endl;
			*stream << "int axisRotateOrder[]     = [" << rotateOrderString << "];" << endl;
			*stream << "double axisRotateAngles[] = [" << rotateAnglesString << "];" << endl;
		}
		*stream << "int from_gtype = 2;" << endl;
		
		Interface::FITKUnstructuredFluidMeshVTK* mesh = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
		QString fileName = mesh->getFieldMesh()->getUserData(Core::FITKUserData::FITKUserRole + 10).toString();
		Interface::FITKBoundaryMeshVTKManager* boundMgr = mesh->getBoundaryMeshManager();
		if (!boundMgr || boundMgr->getDataCount() <= 0)
		{
			AppFrame::FITKMessageError(tr("Err: not Mesh"));
			return false;
		}

		*stream << "#########################################################################" << endl;
		*stream << "#                           File path                                   #" << endl;
		*stream << "#########################################################################" << endl;
		*stream << "// from_gfile: path of original data file for unstructure grid convert from." << endl;
		*stream << "// out_gfile:  path of target file for grid convert to, *.fts type of file usually." << endl;
		*stream << "string from_gfile = " << "\"./grid/" + QFileInfo(fileName).baseName() << ".cgns" << "\";" << endl;
		*stream << "string out_gfile = " << "\"./grid/" + QFileInfo(fileName).baseName() << ".fts" << "\";" << endl;

        this->closeFileStream();
        return true;

    }
}



