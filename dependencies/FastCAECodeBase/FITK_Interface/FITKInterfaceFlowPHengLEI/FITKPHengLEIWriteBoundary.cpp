/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIParaWriterGrid.h"
#include "FITKPHengLEIData.h"
#include <QTextStream>
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"
#include "FITKPHengLEIInFlow.h"
#include "FITKPHengLEIDiscretisation.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDoubleList.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include <QToolBox>
#include "FITKPHengLEIWriteBoundary.h"
#include "FITKPHengLEIAbsParaWriter.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include <QFile>
#include "FITKPHengLEIAbstractInFlowType.h"
#include "FITKPHengLEIViscousModel.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamString.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITKPHengLEIAbsSpatialDiscretisation.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKPHengLEIAbsSolver.h"
#include "FITK_Kernel/FITKEasyParam/FITKAbstractEasyParam.h"
#include "FITKPHengLEIMeshPara.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIOutputSetting.h"
#include "FITKPHengLEIBoundary.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITKPHengLEIBoundaryType.h"
#include <QFileInfo>
#include <QList>
#include <QVector>
#include <QTemporaryFile>


namespace Interface
{
	bool FITKPHengLEIWriteBoundary::writeParamFile()
	{
		FITKPHengLEIData* physicData = FITKPHengLEIData::GetPHengLEIDataFromFrame();
		FITKPHengLEIBoundaryManager* boundaryManager = physicData->getBoundaryManager();
		_boundaryManager = boundaryManager;

		FITKPHengLEIAbsParaWriter::writeParamFile();
		//获取文件流
		if (!this->openFileStream()) return false;
		QTextStream* stream = this->getStream();
		if (stream == nullptr) return false;

		auto globalData = FITKAPP->getGlobalData();
		if (globalData == nullptr)return false;
		Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData< Interface::FITKUnstructuredFluidMeshVTK>();
		if (meshData == nullptr)return false;
		Interface::FITKBoundaryMeshVTKManager* boundMeshManager = meshData->getBoundaryMeshManager();
		if (boundMeshManager == nullptr)return false;


		*stream << "int nBoundaryConditions = " << boundMeshManager->getDataCount() << ";" << endl;


		for (int i = 0; i < _boundaryManager->getDataCount();i++)
		{
			FITKPHengLEIBoundary* boundary = _boundaryManager->getDataByIndex(i);
			int meshBoundaryID = boundary->getMeshBoundaryID();
			auto meshBoundary = boundMeshManager->getDataByID(meshBoundaryID);
			// 获取边界网格名称
			 *stream << "string bcName = \"" << meshBoundary->getDataObjectName() << "\";" << endl;
			 *stream << "{" << endl;
			// 获取边界类型
			 FITKPHengLEIBoundary::PHengLEIBoundaryType boundaryType = boundary->getBoundaryEnumType();
			 switch (boundaryType)
			 {
			 case Interface::FITKPHengLEIBoundary::PHBNone:
				 break;
			 case Interface::FITKPHengLEIBoundary::PHBFarField:
				 *stream << "  int bcType = 4;" << endl;
				 writeBoundaryFarfield(stream, meshBoundaryID);
				 *stream << "}" << endl;
				 *stream << endl;
				 break;
			 case Interface::FITKPHengLEIBoundary::PHBWall:
				 *stream << "  int bcType = 2;" << endl;
				 writeBoundaryWall(stream, meshBoundaryID);
				 *stream << "}" << endl;
				 *stream << endl;
				 break;
			 case Interface::FITKPHengLEIBoundary::PHBInFlow:
				 *stream << "  int bcType = 5;" << endl;
				 writeBoundaryInFlow(stream, meshBoundaryID);
				 *stream << "}" << endl;
				 *stream << endl;
				 break;
			 case Interface::FITKPHengLEIBoundary::PHBSYMMETRY:
				 *stream << "  int bcType = 3;" << endl;
				 writeBoundarySYMMETRY(stream, meshBoundaryID);
				 *stream << "}" << endl;
				 *stream << endl;
				 break;
			 case Interface::FITKPHengLEIBoundary::PHBPressureInlet:
				 *stream << "  int bcType = 52;" << endl;
				 writeBoundaryPressureInlet(stream, meshBoundaryID);
				 *stream << "}" << endl;
				 *stream << endl;
				 break;
			 case Interface::FITKPHengLEIBoundary::PHBPressureOutlet:
				 *stream << "  int bcType = 62;" << endl;
				 writeBoundaryPressureOutlet(stream, meshBoundaryID);
				 *stream << "}" << endl;
				 *stream << endl;
				 break;
			 case Interface::FITKPHengLEIBoundary::PHBMassFlowInlet:
				 *stream << "  int bcType = 53;" << endl;
				 writeBoundaryMassFlowInlet(stream, meshBoundaryID);
				 *stream << "}" << endl;
				 *stream << endl;
				 break;
			 case Interface::FITKPHengLEIBoundary::PHBMassFlowOutlet:
				 *stream << "  int bcType = 63;" << endl;
				 writeBoundaryMassFlowOutlet(stream, meshBoundaryID);
				 *stream << "}" << endl;
				 *stream << endl;
				 break;
			 case Interface::FITKPHengLEIBoundary::PhBOutFlow:
				 *stream << "  int bcType = 6;" << endl;
				 writeBoundaryMassFlowOutFlow(stream, meshBoundaryID);
				 *stream << "}" << endl;
				 *stream << endl;
				 break;
			 default:
				 *stream << "}" << endl;
				 *stream << endl;
				 break;
			 }
		}
		this->closeFileStream();
		return true;

	}

	void FITKPHengLEIWriteBoundary::consoleMessage(int level, const QString& str)
	{
		return;
	}

	void FITKPHengLEIWriteBoundary::run()
	{
		return;
	}
	void FITKPHengLEIWriteBoundary::writeBoundaryFarfield(QTextStream * stream, int meshBoundaryID)
	{
		FITKPHengLEIBoundary* boundary = _boundaryManager->getBoundary(meshBoundaryID);
		FITKPHengLEIBoundaryType* boundaryType = boundary->getBoundaryType();
		Core::FITKParameter* boundaryTypeParams = boundaryType->getBoundaryTypeParams();
		if (boundaryType->getDataObjectName() == "Farfield1")
		{
			Core::FITKParamDouble* refMachNumberDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefMachNumber"));
			double refMachNumber = refMachNumberDouble->getValue();

			Core::FITKParamDouble* AttackedDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Attacked"));
			double Attacked = AttackedDouble->getValue();

			Core::FITKParamDouble* AngleSlideDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("AngleSlide"));
			double AngleSlide = AngleSlideDouble->getValue();

			Core::FITKParamDouble* RefReNumberDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefReNumber"));
			double RefReNumber = RefReNumberDouble->getValue();

			Core::FITKParamDouble* RefDismensionalTemperatureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDismensionalTemperature"));
			double RefDismensionalTemperature = RefDismensionalTemperatureDouble->getValue();

			*stream << "  int inflowParaType = 0; " << endl;
			*stream << "  double refMachNumber = " << refMachNumber << ";" << endl;
			*stream << "  double attackd = " << Attacked << ";" << endl;
			*stream << "  double angleSlide = " << AngleSlide << ";" << endl;
			*stream << "  double refReNumber = " << RefReNumber << ";" << endl;
			*stream << "  double refDimensionalTemperature = " << RefDismensionalTemperature << ";" << endl;
		}
		else if(boundaryType->getDataObjectName() == "Farfield2")
		{
			Core::FITKParamDouble* refMachNumberDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefMachNumber"));
			double refMachNumber = refMachNumberDouble->getValue();

			Core::FITKParamDouble* AttackedDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Attacked"));
			double Attacked = AttackedDouble->getValue();

			Core::FITKParamDouble* AngleSlideDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("AngleSlide"));
			double AngleSlide = AngleSlideDouble->getValue();

			Core::FITKParamDouble* HeightDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Height"));
			double Height = HeightDouble->getValue();

			*stream << "  int inflowParaType = 1; " << endl;
			*stream << "  double refMachNumber = " << refMachNumber << ";" << endl;
			*stream << "  double attackd = " << Attacked << ";" << endl;
			*stream << "  double angleSlide = " << AngleSlide << ";" << endl;
			*stream << "  double height = " << Height << ";" << endl;
		}else if (boundaryType->getDataObjectName() == "Farfield3")
		{
			Core::FITKParamDouble* RefDimensionalVelocityDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalVelocity"));
			double RefDimensionalVelocity = RefDimensionalVelocityDouble->getValue();

			Core::FITKParamDouble* RefDimensionalDensityDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalDensity"));
			double RefDimensionalDensity = RefDimensionalDensityDouble->getValue();

			Core::FITKParamDouble* AttackedDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Attacked"));
			double Attacked = AttackedDouble->getValue();

			Core::FITKParamDouble* AngleSlideDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("AngleSlide"));
			double AngleSlide = AngleSlideDouble->getValue();

			Core::FITKParamDouble* RefDimensionalTempatureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalTempature"));
			double RefDimensionalTempature = RefDimensionalTempatureDouble->getValue();

			*stream << "  int inflowParaType = 4; " << endl;
			*stream << "  double refDimensionalVelocity = " << RefDimensionalVelocity << ";" << endl;
			*stream << "  double refDimensionalDensity = " << RefDimensionalDensity << ";" << endl;
			*stream << "  double attackd = " << Attacked << ";" << endl;
			*stream << "  double angleSlide = " << AngleSlide << ";" << endl;
			*stream << "  double refDimensionalTemperature = " << RefDimensionalTempature << ";" << endl;
		}
		else if (boundaryType->getDataObjectName() == "Farfield4")
		{
			Core::FITKParamDouble* RefDimensionalVelocityDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalVelocity"));
			double RefDimensionalVelocity = RefDimensionalVelocityDouble->getValue();

			Core::FITKParamDouble* RefDimensionalPressureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalPressure"));
			double RefDimensionalPressure = RefDimensionalPressureDouble->getValue();

			Core::FITKParamDouble* AttackedDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Attacked"));
			double Attacked = AttackedDouble->getValue();

			Core::FITKParamDouble* AngleSlideDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("AngleSlide"));
			double AngleSlide = AngleSlideDouble->getValue();

			Core::FITKParamDouble* RefDimensionalTempatureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalTempature"));
			double RefDimensionalTempature = RefDimensionalTempatureDouble->getValue();

			*stream << "  int inflowParaType = 5; " << endl;
			*stream << "  double refDimensionalVelocity = " << RefDimensionalVelocity << ";" << endl;
			*stream << "  double refDimensionalPressure = " << RefDimensionalPressure << ";" << endl;
			*stream << "  double attackd = " << Attacked << ";" << endl;
			*stream << "  double angleSlide = " << AngleSlide << ";" << endl;
			*stream << "  double refDimensionalTemperature = " << RefDimensionalTempature << ";" << endl;
		}
		else if (boundaryType->getDataObjectName() == "Farfield5")
		{
			Core::FITKParamDouble* refMachNumberDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefMachNumber"));
			double refMachNumber = refMachNumberDouble->getValue();

			Core::FITKParamDouble* AttackedDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Attacked"));
			double Attacked = AttackedDouble->getValue();

			Core::FITKParamDouble* AngleSlideDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("AngleSlide"));
			double AngleSlide = AngleSlideDouble->getValue();

			Core::FITKParamDouble* RefDismensionalTemperatureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDismensionalTemperature"));
			double RefDismensionalTemperature = RefDismensionalTemperatureDouble->getValue();

			Core::FITKParamDouble* RefDimensionalPressureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalPressure"));
			double RefDimensionalPressure = RefDimensionalPressureDouble->getValue();

			*stream << "  int inflowParaType = 6; " << endl;
			*stream << "  double refMachNumber = " << refMachNumber << ";" << endl;
			*stream << "  double attackd = " << Attacked << ";" << endl;
			*stream << "  double angleSlide = " << AngleSlide << ";" << endl;
			*stream << "  double refDimensionalTemperature = " << RefDismensionalTemperature << ";" << endl;
			*stream << "  double refDimensionalPressure = " << RefDimensionalPressure << ";" << endl;
		}
		else if (boundaryType->getDataObjectName() == "Farfield6")
		{
			Core::FITKParamDouble* PrimDensityDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("PrimDensity"));
			double PrimDensity = PrimDensityDouble->getValue();

			Core::FITKParamDouble* PrimUDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("PrimU"));
			double PrimU = PrimUDouble->getValue();

			Core::FITKParamDouble* PrimVDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("PrimV"));
			double PrimV = PrimVDouble->getValue();

			Core::FITKParamDouble* PrimWDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("PrimW"));
			double PrimW = PrimWDouble->getValue();

			Core::FITKParamDouble* PrimPressureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("PrimPressure"));
			double PrimPressure = PrimPressureDouble->getValue();

			*stream << "  int inflowParaType = 9; " << endl;
			*stream << "  double primDensity = " << PrimDensity << ";" << endl;
			*stream << "  double primU = " << PrimU << ";" << endl;
			*stream << "  double primV = " << PrimV << ";" << endl;
			*stream << "  double primW = " << PrimW << ";" << endl;
			*stream << "  double primPressure = " << PrimPressure << ";" << endl;
		}
		else if(boundaryType->getDataObjectName() == "FarfieldNone")
		{
			
		}

	}
	void FITKPHengLEIWriteBoundary::writeBoundaryWall(QTextStream * stream, int meshBoundaryID)
	{
		FITKPHengLEIBoundary* boundary = _boundaryManager->getBoundary(meshBoundaryID);
		FITKPHengLEIBoundaryType* boundaryType = boundary->getBoundaryType();
		Core::FITKParameter* boundaryTypeParams = boundaryType->getBoundaryTypeParams();
		if (boundaryType->getDataObjectName() == "Wall1")
		{
			Core::FITKParamDouble* forceReferenceLengthDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("forceReferenceLength"));
			double forceReferenceLength = forceReferenceLengthDouble->getValue();

			Core::FITKParamDouble* forceReferenceLengthSpanWiseDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("forceReferenceLengthSpanWise"));
			double forceReferenceLengthSpanWise = forceReferenceLengthSpanWiseDouble->getValue();

			Core::FITKParamDouble* forceReferenceAreaDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("forceReferenceArea"));
			double forceReferenceArea = forceReferenceAreaDouble->getValue();

			Core::FITKParamDouble*TorqueRefXDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("TorqueRefX"));
			double TorqueRefX = TorqueRefXDouble->getValue();

			Core::FITKParamDouble* TorqueRefYDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("TorqueRefY"));
			double TorqueRefY = TorqueRefYDouble->getValue();

			Core::FITKParamDouble* TorqueRefZDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("TorqueRefZ"));
			double TorqueRefZ = TorqueRefZDouble->getValue();  


			*stream << "  double forceReferenceLength = " << forceReferenceLength << ";" << endl;
			*stream << "  double forceReferenceLengthSpanWise = " << forceReferenceLengthSpanWise << ";" << endl;
			*stream << "  double forceReferenceArea = " << forceReferenceArea << ";" << endl;
			*stream << "  double TorqueRefX = " << TorqueRefX << ";" << endl;
			*stream << "  double TorqueRefY = " << TorqueRefY << ";" << endl;
			*stream << "  double TorqueRefZ = " << TorqueRefZ << ";" << endl;
		}
		else if (boundaryType->getDataObjectName() == "Wall2")
		{
			Core::FITKParamDouble* forceReferenceLengthDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("forceReferenceLength"));
			double forceReferenceLength = forceReferenceLengthDouble->getValue();

			Core::FITKParamDouble* forceReferenceLengthSpanWiseDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("forceReferenceLengthSpanWise"));
			double forceReferenceLengthSpanWise = forceReferenceLengthSpanWiseDouble->getValue();

			Core::FITKParamDouble* forceReferenceAreaDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("forceReferenceArea"));
			double forceReferenceArea = forceReferenceAreaDouble->getValue();

			Core::FITKParamDouble*TorqueRefXDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("TorqueRefX"));
			double TorqueRefX = TorqueRefXDouble->getValue();

			Core::FITKParamDouble* TorqueRefYDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("TorqueRefY"));
			double TorqueRefY = TorqueRefYDouble->getValue();

			Core::FITKParamDouble* TorqueRefZDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("TorqueRefZ"));
			double TorqueRefZ = TorqueRefZDouble->getValue();

			Core::FITKParamInt* dumpHingeMomentDouble = dynamic_cast<Core::FITKParamInt*>(boundaryTypeParams->getDataByName("dumpHingeMoment"));
			int dumpHingeMoment = dumpHingeMomentDouble->getValue();

			Core::FITKParamDoubleList* localCoordAxis0ParamList = dynamic_cast<Core::FITKParamDoubleList*>(boundaryTypeParams->getDataByName("localCoordAxis0"));
			QList<Core::FITKParamDouble*> localCoordAxis0List = localCoordAxis0ParamList->getDoubleDatas();
			QStringList localCoordAxis0StringList = {};
			for (Core::FITKParamDouble* localCoordAxis : localCoordAxis0List)
			{
				localCoordAxis0StringList << QString::number(localCoordAxis->getValue());
			 }
			QString localCoordAxis0String = localCoordAxis0StringList.join(", ");

			Core::FITKParamDoubleList* localCoordAxis1ParamList = dynamic_cast<Core::FITKParamDoubleList*>(boundaryTypeParams->getDataByName("localCoordAxis1"));
			QList<Core::FITKParamDouble*> localCoordAxis1List = localCoordAxis1ParamList->getDoubleDatas();
			QStringList localCoordAxis1StringList = {};
			for (Core::FITKParamDouble* localCoordAxis : localCoordAxis1List)
			{
				localCoordAxis1StringList << QString::number(localCoordAxis->getValue());
			}
			QString localCoordAxis1String = localCoordAxis1StringList.join(", ");

			*stream << "  double forceReferenceLength = " << forceReferenceLength << ";" << endl;
			*stream << "  double forceReferenceLengthSpanWise = " << forceReferenceLengthSpanWise << ";" << endl;
			*stream << "  double forceReferenceArea = " << forceReferenceArea << ";" << endl;
			*stream << "  double TorqueRefX = " << TorqueRefX << ";" << endl;
			*stream << "  double TorqueRefY = " << TorqueRefY << ";" << endl;
			*stream << "  double TorqueRefZ = " << TorqueRefZ << ";" << endl;
			*stream << "  int    dumpHingeMoment   =  " << dumpHingeMoment << ";" << endl;
			*stream << "  double localCoordAxis0[] = [" << localCoordAxis0String << "];" << endl;
			*stream << "  double localCoordAxis1[] = [" << localCoordAxis1String << "];" << endl;
		}
		else if(boundaryType->getDataObjectName() == "Wall3")
		{
			Core::FITKParamInt* ViscousTypeInt = dynamic_cast<Core::FITKParamInt*>(boundaryTypeParams->getDataByName("ViscousType"));
			int ViscousType = ViscousTypeInt->getValue();

			Core::FITKParamDouble* wallTemperatureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("wallTemperature"));
			double wallTemperature = wallTemperatureDouble->getValue();

			Core::FITKParamDouble* forceReferenceAreaDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("forceReferenceArea"));
			double forceReferenceArea = forceReferenceAreaDouble->getValue();

			Core::FITKParamDouble*uWallDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("uWall"));
			double uWall = uWallDouble->getValue();

			Core::FITKParamDouble* vWallDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("vWall"));
			double vWall = vWallDouble->getValue();

			Core::FITKParamDouble* wWallDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("wWall"));
			double wWall = wWallDouble->getValue();


			*stream << "  int viscousType = " << ViscousType << ";" << endl;
			*stream << "  double wallTemperature = " << wallTemperature << ";" << endl;
			*stream << "  double forceReferenceArea = " << forceReferenceArea << ";" << endl;
			*stream << "  double uWall = " << uWall << ";" << endl;
			*stream << "  double vWall = " << vWall << ";" << endl;
			*stream << "  double wWall = " << wWall << ";" << endl;
		}
		else if (boundaryType->getDataObjectName() == "WallNone")
		{
		}

		
	}
	void FITKPHengLEIWriteBoundary::writeBoundaryInFlow(QTextStream * stream, int meshBoundaryID)
	{
		FITKPHengLEIBoundary* boundary = _boundaryManager->getBoundary(meshBoundaryID);
		FITKPHengLEIBoundaryType* boundaryType = boundary->getBoundaryType();
		Core::FITKParameter* boundaryTypeParams = boundaryType->getBoundaryTypeParams();

		if (boundaryType->getDataObjectName() == "Inflow1")
		{
			Core::FITKParamDouble* refMachNumberDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefMachNumber"));
			double refMachNumber = refMachNumberDouble->getValue();

			Core::FITKParamDouble* AttackedDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Attacked"));
			double Attacked = AttackedDouble->getValue();

			Core::FITKParamDouble* AngleSlideDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("AngleSlide"));
			double AngleSlide = AngleSlideDouble->getValue();

			Core::FITKParamDouble* RefReNumberDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefReNumber"));
			double RefReNumber = RefReNumberDouble->getValue();

			Core::FITKParamDouble* RefDismensionalTemperatureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDismensionalTemperature"));
			double RefDismensionalTemperature = RefDismensionalTemperatureDouble->getValue();

			*stream << "  int inflowParaType = 0; " << endl;
			*stream << "  double refMachNumber = " << refMachNumber << ";" << endl;
			*stream << "  double attackd = " << Attacked << ";" << endl;
			*stream << "  double angleSlide = " << AngleSlide << ";" << endl;
			*stream << "  double refReNumber = " << RefReNumber << ";" << endl;
			*stream << "  double refDimensionalTemperature = " << RefDismensionalTemperature << ";" << endl;
		}
		else if (boundaryType->getDataObjectName() == "Inflow2")
		{
			Core::FITKParamDouble* refMachNumberDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefMachNumber"));
			double refMachNumber = refMachNumberDouble->getValue();

			Core::FITKParamDouble* AttackedDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Attacked"));
			double Attacked = AttackedDouble->getValue();

			Core::FITKParamDouble* AngleSlideDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("AngleSlide"));
			double AngleSlide = AngleSlideDouble->getValue();

			Core::FITKParamDouble* HeightDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Height"));
			double Height = HeightDouble->getValue();

			*stream << "  int inflowParaType = 1; " << endl;
			*stream << "  double refMachNumber = " << refMachNumber << ";" << endl;
			*stream << "  double attackd = " << Attacked << ";" << endl;
			*stream << "  double angleSlide = " << AngleSlide << ";" << endl;
			*stream << "  double height = " << Height << ";" << endl;
		}
		else if (boundaryType->getDataObjectName() == "Inflow3")
		{
			Core::FITKParamDouble* RefDimensionalVelocityDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalVelocity"));
			double RefDimensionalVelocity = RefDimensionalVelocityDouble->getValue();

			Core::FITKParamDouble* RefDimensionalDensityDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalDensity"));
			double RefDimensionalDensity = RefDimensionalDensityDouble->getValue();

			Core::FITKParamDouble* AttackedDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Attacked"));
			double Attacked = AttackedDouble->getValue();

			Core::FITKParamDouble* AngleSlideDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("AngleSlide"));
			double AngleSlide = AngleSlideDouble->getValue();

			Core::FITKParamDouble* RefDimensionalTempatureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalTempature"));
			double RefDimensionalTempature = RefDimensionalTempatureDouble->getValue();

			*stream << "  int inflowParaType = 4; " << endl;
			*stream << "  double refDimensionalVelocity = " << RefDimensionalVelocity << ";" << endl;
			*stream << "  double refDimensionalDensity = " << RefDimensionalDensity << ";" << endl;
			*stream << "  double attackd = " << Attacked << ";" << endl;
			*stream << "  double angleSlide = " << AngleSlide << ";" << endl;
			*stream << "  double refDimensionalTemperature = " << RefDimensionalTempature << ";" << endl;
		}
		else if(boundaryType->getDataObjectName() == "Inflow4")
		{
			Core::FITKParamDouble* RefDimensionalVelocityDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalVelocity"));
			double RefDimensionalVelocity = RefDimensionalVelocityDouble->getValue();

			Core::FITKParamDouble* RefDimensionalPressureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalPressure"));
			double RefDimensionalPressure = RefDimensionalPressureDouble->getValue();

			Core::FITKParamDouble* AttackedDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Attacked"));
			double Attacked = AttackedDouble->getValue();

			Core::FITKParamDouble* AngleSlideDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("AngleSlide"));
			double AngleSlide = AngleSlideDouble->getValue();

			Core::FITKParamDouble* RefDimensionalTempatureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalTempature"));
			double RefDimensionalTempature = RefDimensionalTempatureDouble->getValue();

			*stream << "  int inflowParaType = 5; " << endl;
			*stream << "  double refDimensionalVelocity = " << RefDimensionalVelocity << ";" << endl;
			*stream << "  double refDimensionalPressure = " << RefDimensionalPressure << ";" << endl;
			*stream << "  double attackd = " << Attacked << ";" << endl;
			*stream << "  double angleSlide = " << AngleSlide << ";" << endl;
			*stream << "  double refDimensionalTemperature = " << RefDimensionalTempature << ";" << endl;
		}
		else if (boundaryType->getDataObjectName() == "Inflow5")
		{
			Core::FITKParamDouble* refMachNumberDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefMachNumber"));
			double refMachNumber = refMachNumberDouble->getValue();

			Core::FITKParamDouble* AttackedDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Attacked"));
			double Attacked = AttackedDouble->getValue();

			Core::FITKParamDouble* AngleSlideDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("AngleSlide"));
			double AngleSlide = AngleSlideDouble->getValue();

			Core::FITKParamDouble* RefDismensionalTemperatureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDismensionalTemperature"));
			double RefDismensionalTemperature = RefDismensionalTemperatureDouble->getValue();

			Core::FITKParamDouble* RefDimensionalPressureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("RefDimensionalPressure"));
			double RefDimensionalPressure = RefDimensionalPressureDouble->getValue();

			*stream << "  int inflowParaType = 6; " << endl;
			*stream << "  double refMachNumber = " << refMachNumber << ";" << endl;
			*stream << "  double attackd = " << Attacked << ";" << endl;
			*stream << "  double angleSlide = " << AngleSlide << ";" << endl;
			*stream << "  double refDimensionalTemperature = " << RefDismensionalTemperature << ";" << endl;
			*stream << "  double refDimensionalPressure = " << RefDimensionalPressure << ";" << endl;
		}
		else if (boundaryType->getDataObjectName() == "Inflow6")
		{
			Core::FITKParamDouble* PrimDensityDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("PrimDensity"));
			double PrimDensity = PrimDensityDouble->getValue();

			Core::FITKParamDouble* PrimUDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("PrimU"));
			double PrimU = PrimUDouble->getValue();

			Core::FITKParamDouble* PrimVDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("PrimV"));
			double PrimV = PrimVDouble->getValue();

			Core::FITKParamDouble* PrimWDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("PrimW"));
			double PrimW = PrimWDouble->getValue();

			Core::FITKParamDouble* PrimPressureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("PrimPressure"));
			double PrimPressure = PrimPressureDouble->getValue();

			*stream << "  int inflowParaType = 9; " << endl;
			*stream << "  double primDensity = " << PrimDensity << ";" << endl;
			*stream << "  double primU = " << PrimU << ";" << endl;
			*stream << "  double primV = " << PrimV << ";" << endl;
			*stream << "  double primW = " << PrimW << ";" << endl;
			*stream << "  double primPressure = " << PrimPressure << ";" << endl;
		}
		else if (boundaryType->getDataObjectName() == "InFlow7")
		{
			bool boundaryState = { false };
			double initialVelocity = boundaryTypeParams->getDoubleParaByName("boundaryTypeParams", boundaryState);
			*stream << "  double initU =" << initialVelocity << endl;
		}
		else if (boundaryType->getDataObjectName() == "InflowNone")
		{
			
		}
	}
	void FITKPHengLEIWriteBoundary::writeBoundarySYMMETRY(QTextStream * stream, int meshBoundaryID)
	{

	}
	void FITKPHengLEIWriteBoundary::writeBoundaryPressureInlet(QTextStream * stream, int meshBoundaryID)
	{
		FITKPHengLEIBoundary* boundary = _boundaryManager->getBoundary(meshBoundaryID);
		FITKPHengLEIBoundaryType* boundaryType = boundary->getBoundaryType();
		Core::FITKParameter* boundaryTypeParams = boundaryType->getBoundaryTypeParams();

		Core::FITKParamDouble* TotalPressureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Total Pressure"));
		double TotalPressure = TotalPressureDouble->getValue();

		Core::FITKParamDouble* TotalTemperatureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Total Temperature"));
		double TotalTemperature = TotalTemperatureDouble->getValue();

		Core::FITKParamDoubleList* DirectionInletParamList = dynamic_cast<Core::FITKParamDoubleList*>(boundaryTypeParams->getDataByName("Direction Inlet"));
		QList<Core::FITKParamDouble*> DirectionInletList = DirectionInletParamList->getDoubleDatas();
		QStringList DirectionInletStringList = {};
		for (Core::FITKParamDouble* localCoordAxis : DirectionInletList)
		{
			DirectionInletStringList << QString::number(localCoordAxis->getValue());
		}
		QString localCoordAxis1String = DirectionInletStringList.join(", ");

		*stream << "  int inflowParaType = 52; " << endl;
		*stream << "  double totalPressure = " << TotalPressure << ";" << endl;
		*stream << "  double totalTemperature = " << TotalTemperature << ";" << endl;
		*stream << "  double direction_inlet[] = [" << localCoordAxis1String << "];" << endl;

	}
	void FITKPHengLEIWriteBoundary::writeBoundaryPressureOutlet(QTextStream * stream, int meshBoundaryID)
	{
		FITKPHengLEIBoundary* boundary = _boundaryManager->getBoundary(meshBoundaryID);
		FITKPHengLEIBoundaryType* boundaryType = boundary->getBoundaryType();
		Core::FITKParameter* boundaryTypeParams = boundaryType->getBoundaryTypeParams();

		Core::FITKParamDouble* StaticPressureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Static Pressure"));
		double StaticPressure = StaticPressureDouble->getValue();

		*stream << "  int inflowParaType = 62; " << endl;
		*stream << "  double staticPressure = " << StaticPressure << ";" << endl;
	}
	void FITKPHengLEIWriteBoundary::writeBoundaryMassFlowInlet(QTextStream * stream, int meshBoundaryID)
	{
		FITKPHengLEIBoundary* boundary = _boundaryManager->getBoundary(meshBoundaryID);
		FITKPHengLEIBoundaryType* boundaryType = boundary->getBoundaryType();
		Core::FITKParameter* boundaryTypeParams = boundaryType->getBoundaryTypeParams();

		Core::FITKParamDouble* MassFlowDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Mass Flow"));
		double MassFlow = MassFlowDouble->getValue();

		Core::FITKParamDouble* TotalTemperatureDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Total Temperature"));
		double TotalTemperature = TotalTemperatureDouble->getValue();

		Core::FITKParamDoubleList* DirectionInletParamList = dynamic_cast<Core::FITKParamDoubleList*>(boundaryTypeParams->getDataByName("Direction Inlet"));
		QList<Core::FITKParamDouble*> DirectionInletList = DirectionInletParamList->getDoubleDatas();
		QStringList DirectionInletStringList = {};
		for (Core::FITKParamDouble* localCoordAxis : DirectionInletList)
		{
			DirectionInletStringList << QString::number(localCoordAxis->getValue());
		}
		QString localCoordAxis1String = DirectionInletStringList.join(", ");

		*stream << "  int inflowParaType = 53; " << endl;
		*stream << "  double massFlow = " << MassFlow << ";" << endl;
		*stream << "  double totalTemperature = " << TotalTemperature << ";" << endl;
		*stream << "  double direction_inlet[] = [" << localCoordAxis1String << "];" << endl;
	}
	void FITKPHengLEIWriteBoundary::writeBoundaryMassFlowOutlet(QTextStream * stream, int meshBoundaryID)
	{
		FITKPHengLEIBoundary* boundary = _boundaryManager->getBoundary(meshBoundaryID);
		FITKPHengLEIBoundaryType* boundaryType = boundary->getBoundaryType();
		Core::FITKParameter* boundaryTypeParams = boundaryType->getBoundaryTypeParams();

		Core::FITKParamDouble* MassFlowDouble = dynamic_cast<Core::FITKParamDouble*>(boundaryTypeParams->getDataByName("Mass Flow"));
		double MassFlow = MassFlowDouble->getValue();

		*stream << "  int inflowParaType = 63; " << endl;
		*stream << "  double massFlow = " << MassFlow << ";" << endl;
	}

	void FITKPHengLEIWriteBoundary::writeBoundaryMassFlowOutFlow(QTextStream* stream, int meshBoundaryID)
	{
		FITKPHengLEIBoundary* boundary = _boundaryManager->getBoundary(meshBoundaryID);
		FITKPHengLEIBoundaryType* boundaryType = boundary->getBoundaryType();
		bool boundaryState = { false };
		Core::FITKParameter* boundaryTypeParams = boundaryType->getBoundaryTypeParams();
		double initialPressure = boundaryTypeParams->getDoubleParaByName("Initial P", boundaryState);
		*stream << "  double initP =" << initialPressure << endl;
	}

}



